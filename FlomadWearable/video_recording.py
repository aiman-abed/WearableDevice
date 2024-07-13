import time
import cv2


def video_recording_task(video_queue):
    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
    cap.set(cv2.CAP_PROP_FPS, 30)

    while True:
        ret, frame = cap.read()
        if ret:
            video_queue.put(frame)
        time.sleep(1 / 30)  # Approx 30 fps