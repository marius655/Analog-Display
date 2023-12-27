//Frontend
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPUpdateServer.h>
#include "html.h"

//Backend
#include <Wire.h> //I2C
#include <MCP4725.h> //Analog Converter
#include <ESP_EEPROM.h> //EEPROM

struct MyEEPROMStruct {       //EEPROM Static
  uint32_t Sleeptime_uS; //sleeptime in uS stadart 30 sek //Merker der Deep Sleep Zeit
  bool esp_eingerichtet; //Merker ob server eingerichtet
  char ssid[64]; //W-LAN SSID max 64 Zeichen
  char password[64]; //W-LAN Passwort max 64 Zeichen
} eepromVar1;
//Flash dynamisch
bool LED_status; //LED BEleuchtung
float DAC1_Volt; //Wert der ersten Anzeige
float DAC2_Volt; //Wert der Zweiten Anzeige
uint16_t Akkustand_Prozent; //ermittelter akkustand in Prozent * 10
#include "Frontend_Runtime.h"
#include "Backend_Runtime.h"
