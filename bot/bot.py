"""
File name         : bot.py
Description       : Bot de Telegram para organizar servicio del Laboratorio de Mecatrónica
Author            : Mecantronic
Date created      : 02/01/2021
Python Version    : 3.8.10
"""

import random
from telegram import (ParseMode, Update, InlineKeyboardButton, InlineKeyboardMarkup, ReplyKeyboardMarkup, ReplyKeyboardRemove)
from telegram.ext import (Updater, CommandHandler, CallbackContext, CallbackQueryHandler, MessageHandler, Filters)
import os
import logging
from dotenv import load_dotenv
import requests, json

load_dotenv('bot.env')
token = os.getenv("TOKEN")
api_key = os.getenv("OPENWEATHERMAP_KEY")

logging.basicConfig(filename='bot.log', format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',level=logging.INFO)
logger = logging.getLogger(__name__)

services_keyboard  = [['/buscar 🔎', '/sensores 🌡'], ['/acciones 💡', '/suerte 🍀']]


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
		"/abrir_puerta \n"
		"/prender_luz \n"
		"/reproducir_musica"
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


def ambiente(update, context):
	''' Función que define el comando /ambiente.'''

	# direccion web desde donde solicitaremos la informacion
	base_url = "http://api.openweathermap.org/data/2.5/weather?"
 
	# ciudad (se lo mas especifico posible en el nombre)
	city_name = "Colegiales"
 
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


def main():
	# Crear "updater" y pasarle el token de tu bot
	updater = Updater(token, use_context=True)
	
	dp = updater.dispatcher

	# Eventos que activarán nuestro bot.
	# /comandos
	dp.add_handler(CommandHandler('start', start))
	dp.add_handler(CommandHandler('ayuda', ayuda))
	dp.add_handler(CommandHandler('salir', salir))
	dp.add_handler(CommandHandler('suerte', suerte))

	dp.add_handler(CommandHandler('acciones', acciones))

	dp.add_handler(CommandHandler('sensores', sensores))
	dp.add_handler(CommandHandler('ambiente', ambiente))

	dp.add_handler(CommandHandler('buscar', buscar))
	dp.add_handler(CommandHandler('mensaje', mensaje))

	dp.add_handler(CallbackQueryHandler(button))

	# Para manejar mensajes no programados
	dp.add_handler(MessageHandler(Filters.text & (~Filters.command), unknown))
	dp.add_handler(MessageHandler(Filters.command, unknown_command))
	dp.add_handler(MessageHandler(Filters.photo | Filters.audio | Filters.voice | \
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


def button(update: Update, context: CallbackContext):
	""" Parses el CallbackQuery y actualización del mensaje de texto
		Ref: https://core.telegram.org/bots/api#callbackquery
	"""
	
	query = update.callback_query
	query.answer()
	query.edit_message_text(text=f"Envia 👉 {query.data}")

if __name__ == '__main__':
	logging.info('InfiniemBot Iniciado...')
	main()


