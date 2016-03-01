/**
 * WifiCreds.h
 * Mike McCracken
 * 
 * A library for managing wifi credentials on your ESP8266 project

Use it like this:

///In your sketch:

#include <WifiCreds.h>
// SSID and password - you can initialize these to defaults if you want
char ssid[WifiCreds::ssid_max_length];
char pass[WifiCreds::password_max_length];

/// Somewhere in setup():
  WifiCreds::begin(512); // This initializes the EEPROM
  WifiCreds::read_ssid(ssid); // If there's something in EEPROM, it copies it in
  WifiCreds::read_password(pass); // Copy passwd if present

  Serial.begin(115200); while(!Serial) ; // Open Serial connection and wait for it to be good
  Serial.println("");
  Serial.print("Initializing connection to "); Serial.println(ssid);
  Serial.println("Send \"<ssid> <password>\" to update credentials"); 
  Serial.println("");

  // This will try to connect to wifi, but accept a string via serial with two words: ssid & password
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, LOW); // Blink briefly to indicate you're connecting
    delay(5);
    digitalWrite(LED_PIN, HIGH);
    if (Serial.available() > 0){
      WifiCreds::read_from_serial(ssid, WifiCreds::ssid_max_length);
      WifiCreds::read_from_serial(pass, WifiCreds::password_max_length);
      Serial.println("");
      Serial.println("Got new wifi credentials.");
      Serial.print(" * SSID: "); Serial.println(ssid);
      Serial.print(" * Password: "); Serial.println(pass);
      WifiCreds::write_credentials(ssid, pass);
      WiFi.begin(ssid, pass);
    }
    delay(495);
    Serial.print(".");
  }
  Serial.println(""); 

 */

#ifndef WifiCreds_h
#define WifiCreds_h

#include "Arduino.h"
#include <EEPROM.h>

class WifiCreds
{
  public:
    static const unsigned int ssid_max_length = 33;
    static const unsigned int password_max_length = 94;
    static void begin(unsigned int eepromInit);
    // static void set_memory_position(unsigned int offset); // If we're sharing memory
    static void read_ssid(char *str); // Read ssid from EEPROM into str
    static void read_password(char *str); // Read passwd from EEPROM into str
    static void read_from_serial(char *str, int max_len); // read a word into a char* - stop at \n, \r, \0 or ' '
    static void write_credentials(char *ssid, char *password); // Write creds
    static void clear_settings(); // Clear out our ssid & pass
  private:
    static unsigned int _offset;
    static const unsigned int _values_set_index = 0;
    static const unsigned int _values_set_value = 42;
    static const unsigned int _ssid_index = 1;
    static const unsigned int _pass_index = 34;
};

#endif