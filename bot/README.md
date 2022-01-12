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

``` bash
    python3 -m venv boot --system-site-packages  # crear un enviroment.
    source boot/bin/activate  # activar el enviroment.
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
