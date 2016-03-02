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
#include <ESP8266WiFi.h>

class WifiCreds
{
  public:
    WifiCreds();
    WifiCreds(char *default_ssid, char *default_password);
    WifiCreds(const char *default_ssid, const char *default_password);
    
    void init_eeprom(int eepromInit, int offset);
    
    void connect();
    void connect(int indicator_pin);
    
    bool is_wifi_enabled();
    void disable_wifi();
    bool is_connected();
  private:
    void _init();
    
    static const unsigned int ssid_max_length = 33;
    static const unsigned int password_max_length = 94;

    bool _is_wifi_enabled;
    char _ssid[ssid_max_length+1];
    char _password[password_max_length+1];
    int _offset;
    bool _is_eeprom_init;
    bool _verbose;
    bool _is_connected;
    
    static const unsigned int _values_set_index = 0;
    static const unsigned int _values_set_value = 42;
    static const unsigned int _ssid_index = 1;
    static const unsigned int _pass_index = 34;

    void read_ssid(char *str); // Read ssid from EEPROM into str
    void read_password(char *str); // Read passwd from EEPROM into str
    void read_from_serial(char *str, int max_len); // read a word into a char* - stop at \n, \r, \0 or ' '
    void write_credentials(char *ssid, char *password); // Write creds
    void clear_settings(); // Clear out our ssid & pass
};

#endif