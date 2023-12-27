#define DEBUG_FRONTEND
#include "Frontend_Funktions.h"

/* Funktionen
void frontendSetup();
*/

bool activateFrontend = false;
void frontendSetup(){
  if((strcmp(eepromVar1.ssid,"") != 0) && (strcmp(eepromVar1.password,"") != 0)){
    WiFi.mode(WIFI_STA);
    WiFi.begin(eepromVar1.ssid, eepromVar1.password);
    // Wait for connection
    int i = 0;
    while (WiFi.status() != WL_CONNECTED){
      delay(500);
      i++;
      if( i > 20 ){
        activateFrontend = true;
        break;
      }
    }
  }
  else{activateFrontend = true;}

  if(activateFrontend){frontendStartInAPMode( IPAddress(192,168,1,10) );}

  setupFrontend();
  frontendStart();
}
