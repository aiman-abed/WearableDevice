#include "video_recording.h"
#include "pins.h"
#include <esp_log.h>
#include <esp_heap_caps.h> 
#include <Arduino.h>       

static const char* TAG = "VideoRecording";

bool init_camera() {
    camera_config_t config = {
        .pin_pwdn = PWDN_GPIO_NUM,
        .pin_reset = RESET_GPIO_NUM,
        .pin_xclk = XCLK_GPIO_NUM,
        .pin_sscb_sda = SIOD_GPIO_NUM,
        .pin_sscb_scl = SIOC_GPIO_NUM,
        .pin_d7 = Y9_GPIO_NUM,
        .pin_d6 = Y8_GPIO_NUM,
        .pin_d5 = Y7_GPIO_NUM,
        .pin_d4 = Y6_GPIO_NUM,
        .pin_d3 = Y5_GPIO_NUM,
        .pin_d2 = Y4_GPIO_NUM,
        .pin_d1 = Y3_GPIO_NUM,
        .pin_d0 = Y2_GPIO_NUM,
        .pin_vsync = VSYNC_GPIO_NUM,
        .pin_href = HREF_GPIO_NUM,
        .pin_pclk = PCLK_GPIO_NUM,
        .xclk_freq_hz = 20000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = (ESP.getPsramSize() > 0) ? FRAMESIZE_UXGA : FRAMESIZE_SVGA,
        .jpeg_quality = (ESP.getPsramSize() > 0) ? 10 : 12,
        .fb_count = (ESP.getPsramSize() > 0) ? 2 : 1
    };

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed with error 0x%x", err);
        return false;
    }
    return true;
}

void video_recording_task(void* pvParameters) {
    QueueHandle_t videoQueue = (QueueHandle_t)pvParameters;
    while (1) {
        capture_and_send_frame(videoQueue);
        vTaskDelay(30 / portTICK_PERIOD_MS);
    }
}

void capture_and_send_frame(QueueHandle_t videoQueue) {
    camera_fb_t* frame = esp_camera_fb_get();
    if (frame) {
        xQueueSend(videoQueue, &frame, portMAX_DELAY);
    }
}