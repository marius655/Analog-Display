#include "backend_Funktions.h"

/* Funktionen
void backend()
*/
 

void backend(){
  EEPROM_Init();
  pinMode(13, INPUT); //activate reset taster
  if(digitalRead(13) == true){eepromVar1.esp_eingerichtet == false;} //if activatet set website to active  
  
  if(eepromVar1.esp_eingerichtet == false){//when website requestet on aktivation or first start
    frontendSetup(); //init Frontend
    //WEBSEITE HIER Starten und HANDLER eintragen !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //am ende handler deaktivieren!!!!!!!!!!!!!!!!
    //warten bis eingabe gültig und fertig
    while(eepromVar1.esp_eingerichtet == false){
      frontendHandleClient();
    }
    EEPROM_Write();//save new data in strukt
  }
  else{ //normal operation
    //W-Lan Starten
    WiFi.mode(WIFI_STA);
    WiFi.begin(eepromVar1.ssid, eepromVar1.password);
    //while connecting make all the other offline tasks
    Akkustand_Prozent = Akkustand_ProzentF();
    Akku_Laden_LED(Akkustand_Prozent,100); //if less than 10 % aktivate charge indicator 
    while (WiFi.status() != WL_CONNECTED) {//wait until connection is valid
      delay(100);//im crying, we are burning Runtime and charge
    }
    //hier I/O Übermitteln!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    LED_Backlight_Aktualisieren(LED_status);
    Anzeige_Aktualisieren(DAC1_Volt,DAC2_Volt);
    sleep();
  }
}
