#include <WiFi.h>
#include <WebServer.h>
#include <esp_camera.h>
#include "web_server_camera.h"

WebServer server(80);

void handleRoot() {
    server.send(200, "text/plain", "Hello, world!");
}

void handleJpgStream() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        server.send(500, "text/plain", "Camera capture failed");
        return;
    }
    // Set the content length and content type
    server.setContentLength(fb->len);
    server.send(200, "image/jpeg", "");
    server.sendContent(reinterpret_cast<const char*>(fb->buf), fb->len);
    esp_camera_fb_return(fb);
}

void startWebCameraServer() {  // Renamed from startCameraServer
    server.on("/", handleRoot);
    server.on("/jpg", HTTP_GET, handleJpgStream);

    server.begin();
    Serial.println("HTTP server started");
}