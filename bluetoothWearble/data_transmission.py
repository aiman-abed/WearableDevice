import time


def data_transmission_task(compressed_audio_queue, compressed_video_queue):
    while True:
        if not compressed_audio_queue.empty() and not compressed_video_queue.empty():
            audio_data = compressed_audio_queue.get()
            video_data = compressed_video_queue.get()

            # Simulated sending to web server
            print(f"Sending {len(audio_data)} bytes of audio and {len(video_data)} bytes of video to server")

            # Here you would actually send the data to your server
            # requests.post('http://your-server.com/upload', files={'audio': audio_data, 'video': video_data})

        time.sleep(1)