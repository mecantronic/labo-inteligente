import os
from fastapi import FastAPI
from fastapi.responses import JSONResponse
from fastapi.encoders import jsonable_encoder
from pydantic import BaseModel
import speech_to_text_wav2vec2 as stt_w2v2

# Load model of speech to text
model, processor = stt_w2v2.load_model()

# Deploying FastAPI
app = FastAPI()

class Audio(BaseModel):
    audio_path: str

@app.get('/get-response-from-audio/')
async def get_response_from_audio(audio_path: Audio):
    
    if os.path.exists('audios_foxie/'):
        path = 'audios_foxie/'
        print(path)
    else:
        print('else')
        path = '../bot/audios_telegram/'
        print(path)
    
    # Getting audio name
    audio_name = audio_path.audio_path
    
    # Getting audio full path
    audio_path = os.path.join(path, audio_name)

    # Audio to array conversion
    audio_array = stt_w2v2.audio_to_array(audio_path)

    # Prediction
    predicted_sentence = stt_w2v2.inference_model(model, processor, audio_array)
    output = jsonable_encoder({'Prediction':predicted_sentence[0]})


    """
    ### load audio
    # print(audio_path)
    ### STT
    ### NLP
    ### Information query
    # print(audio_path{'audio_path'})
    # # direccion web desde donde solicitaremos la informacion
    # try:
    #     base_url = "http://api.openweathermap.org/data/2.5/weather?"
    #     #     city_name = "Ituzaingo"
    #     # esta es la URL completa con la informacion concatenada para realizar la petición correcta
    #     complete_url = base_url + "appid=" + api_key + "&q=" + city_name + "&units=metric"	
    #     response = requests.get(complete_url)
    #     # Obtenemos la respuesta en formato JSON
    #     info = response.json()["main"]
    # except:
    #     info = 'que decis flaco'
    """
    return JSONResponse(output)

if __name__ == '__main__':
    import os
    import uvicorn
    uvicorn.run("voice_assistant_api:app", host="127.0.0.1", port=8000, log_level="info")

    # url = 'http://127.0.0.1:8000'
    # endpoint = '/get-response-from-audio'
    # data = json.dumps({'audio_path': 'audio.wav'})
    # response = requests.post(url+endpoint, data=data)
    
