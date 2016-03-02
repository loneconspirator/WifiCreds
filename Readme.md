# EEPROM Wifi Credential Sotrage for ESP8266
This simple library will connect the ESP8266 to a wifi network for you and while it's attempting connection, accept new credentials via the serial interface.

## Usage:

If you are not using EEPROM for anything else, the simplest method is as follows:

In your sketch:

 * Include the library 
 `#include <WifiCreds.h>`

 * declare the object, optionally providing default credentials
`WifiCreds wifi()`
or
`WifiCreds wifi("ssid", "password")`

Then, in your setup() function:

 * Open your Serial connection (must be done before connecting)

 * Connect to wifi
`wifi.connect()`
or, send an LED pin for output that you're trying to connect
`wifi.connect(LED_PIN)`

To send in new credentials, just send a string in on the Serial connection with the ssid and password separated by a space _while it's trying to connect_

After this, the WiFi global object will be set up and available.

## Skipping Wifi
If you want to have a button input that will cancel the attempt to connect, that can be achieved with an interrupt that calls `wifi.disable_wifi()`

In your sketch:
```
void button() {
  if (wifi.is_wifi_enabled() && !wifi.is_connected())
  wifi.disable_wifi();
}
```
and in setup():
```
pinMode(BUTTON_PIN, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button, FALLING);
```

## EEPROM configuration
If you need to use the EEPROM for other things, before connect(), you can call `init_eeprom(eepromInit, offset)`.

*eepromInit* will be the value sent to EEPROM.begin (send 0 if you initialize the EEPROM on your own)
*offset* is how many bytes to offest our use of the eeprom. This is 0 by default. WifiCreds takes 128 bytes of EEPROM