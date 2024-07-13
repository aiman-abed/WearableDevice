import time
from pydub import AudioSegment


def audio_compression_task(audio_queue, compressed_audio_queue):
    while True:
        if not audio_queue.empty():
            audio_data = audio_queue.get()
            audio = AudioSegment(
                audio_data,
                sample_width=2,
                frame_rate=44100,
                channels=1
            )
            mp3_data = audio.export(format="mp3").read()
            compressed_audio_queue.put(mp3_data)

            # Delete extra files (simulated)
            print("Deleting extra audio files...")
        time.sleep(1)