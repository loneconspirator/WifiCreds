/**
 * WifiCreds.h
 * Mike McCracken
 * 
 * A library for managing wifi credentials on your ESP8266 project
 */

#include "WifiCreds.h"
#include "Arduino.h"
#include <EEPROM.h>

// This just initialized the object and the EEPROM - send 0 to not init the EEPROM
// This would mostly be used if WifiCred is the only thing using EEPROM
void WifiCreds::begin(unsigned int eepromInit) {
  // WifiCreds::_offset = 0;
  if (eepromInit != 0)
    EEPROM.begin(eepromInit);
}

// The offest for the memory block WifiCred uses (defaults to 0)
// void WifiCreds::set_memory_position(unsigned int offset) {
//   WifiCreds::_offset = offset;
// }
//
// Check if the creds are saved in EEPROM and read them if so
void WifiCreds::read_ssid(char *str) {
  if (EEPROM.read(_values_set_index) == _values_set_value)
    for (int i=0; i<ssid_max_length; i++)
      str[i] = EEPROM.read(_ssid_index + i);
}

// Check if the creds are saved in EEPROM and read them if so
void WifiCreds::read_password(char *str) {
  if (EEPROM.read(_values_set_index) == _values_set_value)
    for (int i=0; i<password_max_length; i++)
      str[i] = EEPROM.read(_pass_index + i);
}

void WifiCreds::write_credentials(char *ssid, char *password) {
  // Write the SSID into EEPROM
  for (int i=0; i<ssid_max_length; i++)
    EEPROM.write(_ssid_index + i, ssid[i]);
  // Write the password into EEPROM
  for (int i=0; i<password_max_length; i++)
    EEPROM.write(_pass_index + i, password[i]);
  // Set the flag indicating the values are set
  EEPROM.write(_values_set_index, _values_set_value);
  EEPROM.commit();
}

void WifiCreds::clear_settings() {
  EEPROM.write(_values_set_index, _values_set_value - 1);
}

void WifiCreds::read_from_serial(char *str, int max_len) {
  int i = 0;
  while(Serial.available() > 0 && i < max_len) {
    str[i] = Serial.read();
    if (str[i] == '\n' || str[i] == '\r' || str[i] == '\0' || str[i] == ' ')
      break;
    i++;
  }
  str[i] = '\0';
}