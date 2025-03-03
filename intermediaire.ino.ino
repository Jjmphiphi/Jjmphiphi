#include <btAudio.h>
#include <webDSP.h>
#include "PreferencesManager.h"

btAudio audio = btAudio("Vieille_Radio");
String command;
// create webserver object
webDSP web;

// Declare the variables
float thresh;
float attack;
float release;
float ratio;
float kneeWidth;
float gain;
int fo;
float fc;
float volume;

void setup() {
    // Initialize the LED pin
    pinMode(2, OUTPUT); // Assuming the LED is connected to GPIO 2
    digitalWrite(2, LOW); // Turn off the LED initially

    // Streams audio data to the ESP32  
    audio.begin();

    // Re-connects to last connected device
    audio.reconnect();

    // Replace ssid and password with your details
    const char* ssid = "Sables";
    const char* password = "1234567890";
    web.begin(ssid, password, &audio);

    // Outputs the received data to an I2S DAC, e.g. https://www.adafruit.com/product/3678
    int bck = 26;
    int ws = 25;
    int dout = 22;
    audio.I2S(bck, dout, ws);

    // Opens the serial port
    Serial.begin(115200);

    // Retrieve saved values or set defaults
    thresh = preferences.getFloat("thresh", 30.0);
    attack = preferences.getFloat("attack", 0.1);
    release = preferences.getFloat("release", 0.2);
    ratio = preferences.getFloat("ratio", 10.0);
    kneeWidth = preferences.getFloat("kneeWidth", 1.0);
    gain = preferences.getFloat("gain", 0.0);
    fo = preferences.getInt("fo", 3);
    volume = preferences.getFloat("volume", 0.95);

    // Initialize the audio DSP with the retrieved values
    audio.compress(thresh, attack, release, ratio, kneeWidth, gain);
    audio.volume(volume);
}

void loop() {
    // continually check on client
    web._server.handleClient();
    delay(300);

    if (Serial.available()) {
        command = Serial.readStringUntil('#');
        if (command.equals("compress")) {
            Serial.println("Applying Compression");
            audio.compress(thresh, attack, release, ratio, kneeWidth, gain);
        } else if (command.equals("decompress")) {
            Serial.println("Releasing Compression");
            audio.decompress();
        } else if (command.equals("gain")) {
            gain = Serial.parseInt();
            Serial.println((String)"Compressing with a gain of " + gain);
            audio.compress(thresh, attack, release, ratio, kneeWidth, gain);
            preferences.putFloat("gain", gain);
        } else if (command.equals("thresh")) {
            thresh = Serial.parseFloat();
            Serial.println(thresh);
            Serial.println((String)"Compressing with a threshold of " + thresh + "dB");
            audio.compress(thresh, attack, release, ratio, kneeWidth, gain);
            preferences.putFloat("thresh", thresh);
        } else if (command.equals("attack")) {
            attack = Serial.parseFloat();
            Serial.println((String)"Compressing with an attack time of " + attack * 1000 + "ms");
            audio.compress(thresh, attack, release, ratio, kneeWidth, gain);
            preferences.putFloat("attack", attack);
        } else if (command.equals("release")) {
            release = Serial.parseFloat();
            Serial.println((String)"Compressing with a release time of " + release * 1000 + "ms");
            audio.compress(thresh, attack, release, ratio, kneeWidth, gain);
            preferences.putFloat("release", release);
        } else if (command.equals("ratio")) {
            ratio = Serial.parseFloat();
            Serial.println((String)"Compressing with a Ratio of " + ratio);
            audio.compress(thresh, attack, release, ratio, kneeWidth, gain);
            preferences.putFloat("ratio", ratio);
        } else if (command.equals("width")) {
            kneeWidth = Serial.parseInt();
            Serial.println((String)"Compressing with a knee width of " + kneeWidth + "dB");
            audio.compress(thresh, attack, release, ratio, kneeWidth, gain);
            preferences.putFloat("kneeWidth", kneeWidth);
        } else if (command.equals("hp")) {
            Serial.println("Setting a High-Pass Filter...");
            fc = Serial.parseFloat();
            audio.createFilter(fo, fc, highpass);
        } else if (command.equals("stopFilt")) {
            audio.stopFilter();
            Serial.println("Stopping Filter...");
        } else if (command.equals("order")) {
            fo = Serial.parseInt();
            Serial.println((String)"Filter order changed to " + fo + " for next filter ");
            preferences.putInt("fo", fo);
        } else if (command.equals("lp")) {
            Serial.println("Setting a Low-Pass Filter...");
            fc = Serial.parseFloat();
            audio.createFilter(fo, fc, lowpass);
        } else if (command.equals("volume")) {
            volume = Serial.parseFloat();
            Serial.println((String)"Setting volume to " + volume);
            audio.volume(volume);
            preferences.putFloat("volume", volume);
        } else {
            Serial.println("Command not recognised");
        }
    }
}