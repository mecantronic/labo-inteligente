# Sugerencias
[ ] `About`: proyecto de integración de tecnologías, de código abierto, para brindar cierto nivel de inteligencia a un laboratorio estándar.

[ ] Descripción/objetivos del repo: responder preguntas ¿qué es? ¿a quién está orientado? ¿por qué es de código abierto?

[ ] Agregar documento `contributing.md` para que quien se encuentra con el repo, pueda empezar a colaborar inmediatamente.

[ ] Mover toda la documentación de cada servicio a su propia carpeta, y enlazarla desde el README principal.

[ ] Utilizar nombres descriptivos para los readme de cada carpeta. Por ejemplo `bot_readme.md`


# Laboratorio inteligente
Repositorio con la documentación de los servicios que vayamos desarrollando.


## Servicios

* [Bot Telegram](/bot/README.md)
* [Sensores esp32](/sensores_esp32/sensores_esp32_readme.md)

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
