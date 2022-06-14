FROM python:3.8.12-slim

WORKDIR /home/voice_assistant

RUN mkdir audios_foxie

RUN apt update 
RUN apt upgrade -y 
RUN pip3 install --upgrade pip
RUN apt install ffmpeg -y

COPY requirements.txt .
RUN pip3 install -r requirements.txt

COPY voice_assistant_api.py .
COPY speech_to_text_wav2vec2.py .
COPY run_api.py .

CMD [ "python3", "./run_api.py"]

