FROM python:3.8.12-slim

WORKDIR /home/bot

RUN mkdir audios_telegram

RUN apt update && apt upgrade -y \ 
    && pip3 install --upgrade pip
    
COPY requirements.txt .
RUN pip3 install -r requirements.txt

COPY bot.py .
COPY bot.log .
COPY bot.env .

CMD [ "python3", "./bot.py"]

