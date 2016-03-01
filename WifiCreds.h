/**
 * WifiCreds.h
 * Mike McCracken
 * 
 * A library for managing wifi credentials on your ESP8266 project
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