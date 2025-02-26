#include "webDSP.h"

webDSP::webDSP() : _server(80), _audio(nullptr), _hpFreq(1000), _hpAmp(1.0), _lpFreq(1000), _lpAmp(1.0) {}

void webDSP::begin(const char* ssid, const char* password, btAudio* audio) {
  _audio = audio;
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  _server.on("/", std::bind(&webDSP::handleRoot, this));
  _server.on("/hp", std::bind(&webDSP::handleHighPass, this));
  _server.on("/lp", std::bind(&webDSP::handleLowPass, this));
  _server.on("/stopFilt", std::bind(&webDSP::handleStopFilter, this));
  _server.begin();
  Serial.println("Web server started");

  _preferences.begin("webDSP", false);
  loadSettings();
}

void webDSP::handleRoot() {
  String html = "<html><head><title>Audio Filter Control</title></head><body>";
  html += "<h1>Audio Filter Control</h1>";
  html += "<div class='slider-container'>";
  html += "<label class='slider-label' for='hpFreq'>High-Pass Filter Frequency (Hz):</label>";
  html += "<input type='range' id='hpFreq' name='hpFreq' min='20' max='20000' value='" + String(_hpFreq) + "' oninput='updateLabel(\"hpFreqLabel\", this.value)'>";
  html += "<span id='hpFreqLabel'>" + String(_hpFreq) + "</span></div>";
  html += "<div class='slider-container'>";
  html += "<label class='slider-label' for='hpAmp'>High-Pass Filter Amplitude:</label>";
  html += "<input type='range' id='hpAmp' name='hpAmp' min='0.1' max='10' step='0.1' value='" + String(_hpAmp) + "' oninput='updateLabel(\"hpAmpLabel\", this.value)'>";
  html += "<span id='hpAmpLabel'>" + String(_hpAmp) + "</span></div>";
  html += "<div class='slider-container'>";
  html += "<label class='slider-label' for='lpFreq'>Low-Pass Filter Frequency (Hz):</label>";
  html += "<input type='range' id='lpFreq' name='lpFreq' min='20' max='20000' value='" + String(_lpFreq) + "' oninput='updateLabel(\"lpFreqLabel\", this.value)'>";
  html += "<span id='lpFreqLabel'>" + String(_lpFreq) + "</span></div>";
  html += "<div class='slider-container'>";
  html += "<label class='slider-label' for='lpAmp'>Low-Pass Filter Amplitude:</label>";
  html += "<input type='range' id='lpAmp' name='lpAmp' min='0.1' max='10' step='0.1' value='" + String(_lpAmp) + "' oninput='updateLabel(\"lpAmpLabel\", this.value)'>";
  html += "<span id='lpAmpLabel'>" + String(_lpAmp) + "</span></div>";
  html += "<button onclick='setHighPassFilter()'>Set High-Pass Filter</button>";
  html += "<button onclick='setLowPassFilter()'>Set Low-Pass Filter</button>";
  html += "<button onclick='stopFilter()'>Stop Filter</button>";
  html += "<script>";
  html += "function updateLabel(labelId, value) { document.getElementById(labelId).textContent = value; }";
  html += "function setHighPassFilter() { const freq = document.getElementById('hpFreq').value; const amp = document.getElementById('hpAmp').value; fetch(`/hp?freq=${freq}&amp=${amp}`); }";
  html += "function setLowPassFilter() { const freq = document.getElementById('lpFreq').value; const amp = document.getElementById('lpAmp').value; fetch(`/lp?freq=${freq}&amp=${amp}`); }";
  html += "function stopFilter() { fetch('/stopFilt'); }";
  html += "</script></body></html>";
  _server.send(200, "text/html", html);
}

void webDSP::handleHighPass() {
  if (_server.hasArg("freq") && _server.hasArg("amp")) {
    _hpFreq = _server.arg("freq").toFloat();
    _hpAmp = _server.arg("amp").toFloat();
    _audio->createFilter(3, _hpFreq, highpass);
    saveSettings();
    _server.send(200, "text/plain", "High-Pass Filter Set");
  } else {
    _server.send(400, "text/plain", "Bad Request");
  }
}

void webDSP::handleLowPass() {
  if (_server.hasArg("freq") && _server.hasArg("amp")) {
    _lpFreq = _server.arg("freq").toFloat();
    _lpAmp = _server.arg("amp").toFloat();
    _audio->createFilter(3, _lpFreq, lowpass);
    saveSettings();
    _server.send(200, "text/plain", "Low-Pass Filter Set");
  } else {
    _server.send(400, "text/plain", "Bad Request");
  }
}

void webDSP::handleStopFilter() {
  _audio->stopFilter();
  _server.send(200, "text/plain", "Filter Stopped");
}

void webDSP::handleClient() {
  _server.handleClient();
}

void webDSP::loadSettings() {
  _hpFreq = _preferences.getFloat("hpFreq", 1000);
  _hpAmp = _preferences.getFloat("hpAmp", 1.0);
  _lpFreq = _preferences.getFloat("lpFreq", 1000);
  _lpAmp = _preferences.getFloat("lpAmp", 1.0);
}

void webDSP::saveSettings() {
  _preferences.putFloat("hpFreq", _hpFreq);
  _preferences.putFloat("hpAmp", _hpAmp);
  _preferences.putFloat("lpFreq", _lpFreq);
  _preferences.putFloat("lpAmp", _lpAmp);
}