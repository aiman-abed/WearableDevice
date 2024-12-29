#ifndef VIDEO_RECORDING_H
#define VIDEO_RECORDING_H

#include <esp_camera.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

/**
 * @brief Initialize the camera with specified configuration.
 *
 * @return true if the camera was initialized successfully, false otherwise.
 */
bool init_camera();

/**
 * @brief Task to handle video recording. Captures frames from the camera and sends them to a queue.
 *
 * @param pvParameters Queue handle to send captured frames.
 */
void video_recording_task(void* pvParameters);

/**
 * @brief Capture a frame and send it to the provided queue.
 *
 * @param videoQueue Queue handle to send captured frames.
 */
void capture_and_send_frame(QueueHandle_t videoQueue);

#endif // VIDEO_RECORDING_H