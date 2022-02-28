#pragma once

#include <ESPAsyncWebServer.h>
#include <Update.h>
#include <state/state.h>

void handleUpload(AsyncWebServerRequest *, const String &, size_t, uint8_t *, size_t, bool);
extern AsyncCallbackJsonWebHandler *network_handler;
extern AsyncCallbackJsonWebHandler *radar_handler;
extern AsyncCallbackJsonWebHandler *relay_handler;
extern AsyncCallbackJsonWebHandler *user_handler;

void startEspServer();