#ifndef AUDIO_RECORDING_H
#define AUDIO_RECORDING_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

/**
 * @brief Initialize the audio I2S interface with specified configuration.
 *
 * @return true if the audio interface was initialized successfully, false otherwise.
 */
bool init_audio();

/**
 * @brief Task to handle audio recording. Reads audio data from I2S and sends it to a queue.
 *
 * @param pvParameters Queue handle to send recorded audio data.
 */
void audio_recording_task(void* pvParameters);

#endif // AUDIO_RECORDING_H