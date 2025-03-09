#ifndef BTAUDIO_H
#define BTAUDIO_H

#include <Arduino.h>
#include <BluetoothSerial.h>

class btAudio {
public:
  btAudio(const char* name);
  void begin();
  void reconnect();
  void I2S(int bck, int dout, int ws);
  void createFilter(int order, float freq, int type);
  void stopFilter();
  void compress(float thresh, float attack, float release, float ratio, float kneeWidth, float gain);
  void decompress();
  void setVolume(int volume); // Add this line

private:
  BluetoothSerial _bt;
  const char* _name;
  int _volume; // Add this line
};

#endif // BTAUDIO_H