// web_server_audio.cpp
#include "web_server_audio.h"
#include "audio_recording.h" 
#include <esp_http_server.h>
#include "pins.h"

#define BUFFER_SIZE 1024 // Ensure BUFFER_SIZE is defined

void web_server_task(void* pvParameters) {
    QueueHandle_t audioQueue = (QueueHandle_t)pvParameters;

    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t audio_uri = {
            .uri = "/audio",
            .method = HTTP_GET,
            .handler = audio_get_handler,
            .user_ctx = audioQueue
        };
        httpd_register_uri_handler(server, &audio_uri);
    }

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

esp_err_t audio_get_handler(httpd_req_t *req) {
    QueueHandle_t audioQueue = (QueueHandle_t) req->user_ctx;

    // Check if there's audio data in the queue
    if (uxQueueMessagesWaiting(audioQueue) == 0) {
        httpd_resp_send(req, "No audio data available", HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }

    // Retrieve audio data from the queue
    void *audio_data;
    if (xQueueReceive(audioQueue, &audio_data, portMAX_DELAY) == pdTRUE) {
        // Send audio data as the HTTP response
        httpd_resp_send(req, (const char *)audio_data, BUFFER_SIZE * sizeof(int16_t));
        free(audio_data); // Free memory after sending
        return ESP_OK;
    } else {
        httpd_resp_send_500(req); // Send an internal server error response
        return ESP_FAIL;
    }
}