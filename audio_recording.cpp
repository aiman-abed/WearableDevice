#include "audio_recording.h"
#include <driver/i2s.h>
#include <stdlib.h>
#include <cstring> 
#include "pins.h"
#include <Arduino.h>

#define SAMPLE_RATE 16000
#define BUFFER_SIZE 1024

bool init_audio() {
    // Configure I2S driver
    i2s_config_t i2s_config = {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 1024
    };

    // Configure I2S pins
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCK_IO,
        .ws_io_num = I2S_WS_IO,
        .data_out_num = I2S_DO_IO,
        .data_in_num = I2S_DI_IO
    };

    // Install and start I2S driver
    if (i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL) != ESP_OK) {
        return false;
    }

    // Set I2S pins
    if (i2s_set_pin(I2S_NUM_0, &pin_config) != ESP_OK) {
        return false;
    }

    return true;
}

void audio_recording_task(void* pvParameters) {
    QueueHandle_t audioQueue = (QueueHandle_t)pvParameters;
    int16_t* buffer = (int16_t*) malloc(BUFFER_SIZE * sizeof(int16_t));
    if (buffer == NULL) {
        Serial.println("Failed to allocate memory for audio buffer");
        vTaskDelete(NULL);
    }

    while (1) {
        size_t bytes_read;
        // Read audio data from I2S
        i2s_read(I2S_NUM_0, buffer, BUFFER_SIZE * sizeof(int16_t), &bytes_read, portMAX_DELAY);
        int16_t* copy = (int16_t*) malloc(BUFFER_SIZE * sizeof(int16_t));
        if (copy != NULL) {
            memcpy(copy, buffer, BUFFER_SIZE * sizeof(int16_t));
            xQueueSend(audioQueue, &copy, portMAX_DELAY);
        }
    }
    free(buffer);
}
