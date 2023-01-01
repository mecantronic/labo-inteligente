import sounddevice as sd
from scipy.io.wavfile import write
from argparse import ArgumentParser, Namespace

FS = 44100  # Sample rate

def get_args():
    """Get command line arguments.
    Returns:
        Dictionary of arguments.
    """
    parser = ArgumentParser()
    parser.add_argument("--time", required=False, type=str, help="Lenght in seconds of the audio recording",
                        default = '5')
    parser.add_argument("--name", required=False, type=str, help="Name of the audio file",
                        default = 'output.wav')
    
    return vars(parser.parse_args()) # returns as dictionary with vars()

def recorder(time, name):
    print('> Recording\n...')
    myrecording = sd.rec(int(time * FS), samplerate=FS, channels=1)
    sd.wait()  # Wait until recording is finished
    write(name, FS, myrecording)  # Save as WAV file 
    print(f'> Recording saved as {name}')
    
if __name__ == '__main__':
    args = get_args()
    recorder(int(args['time']),args['name'])