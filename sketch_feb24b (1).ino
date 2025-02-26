#include <btAudio.h>
#include <webDSP.h>

btAudio audio = btAudio("ESP_Speaker");
webDSP web;

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
}

int fo = 3;
float fc = 1000; // Default frequency

void loop() {
  // continually check on client by calling the newly added method
  web.handleClient();
  delay(300);

  if (Serial.available()) {
    String command = Serial.readStringUntil('#');
    if (command.equals("compress")) {
      Serial.println("Applying Compression");
      float thresh = 30;
      float attack = 0.1;
      float release = 0.2;
      float ratio = 10;
      float kneeWidth = 1;
      float gain = 0;
      audio.compress(thresh, attack, release, ratio, kneeWidth, gain);
    } else if (command.equals("decompress")) {
      Serial.println("Releasing Compression");
      audio.decompress();
    } else if (command.equals("gain")) {
      float gain = Serial.parseInt();
      Serial.println((String)"Compressing with a gain of " + gain);
      audio.compress(30, 0.1, 0.2, 10, 1, gain);
    } else if (command.equals("thresh")) {
      float thresh = Serial.parseFloat();
      Serial.println(thresh);
      Serial.println((String)"Compressing with a threshold of " + thresh + "dB");
      audio.compress(thresh, 0.1, 0.2, 10, 1, 0);
    } else if (command.equals("attack")) {
      float attack = Serial.parseFloat();
      Serial.println((String)"Compressing with an attack time of " + attack * 1000 + "ms");
      audio.compress(30, attack, 0.2, 10, 1, 0);
    } else if (command.equals("release")) {
      float release = Serial.parseFloat();
      Serial.println((String)"Compressing with a release time of " + release * 1000 + "ms");
      audio.compress(30, 0.1, release, 10, 1, 0);
    } else if (command.equals("ratio")) {
      float ratio = Serial.parseFloat();
      Serial.println((String)"Compressing with a Ratio of " + ratio);
      audio.compress(30, 0.1, 0.2, ratio, 1, 0);
    } else if (command.equals("width")) {
      float kneeWidth = Serial.parseInt();
      Serial.println((String)"Compressing with a knee width of " + kneeWidth + "dB");
      audio.compress(30, 0.1, 0.2, 10, kneeWidth, 0);
    } else if (command.equals("hp")) {
      Serial.println("Setting a High-Pass Filter...");
      fc = Serial.parseFloat();
      audio.createFilter(fo, fc, highpass);
    } else if (command.equals("lp")) {
      Serial.println("Setting a Low-Pass Filter...");
      fc = Serial.parseFloat();
      audio.createFilter(fo, fc, lowpass);
    } else if (command.equals("stopFilt")) {
      audio.stopFilter();
      Serial.println("Stopping Filter...");
    } else if (command.equals("order")) {
      fo = Serial.parseInt();
      Serial.println((String)"Filter order changed to " + fo + " for next filter ");
    } else {
      Serial.println("Command not recognised");
    }
  }
}