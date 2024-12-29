#ifndef WEB_SERVER_AUDIO_H
#define WEB_SERVER_AUDIO_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <esp_http_server.h>

// HTTP GET handler for audio
esp_err_t audio_get_handler(httpd_req_t *req);

// Function prototypes
void web_server_task(void* pvParameters);

#endif // WEB_SERVER_AUDIO_H