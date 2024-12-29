#define CAMERA_MODEL_XIAO_ESP32S3  // Add this line to define your camera model

#include <WiFi.h>
#include "video_recording.h"
#include "audio_recording.h"
#include "web_server_audio.h"
#include "config.h" 
#include <esp_task_wdt.h> 
#include "camera_pins.h"
#include "app_httpd.h"  // Include the app_httpd header

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    // Initialize camera
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_UXGA;
    config.pixel_format = PIXFORMAT_JPEG;  // for streaming
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    if (config.pixel_format == PIXFORMAT_JPEG) {
        if (psramFound()) {
            config.jpeg_quality = 10;
            config.fb_count = 2;
            config.grab_mode = CAMERA_GRAB_LATEST;
        } else {
            config.frame_size = FRAMESIZE_SVGA;
            config.fb_location = CAMERA_FB_IN_DRAM;
        }
    } else {
        config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
        config.fb_count = 2;
#endif
    }

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t *s = esp_camera_sensor_get();
    if (s->id.PID == OV3660_PID) {
        s->set_vflip(s, 1);        
        s->set_brightness(s, 1);   
        s->set_saturation(s, -2);  
    }
    if (config.pixel_format == PIXFORMAT_JPEG) {
        s->set_framesize(s, FRAMESIZE_QVGA);
    }

#if defined(CAMERA_MODEL_XIAO_ESP32S3)
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
#endif

#if defined(LED_GPIO_NUM)
    setupLedFlash(LED_GPIO_NUM);
#endif

    // Initialize WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.setSleep(false);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    // Start camera server
    startCameraServer();  // Call the function from app_httpd.cpp
    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");

    // Create queues
    QueueHandle_t videoQueue = xQueueCreate(10, sizeof(camera_fb_t*));
    QueueHandle_t audioQueue = xQueueCreate(10, sizeof(int16_t*));

    // Create tasks    
    xTaskCreatePinnedToCore(video_recording_task, "Video Task", 8192, videoQueue, 2, NULL, 0);
    xTaskCreatePinnedToCore(audio_recording_task, "Audio Task", 4096, audioQueue, 1, NULL, 1);
    xTaskCreatePinnedToCore(web_server_task, "Web Server Task", 8192, audioQueue, 1, NULL, 1);

    // Initialize Task Watchdog Timer
    esp_task_wdt_config_t wdt_config = {
        .timeout_ms = 10000  // 10 seconds timeout
    };
    esp_task_wdt_init(&wdt_config); // Initialize the watchdog timer with configuration
    esp_task_wdt_add(NULL); // Add the main loop task to the watchdog
}

void loop() {
    esp_task_wdt_reset(); // Reset the watchdog timer
    delay(1000); 
}