#include <webDSP.h>

webDSP::webDSP() {}

void webDSP::begin(const char* ssid, const char* password, btAudio* audio) {
    WiFi.mode(WIFI_STA);
    while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(1000);
        Serial.print(".");
    }
    _audio = audio;
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    _server.on("/", [this]() { this->handleRoot(); });
    _server.on("/workStyle.css", [this]() { this->handleCss(); });
    _server.on("/get", [this]() { this->handleGet(); });
    _server.begin();
}

void webDSP::handleRoot() {
    _server.send(200, "text/html", html2);
}

void webDSP::handleCss() {
    _server.send_P(200, "text/css", css2);
}

void webDSP::handleGet() {
    String inputName = _server.argName(0);
    String inputVal = _server.arg(0);

    _server.send_P(200, "text/html", html2);
    Serial.println(inputName);

    if (inputName == "volume") {
        float volume = inputVal.toFloat();
        _audio->volume(volume);
        preferences.begin("btAudio", false);
        preferences.putFloat("volume", volume);
        preferences.end();
    } else if (inputName == "hpf") {
        float hpf = inputVal.toFloat();
        _audio->createFilter(3, hpf, highpass);
        preferences.begin("btAudio", false);
        preferences.putFloat("hpf", hpf);
        preferences.end();
    } else if (inputName == "lpf") {
        float lpf = inputVal.toFloat();
        _audio->createFilter(3, lpf, lowpass);
        preferences.begin("btAudio", false);
        preferences.putFloat("lpf", lpf);
        preferences.end();
    } else if (inputName == "thresh") {
        _audio->_T = inputVal.toFloat();
        _audio->compress(_audio->_T, _audio->_alphAtt, _audio->_alphRel, _audio->_R, _audio->_w, _audio->_mu);
        preferences.begin("btAudio", false);
        preferences.putFloat("thresh", _audio->_T);
        preferences.end();
    } else if (inputName == "attack") {
        _audio->_alphAtt = inputVal.toFloat();
        _audio->compress(_audio->_T, _audio->_alphAtt, _audio->_alphRel, _audio->_R, _audio->_w, _audio->_mu);
        preferences.begin("btAudio", false);
        preferences.putFloat("attack", _audio->_alphAtt);
        preferences.end();
    } else if (inputName == "release") {
        _audio->_alphRel = inputVal.toFloat();
        _audio->compress(_audio->_T, _audio->_alphAtt, _audio->_alphRel, _audio->_R, _audio->_w, _audio->_mu);
        preferences.begin("btAudio", false);
        preferences.putFloat("release", _audio->_alphRel);
        preferences.end();
    } else if (inputName == "ratio") {
        _audio->_R = inputVal.toFloat();
        _audio->compress(_audio->_T, _audio->_alphAtt, _audio->_alphRel, _audio->_R, _audio->_w, _audio->_mu);
        preferences.begin("btAudio", false);
        preferences.putFloat("ratio", _audio->_R);
        preferences.end();
    } else if (inputName == "kneeWidth") {
        _audio->_w = inputVal.toFloat();
        _audio->compress(_audio->_T, _audio->_alphAtt, _audio->_alphRel, _audio->_R, _audio->_w, _audio->_mu);
        preferences.begin("btAudio", false);
        preferences.putFloat("kneeWidth", _audio->_w);
        preferences.end();
    } else if (inputName == "gain") {
        _audio->_mu = inputVal.toFloat();
        _audio->compress(_audio->_T, _audio->_alphAtt, _audio->_alphRel, _audio->_R, _audio->_w, _audio->_mu);
        preferences.begin("btAudio", false);
        preferences.putFloat("gain", _audio->_mu);
        preferences.end();
    } else if (inputName == "stopFilt") {
        _audio->stopFilter();
    }
}