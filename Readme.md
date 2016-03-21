# EEPROM Wifi Credential Sotrage for ESP8266
This simple library will connect the ESP8266 to a wifi network for you and while it's attempting connection, accept new credentials via the serial interface.

## Usage:

If you are not using EEPROM for anything else, the simplest method is as follows:

Download the library:

 * Clone the project to your libraries folder (example for default OSX setup)
 ```
 cd ~/Documents/Arduino/libraries
 git clone https://github.com/loneconspirator/WifiCreds.git
 ```

In your sketch (example starting sketch file below):

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

# Quickstart
Here's an example file you can start with once the library is installed (I haven't tested this so there might be a glitch to two)
```
/**
 * ESP8266 IoT program
 * 
 */


#include <WifiCreds.h>
#include "espconn.h"

#include <ESP8266WiFi.h>
#include <WiFi.h>

#include <EEPROM.h>

WifiCreds wifi("default_ssid", "default_password");

// This is the pin of the blue LED on the huzzah which is for some reason backwards
#define IND_PIN 2 // output pin with LED indicating Wifi is connecting
#define BUTTON_PIN 2 // button that when connected to ground skips connecting to internet

void button() {
  if (wifi.is_wifi_enabled() && !wifi.is_connected())
    wifi.disable_wifi();
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize pins
  pinMode(IND_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button, FALLING);

  Serial.begin(115200); while(!Serial) ; // Open Serial connection and wait for it to be good

  wifi.connect(IND_PIN);

  if (wifi.is_wifi_enabled()) {
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  }
}
```