"""
File name         : bot.py
Description       : Bot de Telegram para organizar servicio del Laboratorio de Mecatrónica
Author            : Mecantronic
Date created      : 02/01/2021
Python Version    : 3.8.10
"""

import random
from telegram import (ParseMode, Update, InlineKeyboardButton, InlineKeyboardMarkup, ReplyKeyboardMarkup, ReplyKeyboardRemove)
from telegram.ext import (Updater, CommandHandler, CallbackContext, CallbackQueryHandler, ConversationHandler, MessageHandler, Filters)
import os
import logging
from dotenv import load_dotenv
import requests, json, socket
from datetime import datetime
import pandas

import mysql.connector

load_dotenv('bot.env')
token = os.getenv("TELEGRAM_KEY")
api_key = os.getenv("OPENWEATHERMAP_KEY")

logging.basicConfig(filename='bot.log', format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',level=logging.INFO)
logger = logging.getLogger(__name__)

services_keyboard  = [['/buscar 🔎', '/sensores 🌡'], ['/acciones 💡', '/alta']]

updater = Updater(token, use_context=True)
    
dp = updater.dispatcher
ROL=range(1)


def error_callback(update, context):
    """ Recolector de errores """

    logger.warning('[UPDATE] "%s" causa del error "%s"', update, context.error)


# -- SERVICIOS DISPONIBLES --
def ayuda(update, context: CallbackContext):
    """ Función que define el comando /ayuda."""

    update.message.reply_text(
        "🤖 ¡Acá estoy!. ¿En qué te puedo ayudar? 🤔 \n"
        "Usa /start para comenzar a interactuar conmigo 🕹.\n" 
        "Usa /salir para dejar de hablar conmigo 💔. "
    )

def solicitar_alta(update: Update, context: CallbackContext):

    user = update.message.from_user
    chat_id = update.message.chat_id
    db = mysql.connector.connect(host="localhost",user="root", password="ladesiempre", database="users")
    cur = db.cursor()
    query = f"SELECT id FROM usuarios;"
    print(query)
    cur.execute(query)
    ids=cur.fetchall()
    print(ids)
    ids = [int(id[0]) for id in ids]
    print(ids)
    if chat_id in ids:
        update.message.reply_text("Ya se te dió de alta previamente")
        return
    if len (context.args) == 0:
        try:
            
            #context.bot.send_message(chat_id = os.getenv(context.args[0].upper()+"_ID"), text="{} solicitó el alta". format(context.args[0],user.first_name), parse_mode=ParseMode.HTML)
            update.message.reply_text("Su alta fue solicitada")
            # Aca agregar la posibilidad de responder con si o no y enviarle al usuario que consulto. 
            # O usar update.message.location, para enviar la localización.()
            context.bot.send_message(chat_id = os.getenv("MANTIAGO"+"_ID"), text = f"Hola. Alguien solicitó el alta. ")
            print(user.first_name)
            print(user.id)

            db = mysql.connector.connect(host="localhost",user="root", password="ladesiempre", database="users")
            cur = db.cursor()
            query = f"INSERT INTO usuarios(id, name) VALUES({user.id},'{user.first_name} {user.last_name}')"
            cur.execute(query)
            db.commit()

        except:
            update.message.reply_text("El usuario {} no esta dentro de mis contactos disponibles 😕.". format(context.args[0]))
    else:
        update.message.reply_text("Tiene que aclarar")


def revisar_altas(update, context):

    chat_id = update.message.chat_id
    db = mysql.connector.connect(host="localhost",user="root", password="ladesiempre", database="users")
    cur = db.cursor()
    query = f"SELECT rol FROM usuarios WHERE id={chat_id};"
    cur.execute(query)
    #if chat_id == int(os.getenv('HERNAN_ID')) or chat_id == int(os.getenv('MAXI_ID')) or chat_id == int(os.getenv('NAHUE_ID')) or chat_id == int(os.getenv('MANTIAGO_ID')) or chat_id == int(os.getenv('FRAN_ID')) or chat_id == int(os.getenv('PAU_ID')):
        # Establecemos el tipo de socket/conexion
    rol=cur.fetchone()
    print(rol)
    if "".join(rol) == "Admin":

        db = mysql.connector.connect(host="localhost",user="root", password="ladesiempre", database="users")
        cur = db.cursor()
        
        query = f"SELECT * FROM usuarios WHERE rol is NULL;"
        cur.execute(query)
        
        usuarios= cur.fetchall()

        if len(usuarios)>0:
            update.message.reply_text(
                    f'Hay {len(usuarios)} usuarios que quieren darse de alta'
                )
            keyboard = [[InlineKeyboardButton('Usuario', callback_data="Usuario"), 
                        InlineKeyboardButton('Invitado', callback_data="Invitado")],
                        [InlineKeyboardButton('Rechazar', callback_data="Rechazar"),
                        InlineKeyboardButton('Dejar pendiente', callback_data="Pendiente")]]
            update.message.reply_text(
                f"Nombre: {usuarios[0][1]} \n email: {usuarios[0][2]}",
                reply_markup = InlineKeyboardMarkup(keyboard),
            )
            return ROL

        else:
            empty_list(update)
            return ConversationHandler.END
    else:
        update.message.reply_text(
                    f'No tenés permisos para hacer esto'
                )


def dar_de_alta(update: Update, context: CallbackContext):
    rol = update.message.text
    print(rol)
    db = mysql.connector.connect(host="localhost",user="root", password="ladesiempre", database="users")
    cur = db.cursor()
    query = f"SELECT * FROM usuarios WHERE rol is NULL;"
    cur.execute(query)
    usuarios = cur.fetchall()

    db = mysql.connector.connect(host="localhost", user="root", password="ladesiempre", database="users")
    cur = db.cursor()
    if rol == "Pendiente":
        usuarios = usuarios[1:]

    elif rol == "Rechazar":
        query = f"DELETE usuarios FROM usuarios WHERE id='{usuarios[0][0]}'"
        usuarios = usuarios[1:]

    elif rol == "Usuario" or rol == "Invitado":
        query = f"UPDATE usuarios SET rol='{rol}' WHERE id={usuarios[0][0]}"
        usuarios = usuarios[1:]

    cur.execute(query)
    print("row_count: ", len(usuarios))
    db.commit()
    
    if usuarios!=[]:
            keyboard = [[InlineKeyboardButton('Usuario', callback_data="Usuario"), 
                    InlineKeyboardButton('Invitado', callback_data="Invitado")],
                    [InlineKeyboardButton('Rechazar', callback_data="Rechazar"),
                     InlineKeyboardButton('Dejar pendiente', callback_data="Pendiente")]]
            
            update.message.reply_text(
                f"Nombre: {usuarios[0][1]} \n email: {usuarios[0][2]}",
                reply_markup = InlineKeyboardMarkup(keyboard),
            )
            return ROL
    else:
        empty_list(update)
        return ConversationHandler.END

def empty_list(update):
    update.message.reply_text(f'No quedan usuarios pendientes para dar de alta')

def start(update, context: CallbackContext):
    """ Función que define el comando /start. Comando principal que da inicio al bot. """
    
    # Datos del usuario que se conecta
    chat_id = update.message.chat_id
    first_name = update.message.chat.first_name
    last_name = update.message.chat.last_name
    username = update.message.chat.username
    logging.info("[NEW ACCESS] chat_id : {} - firstname : {} - lastname : {} - username {}". format(chat_id, first_name, last_name , username))

    update.message.reply_text(
        "🤖 Bienvenido {} !\n\n"
        "Estan disponibles los siguientes servicios ⚙️". format(first_name),
        reply_markup = ReplyKeyboardMarkup(
            services_keyboard , one_time_keyboard = True, input_field_placeholder = 'Servicio'
        ),
    )


    # Enviar un mensaje a un ID determinado.
    #context.bot.send_message(chat_id=update.message.chat_id, text=f"Hola! soy un bot, ¿en que te puedo ayudar {first_name}?", parse_mode=ParseMode.HTML)

    # Podemos llamar a otros comandos, sin que se haya activado en el chat (/help).
    #coin(update, context)

    # Enviar un mensaje a un ID determinado.
    #context.bot.send_message(chat_id=1211957358, text="te buscan", parse_mode=ParseMode.HTML)


def salir(update: Update, context: CallbackContext):
    """ Función que define el comando /salir."""

    user = update.message.from_user  # Otra forma tener los atributos del cliente que usa el servicio.
    logger.info("[UPDATE] Usuario %s canceló la conversacion.", user.first_name)
    
    context.bot.callback_data_cache.clear_callback_data()  # type: ignore[attr-defined]
    context.bot.callback_data_cache.clear_callback_queries()  # type: ignore[attr-defined]
    
    update.message.reply_text('Chau!👋 Espero que hablemos pronto nuevamente 😉.', reply_markup=ReplyKeyboardRemove())


def buscar(update: Update, context: CallbackContext):
    """ Función que define el comando /buscar.""" 

    user = update.message.from_user  # Otra forma tener los atributos del cliente que usa el servicio.

    logger.info("[UPDATE] Usuario %s consume el servicio %s", user.first_name, update.message.text)

    update.message.reply_text('¿A quién estas buscando? Te ayudo mandandole un msj. \n Escribi /mensaje seguido del nombre. \n Ej: /mensaje Hernan')

def prender_aire(update: Update, context: CallbackContext):
    os.system('mosquitto_pub -h 192.168.3.12 -t onoff/set -m "on"')


def apagar_aire(update: Update, context: CallbackContext):
    os.system('mosquitto_pub -h 192.168.3.12 -t onoff/set -m "off"')


def sensores(update: Update, context: CallbackContext):
    """ Función que define el comando /buscar.""" 

    user = update.message.from_user  # Otra forma tener los atributos del cliente que usa el servicio.

    logger.info("[UPDATE] Usuario %s consume el servicio %s", user.first_name, update.message.text)

    update.message.reply_text(
        "¿Qué sensor del laboratorio te gustaría consultar?\n\n"
        "/ambiente \n"
        "/ocupacion \n"
        "/ruido"
    )
    
    
def acciones(update: Update, context: CallbackContext):
    """ Función que define el comando /buscar.""" 

    user = update.message.from_user  # Otra forma tener los atributos del cliente que usa el servicio.

    logger.info("[UPDATE] Usuario %s consume el servicio %s", user.first_name, update.message.text)

    update.message.reply_text(
        "¿Qué acción del laboratorio te gustaría realizar?\n\n"
        "/puerta \n"
        "/prender_luz \n"
        "/reproducir_musica \n"
        "/prender_aire \n"
        "/apagar_aire"
        
    )

def alta(update: Update, context: CallbackContext):
    """ Función que define el comando /buscar.""" 

    user = update.message.from_user  # Otra forma tener los atributos del cliente que usa el servicio.

    logger.info("[UPDATE] Usuario %s consume el servicio %s", user.first_name, update.message.text)

    update.message.reply_text(
        "¿Qué acción de usuarios te gustaría realizar?\n\n"
        "/solicitar_alta \n"
        "/revisar_altas \n"

        
    )
    

def mensaje(update: Update, context: CallbackContext):
    """ Función que define el comando /mensaje.""" 

    user = update.message.from_user 
    if len (context.args) == 1:
        try:
            context.bot.send_message(chat_id = os.getenv(context.args[0].upper()+"_ID"), text="😀¡Hola {}! {} esta preguntando si estas en el labo 🙄.". format(context.args[0],user.first_name), parse_mode=ParseMode.HTML)
            update.message.reply_text("{}, {} ya fue avisado que lo estas buscando 👀.". format(user.first_name, context.args[0]))
            # Aca agregar la posibilidad de responder con si o no y enviarle al usuario que consulto. 
            # O usar update.message.location, para enviar la localización.()
        except:
            update.message.reply_text("El usuario {} no esta dentro de mis contactos disponibles 😕.". format(context.args[0]))
    else:
        update.message.reply_text("La cantidad de argumentos no es la correcto 😫. \n Ej /mensaje Hernan.")


def puerta(update, context):
    ''' Función para abrir la puerta '''

    # Datos del usuario que se conecta
    chat_id = update.message.chat_id
    db = mysql.connector.connect(host="localhost",user="root", password="ladesiempre", database="users")
    cur = db.cursor()
    query = f"SELECT rol FROM usuarios WHERE id={chat_id};"
    #query = f"SELECT * FROM usuarios"
    cur.execute(query)
    #if chat_id == int(os.getenv('HERNAN_ID')) or chat_id == int(os.getenv('MAXI_ID')) or chat_id == int(os.getenv('NAHUE_ID')) or chat_id == int(os.getenv('MANTIAGO_ID')) or chat_id == int(os.getenv('FRAN_ID')) or chat_id == int(os.getenv('PAU_ID')):
        # Establecemos el tipo de socket/conexion
    rol=cur.fetchone()
    if rol is None:
        context.bot.send_message(chat_id = update.message.chat_id, text = "🤷🏻‍♂️Lo siento, no tenes permisos para esta acción.", parse_mode=ParseMode.HTML)

    elif ''.join(rol)=="Admin" or ''.join(rol)=="Usuario":
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        port = 1332 # Puerto de comunicacion

        # Realizamos la conexion al la IP y puerto
        sock.connect(('192.168.3.11',port))

        # Send data to server
        data = "abrisoyyo"
        sock.send(data.encode())

        # Cerramos el socket
        sock.close()
        context.bot.send_message(chat_id = update.message.chat_id, text = "🔓 Puerta abierta", parse_mode=ParseMode.HTML)
    else:
        context.bot.send_message(chat_id = update.message.chat_id, text = "🤷🏻‍♂️Lo siento, no tenes permisos para esta acción.", parse_mode=ParseMode.HTML)


def ambiente(update, context):
    ''' Función que define el comando /ambiente.'''

    # direccion web desde donde solicitaremos la informacion
    base_url = "http://api.openweathermap.org/data/2.5/weather?"
 
    # ciudad (se lo mas especifico posible en el nombre)
    city_name = "Palermo"
 
    # esta es la URL completa con la informacion concatenada para realizar la petición correcta
    complete_url = base_url + "appid=" + api_key + "&q=" + city_name + "&units=metric"	

    # Ejecutamos la consulta
    response = requests.get(complete_url)
 
    # Obtenemos la respuesta en formato JSON
    info = response.json()["main"]

    update.message.reply_text(
        "🌡 Temperatura: {} °C\n"
        "💨 Presión: {} hPa\n"
        "💧 Humedad: {} %". format(info["temp"], info["pressure"], info["humidity"])
        )


def suerte(update, context):
    ''' Función que define el comando /suerte.
        Genera un número elatorio entre 1 y 2. '''

    user = update.message.from_user  # Otra forma tener los atributos del cliente que usa el servicio.

    logger.info("[UPDATE] Usuario %s consume el servicio %s", user.first_name, update.message.text)

    msg = "⚫️ Cara" if random.randint(1,2)==1 else "⚪️ Ceca"
    
    # Responde directametne en el canal donde se le ha hablado.
    update.message.reply_text(msg)


def unknown_command(update: Update, context: CallbackContext):
    """ Función que define mensaje para los comandos desconocidos."""

    context.bot.send_message(chat_id = update.message.chat_id, text = update.message.text + " no es un comando disponible.", parse_mode=ParseMode.HTML)


def unknown(update: Update, context: CallbackContext):
    """ Función que define mensaje para los textos desconocidos."""

    context.bot.send_message(chat_id = update.message.chat_id, text = "🤷🏻‍♂️Lo siento, no entiendo tu mensaje.", parse_mode=ParseMode.HTML)

def download_audio (update: Update, context: CallbackContext):
    """ Función que define mensaje para los textos desconocidos."""

    audio_id = context.bot.get_file(update.message.voice.file_id)    
    
    # Setting audio name
    id = str(update.message.chat_id)
    date = datetime.now().strftime("%d-%m-%Y---%H-%M-%S")
    audio_name = id + '---' + date + '.wav'   
    
    # Setting audio path to download
    path = 'audios_telegram/'
    audio_path = os.path.join(path , audio_name)
    audio_id.download(audio_path)

    update.message.reply_text(f'Hola {update.message.chat.first_name}! Foxie 🦊 está escuchando tu audio...')

    # Setting host and port to make request
    host = os.getenv('VOICE_ASSISTANT_HOST', default = 'http://127.0.0.1:')
    port = os.getenv('VOICE_ASSISTANT_PORT', default = '8000') 
    host_port = ''.join((host,port))

    # Testing
    # host_port = 'http://127.0.0.1:8000'

    # Endpoint
    endpoint = '/get-response-from-audio/'

    # Data sent to endpoint
    data_sent = json.dumps({'audio_path': audio_name})

    # Request
    response = requests.get(host_port + endpoint, data = data_sent)
    response = str(response.json()['Prediction'])

    user = update.message.from_user
    logger.info("[UPDATE] Usuario %s envió un audio", user.first_name)

    understand = 0

    if 'puerta' in response:
        try:
            puerta(update, context)
            understand = 1
        except:
            understand = 0        

    if 'temperatura' in response:
        ambiente(update, context)
        understand = 1

    if understand == 0:
        context.bot.send_message(chat_id = update.message.chat_id, text = "🤷🏻‍♂️Lo siento, no entiendo tu mensaje.", parse_mode=ParseMode.HTML)

def usuario(bot, update):  
    bot.send_message(chat_id=update.callback_query.from_user.id, text="Usuario")  

def invitado(bot, update):  
    bot.send_message(chat_id=update.callback_query.from_user.id, text="Invitado")

def rechazar(bot, update):  
    bot.send_message(chat_id=update.callback_query.from_user.id, text="Rechazar")

def pendiente(bot, update):  
    bot.send_message(chat_id=update.callback_query.from_user.id, text="Pendiente")  

def main():
    # Crear "updater" y pasarle el token de tu bot
    

    # Eventos que activarán nuestro bot.
    # /comandos
    dp.add_handler(CommandHandler('start', start))
    dp.add_handler(CommandHandler('ayuda', ayuda))
    dp.add_handler(CommandHandler('salir', salir))
    dp.add_handler(CommandHandler('suerte', suerte))
    dp.add_handler(CommandHandler('alta', alta))

    dp.add_handler(CommandHandler('acciones', acciones))
    dp.add_handler(CommandHandler('puerta', puerta))

    dp.add_handler(CommandHandler('sensores', sensores))
    dp.add_handler(CommandHandler('ambiente', ambiente))
    dp.add_handler(CommandHandler('prender_aire', prender_aire))
    dp.add_handler(CommandHandler('apagar_aire', apagar_aire))

    dp.add_handler(CommandHandler('buscar', buscar))
    dp.add_handler(CommandHandler('mensaje', mensaje))

    dp.add_handler(CallbackQueryHandler(usuario, pattern='Usuario'))
    dp.add_handler(CallbackQueryHandler(invitado, pattern='Invitado'))
    dp.add_handler(CallbackQueryHandler(rechazar, pattern='Rechazar'))
    dp.add_handler(CallbackQueryHandler(pendiente, pattern='Pendiente'))

    
    conv_handler = ConversationHandler(
        entry_points=[CommandHandler('revisar_altas', revisar_altas)],
        fallbacks=[],
        states={ROL: [MessageHandler(Filters.text, dar_de_alta)]},
    )

    dp.add_handler(conv_handler)
    dp.add_handler(CommandHandler('solicitar_alta', solicitar_alta))


    dp.add_handler(MessageHandler(Filters.audio | Filters.voice, download_audio))

    # Para manejar mensajes no programados
    dp.add_handler(MessageHandler(Filters.text & (~Filters.command), unknown))
    dp.add_handler(MessageHandler(Filters.command, unknown_command))
    dp.add_handler(MessageHandler(Filters.photo | \
        Filters.video | Filters.sticker | Filters.document | Filters.location | Filters.contact, \
        unknown))

    # Manejo de errores
    dp.add_error_handler(error_callback)
    
    # Comienza el bot
    updater.start_polling()
    
    # Lo deja a la escucha. Evita que se detenga.
    updater.idle()


# -- SERVICIOS PENDIENTES --
def services(update: Update, context: CallbackContext):
    """ Función que define el comando /buscar.""" 

    user = update.message.from_user  # Otra forma tener los atributos del cliente que usa el servicio.

    logger.info("[UPDATE] Usuario %s consume el servicio %s", user.first_name, update.message.text)
    
    keyboard = [
        [InlineKeyboardButton("Hernan", callback_data='/mensaje')],
        [InlineKeyboardButton("Maxi", callback_data='/mensaje')],
    ]

    reply_markup = InlineKeyboardMarkup(keyboard)

    update.message.reply_text('¿A quién estas buscando? Te ayudo mandandole un msj a:', reply_markup=reply_markup)


if __name__ == '__main__':
    logging.info('InfiniemBot Iniciado...')
    main()