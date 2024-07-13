import time
import numpy as np

def audio_recording_task(audio_queue):
    while True:
        # This is a placeholder. Implement actual audio recording here.
        audio_data = np.random.bytes(44100 * 2)  # 1 second of 16-bit audio at 44.1kHz
        audio_queue.put(audio_data)
        time.sleep(1)