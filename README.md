# Laboratorio inteligente

Resositorio con la documentación de los servicio que vayamos desarrollando.

## Servicios

* [Bot Telegram](/bot/README.md)

## Declaración del servicio

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

## Referencias

* [Repo instalación de la EspCam32](https://github.com/martuan/esp-ia)
* [Script de pYthon como servicio en Linux](http://chips.mecatronium.com/tutorial-como-correr-un-script-de-python-al-iniciar-el-raspberry-pi/)