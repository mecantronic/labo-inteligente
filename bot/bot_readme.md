# InfiniemBot

Es una implementación de la libreria [python-telegram-bot](https://python-telegram-bot.org/) para generar un bot que ayude con las tareas básicas del laboratorio. Además se pretende agregar servicios que lean e interactúen con sensores, actuadores y periféricos del laboratorio.

to be continued ...

## Referencias:
* [Ejemplo inicial](https://github.com/python-telegram-bot/python-telegram-bot/wiki/Extensions-%E2%80%93-Your-first-Bot)
* [Filtros para MessageHandler](https://github.com/python-telegram-bot/python-telegram-bot/wiki/Extensions-%E2%80%93-Advanced-Filters)
* [Ejemplos varios](https://github.com/python-telegram-bot/python-telegram-bot/tree/master/examples)
* [Emojis](https://www.iemoji.com/)
* [Generar Token](https://core.telegram.org/bots#6-botfather)


## Datos

name: MecaBot
username: infiniemBot
link: http://t.me/InfiniemBot
TOKEN: en archivo bot.env
Versión: 0.1 (beta)


## Get started

Dentro de la carpeta `/bot`:

``` bash
    python3 -m venv bot --system-site-packages  # crear un enviroment.
    source bot/bin/activate  # activar el enviroment.
    pip3 install -r requirements .txt  # instalar las librerias necesarias.
    nano bot.env  # archivo con las variables de entorno.
```

## Run

Como script desde terminal.
``` bash
    python3 bot.py
```

## Despliegue en Rpi

Usando las siguientes [instrucciones](https://gist.github.com/maxiyommi/6c3635f076cbeba8c563c4aa3e08589a)
[Correr script de python como servicio en linux](http://chips.mecatronium.com/tutorial-como-correr-un-script-de-python-al-iniciar-el-raspberry-pi/)

## Declaración del servicio en linux

``` bash
[Unit]
Description=Servicio Telegram Bot
After=multi-user.target

[Service]
Type=simple
User=pi
ExecStart=/usr/bin/python3.7 /home/pi/labo_inteligente/bot/bot.py
Restart=on-abort

[Install]
WantedBy=multi-user.target
```

Ejecutandolo via ssh

``` bash
ssh -i "<path del archivo *.pem>" <usuario>@<IP_server> -f "<instrucciones de bash separadas por ';'>"
```

ssh <usuario>@<IP_server> -f "cd labo_inteligente/; python3 bot/bot.py"

## Referencias

* [Repo instalación de la EspCam32](https://github.com/martuan/esp-ia)
* [Script de pYthon como servicio en Linux](http://chips.mecatronium.com/tutorial-como-correr-un-script-de-python-al-iniciar-el-raspberry-pi/)
