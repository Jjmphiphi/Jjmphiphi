#ifndef WEBDSP_H
#define WEBDSP_H

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <btAudio.h>

class webDSP {
public:
  webDSP();
  void begin(const char* ssid, const char* password, btAudio* audio);
  void handleRoot();
  void handleHighPass();
  void handleLowPass();
  void handleStopFilter();
  void handleClient();
  void loadSettings();
  void saveSettings();

private:
  WebServer _server;
  btAudio* _audio;
  Preferences _preferences;
  float _hpFreq;
  float _hpAmp;
  float _lpFreq;
  float _lpAmp;
};

#endif // WEBDSP_H