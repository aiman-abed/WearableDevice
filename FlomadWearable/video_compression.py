import time
import cv2


def video_compression_task(video_queue, compressed_video_queue):
    while True:
        frames = []
        for _ in range(30):  # Collect 1 second of video
            if not video_queue.empty():
                frames.append(video_queue.get())

        if frames:
            # Convert to MJPEG (this is a simplification)
            _, jpeg = cv2.imencode('.jpg', frames[-1])
            compressed_video_queue.put(jpeg.tobytes())

            # Delete extra files (simulated)
            print("Deleting extra video files...")
        time.sleep(1)