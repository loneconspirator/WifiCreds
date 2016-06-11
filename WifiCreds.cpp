/**
 * WifiCreds.h
 * Mike McCracken
 * 
 * A library for managing wifi credentials on your ESP8266 project. 
 * It will accept credentials via serial connection and store them in the EEPROM
 */

#include "WifiCreds.h"
#include "Arduino.h"
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <string.h>

WifiCreds::WifiCreds() {
  _init();
}

WifiCreds::WifiCreds(char *default_ssid, char *default_password){
  _init();
  for(int i=0; i<ssid_max_length; i++) {_ssid[i] = default_ssid[i];}
  _ssid[ssid_max_length] = '\0';
  for(int i=0; i<password_max_length; i++) {_password[i] = default_password[i];}
  _password[password_max_length] = '\0';
}

WifiCreds::WifiCreds(const char *default_ssid, const char *default_password){
  _init();
  for(int i=0; i<ssid_max_length; i++) {_ssid[i] = default_ssid[i];}
  _ssid[ssid_max_length] = '\0';
  for(int i=0; i<password_max_length; i++) {_password[i] = default_password[i];}
  _password[password_max_length] = '\0';
}

void WifiCreds::_init() {
  _is_wifi_enabled = true;
  _ssid[0] = '\0';
  _password[0] = '\0';
  _offset = 0;
  _is_eeprom_init = false;
  _verbose = true;
  _is_connected = false;
  strcpy (id,".  :  :  :  :  :  \0");
}

void WifiCreds::init_eeprom(int eeprom_init, int offset) {
  if (eeprom_init != 0)
    EEPROM.begin(eeprom_init);
  _offset = offset;
  _is_eeprom_init = true;
}

bool WifiCreds::is_wifi_enabled() {
  return _is_wifi_enabled;
}
void WifiCreds::disable_wifi() {
  _is_wifi_enabled = false;
}
bool WifiCreds::is_connected() {
  return _is_connected;
}

char *WifiCreds::mac_id() {return id;}

char *WifiCreds::read_variable(int position, int max_length, char *default_value){
  if (EEPROM.read(position) == _values_set_value){
    char *str = new char[max_length];
    for (int i=0; i<max_length; i++)
      str[i] = EEPROM.read(position + i + 1);
    return str;
  }else
    return default_value;
}
void WifiCreds::write_variable(int position, int max_length, char *value){
  for (int i=0; i<max_length; i++){
    EEPROM.write(position + i + 1, value[i]);
  }
  EEPROM.write(position, _values_set_value);
  EEPROM.commit();
}

// WiFi WifiCreds::get_wifi_object() {
//   if (_is_wifi_enabled)
//     return WiFi;
//   else
//     return NULL;
// }

void WifiCreds::connect() {
  connect(-1);
}

void WifiCreds::connect(int indicator_pin) {
  if (!_is_eeprom_init){
    EEPROM.begin(512);
    _is_eeprom_init = true;
  }
  read_ssid(_ssid);
  read_password(_password);
  if (_verbose) {
    Serial.println("");
    Serial.print("Initializing connection to "); Serial.println(_ssid);
    Serial.println("Send \"ssid password\" to update credentials"); 
    Serial.println("");
  }
  
  WiFi.begin(_ssid, _password);
  
  while (_is_wifi_enabled && WiFi.status() != WL_CONNECTED) {
    if(indicator_pin >= 0) digitalWrite(indicator_pin, LOW);
    delay(5);
    if(indicator_pin >= 0) digitalWrite(indicator_pin, HIGH);
    if (Serial.available() > 0){
      read_from_serial(_ssid, WifiCreds::ssid_max_length);
      read_from_serial(_password, WifiCreds::password_max_length);
      if (_verbose) {
        Serial.println("");
        Serial.println("Got new wifi credentials.");
        Serial.print(" - SSID: "); Serial.println(_ssid);
        Serial.print(" - Password: "); Serial.println(_password);
      }
      WifiCreds::write_credentials(_ssid, _password);
      WiFi.begin(_ssid, _password);
    }
    delay(495);
    if (_verbose) Serial.print(".");
  }
  if (_verbose) {
    Serial.println(""); 
    if (_is_wifi_enabled) {
      Serial.println("WiFi connected");
      Serial.print("IP address: "); Serial.println(WiFi.localIP());
      WiFi.macAddress(mac);
      writeHex(mac[5], id, 1, 2);
      writeHex(mac[4], id, 4, 5);
      writeHex(mac[3], id, 7, 8);
      writeHex(mac[2], id, 10, 11);
      writeHex(mac[1], id, 13, 14);
      writeHex(mac[0], id, 16, 17);
      Serial.print("MAC address: "); Serial.println(id);
    } else {
      Serial.println("Skipping wifi setup");
    }
  }
  if (_is_wifi_enabled) _is_connected = true;;
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
      str[i] = EEPROM.read(_ssid_index + i + _offset);
}

// Check if the creds are saved in EEPROM and read them if so
void WifiCreds::read_password(char *str) {
  if (EEPROM.read(_values_set_index) == _values_set_value)
    for (int i=0; i<password_max_length; i++)
      str[i] = EEPROM.read(_pass_index + i + _offset);
}

void WifiCreds::write_credentials(char *ssid, char *password) {
  // Write the SSID into EEPROM
  for (int i=0; i<ssid_max_length; i++)
    EEPROM.write(_ssid_index + i + _offset, ssid[i]);
  // Write the password into EEPROM
  for (int i=0; i<password_max_length; i++)
    EEPROM.write(_pass_index + i + _offset, password[i]);
  // Set the flag indicating the values are set
  EEPROM.write(_values_set_index + _offset, _values_set_value);
  EEPROM.commit();
}

void WifiCreds::clear_settings() {
  EEPROM.write(_values_set_index + _offset, _values_set_value - 1);
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

// Just some methods to help get a nice mac address string
char WifiCreds::hexChar(byte val) {
  if (val < 10)
    return val + '0';
  else
    return (val - 10) + 'A';
}

void WifiCreds::writeHex(byte val, char *str, int pos1, int pos2) {
  byte left = val / 16;
  byte right = val - (left * 16);
  str[pos1] = hexChar(left);
  str[pos2] = hexChar(right);
}

