import os
import torch
import librosa
from transformers import Wav2Vec2ForCTC, Wav2Vec2Processor

def load_test_dataset():
    from datasets import load_dataset

    LANG_ID = "es"
    SAMPLES = 10

    test_dataset = load_dataset("common_voice", LANG_ID, split=f"test[:{SAMPLES}]")

    return test_dataset

def speech_file_to_array(batch):
    
    speech_array, sampling_rate = librosa.load(batch["path"], sr=16_000)
    batch["speech"] = speech_array
    batch["sentence"] = batch["sentence"].upper()
    
    return batch

def audio_to_array(audio_path):
    audio_array, _ = librosa.load(audio_path, sr = 16_000)
    return audio_array

def load_model(MODEL_ID = "jonatasgrosman/wav2vec2-large-xlsr-53-spanish"):
    if os.path.isdir('model_STT_wav2vec2_esp') and os.path.isdir('processor_STT_wav2vec2_esp'):
        
        model = Wav2Vec2ForCTC.from_pretrained('model_STT_wav2vec2_esp')
        
        processor = Wav2Vec2Processor.from_pretrained('processor_STT_wav2vec2_esp')

        print('Model loaded from local dirs')
    
    else:    
        print('Model not found locally. Downloading and saving in model_STT_wav2_vec2_esp/ and processor_STT_wav2vec2_esp/ dirs')
        model = Wav2Vec2ForCTC.from_pretrained(MODEL_ID)
        processor = Wav2Vec2Processor.from_pretrained(MODEL_ID)

        model.save_pretrained('model_STT_wav2vec2_esp')
        processor.save_pretrained('processor_STT_wav2vec2_esp')

        
    return model, processor

def inference_model(model, processor, audio):
    
    input = processor(audio, sampling_rate=16_000, return_tensors="pt", padding=True)

    with torch.no_grad():
        logits = model(input.input_values, attention_mask = input.attention_mask).logits

    predicted_ids = torch.argmax(logits, dim=-1)
    predicted_sentence = processor.batch_decode(predicted_ids)

    return predicted_sentence


if __name__ == '__main__':
    import time

    # test_dataset = test_dataset.map(speech_file_to_array)

        # Load of speech test
    speech_test = audio_to_array('test_voice.wav')

        # Load the model and processor
    begin = time.time()
    model, processor = load_model()
    end_model = time.time()

        # Inference the speech test
    predicted_sentence = inference_model(model, processor, speech_test)
    end_inference = time.time()

    print('-'*100)
    print('Prediction: ', predicted_sentence[0])
    print('-'*100)
    print('Time loading or downloading model:', str(end_model - begin),' s')
    print('Time inferencing the audio test:', str(end_inference - end_model),' s')