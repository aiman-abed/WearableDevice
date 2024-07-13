import threading
import queue
from web_server_audio import web_server_and_audio_task
from video_recording import video_recording_task
from audio_recording import audio_recording_task
from audio_compression import audio_compression_task
from video_compression import video_compression_task
from data_transmission import data_transmission_task

if __name__ == "__main__":
    audio_queue = queue.Queue()
    video_queue = queue.Queue()
    compressed_audio_queue = queue.Queue()
    compressed_video_queue = queue.Queue()

    threads = [
        threading.Thread(target=web_server_and_audio_task, args=(audio_queue,)),
        threading.Thread(target=video_recording_task, args=(video_queue,)),
        threading.Thread(target=audio_recording_task, args=(audio_queue,)),
        threading.Thread(target=audio_compression_task, args=(audio_queue, compressed_audio_queue)),
        threading.Thread(target=video_compression_task, args=(video_queue, compressed_video_queue)),
        threading.Thread(target=data_transmission_task, args=(compressed_audio_queue, compressed_video_queue))
    ]

    for thread in threads:
        thread.start()

    for thread in threads:
        thread.join()