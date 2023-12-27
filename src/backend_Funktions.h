/* Funktionen
uint16_t Akkustand_ProzentF();
bool Akku_Laden_LED(uint16_t Akkustand_mal_zehn,uint16_t Prozentschwelle_mal_zehn);
bool Anzeige_Aktualisieren(float DAC1_Voltage,float DAC2_Voltage);
void EEPROM_Init();
void LED_Backlight_Aktualisieren(bool LED_zustand);
bool EEPROM_Write();
void sleep();
*/

//set EEPROM Content
boolean EEPROM_Write(){
  EEPROM.put(0, eepromVar1);
  boolean ok = EEPROM.commit();
  //Serial.println((ok) ? "Commit OK" : "Commit failed");
  return (ok);
}

//backlight led control
void LED_Backlight_Aktualisieren(bool LED_zustand){
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  digitalWrite(15,LED_zustand);
  digitalWrite(14,HIGH);
  delay(1);
  digitalWrite(14,LOW);
  pinMode(15, INPUT);
}

//return the Akku charge level in % * 10
uint16_t Akkustand_ProzentF(){
  pinMode(A0,INPUT);
  return(map((analogRead(A0)),2616,3300,0,1000));//3300 = 100%, 2616 = 0%
}


//if akku charge level is less than the Setpoint aktivate, Output the status of the output
bool Akku_Laden_LED(uint16_t Akkustand_mal_zehn,uint16_t Prozentschwelle_mal_zehn){ 
  pinMode(12,OUTPUT);
  if(Akkustand_mal_zehn < Prozentschwelle_mal_zehn){
    digitalWrite(12, HIGH);
    return(true);
  }
  else{
    digitalWrite(12, LOW);
    return(false);
  }
}


//output the analog value, return true if it sucsess input is a float 0 to 5 (Volt)
#define DAC_REF_VOLTAGE 5.000  //dac supply-reference voltage
MCP4725 dac1(MCP4725A1_ADDR_A00, DAC_REF_VOLTAGE); //init DAC 1
MCP4725 dac2(MCP4725A1_ADDR_A01, DAC_REF_VOLTAGE); //init DAC 2
bool Anzeige_Aktualisieren(float DAC1_Voltage,float DAC2_Voltage){ //true = ok, false = error //input = 0V -> 5V
  //clamp to valid outputs
  if(DAC1_Voltage < 0){DAC1_Voltage = 0;}
  if(DAC1_Voltage > 5){DAC1_Voltage = 5;}
  if(DAC2_Voltage < 0){DAC2_Voltage = 0;}
  if(DAC2_Voltage > 5){DAC2_Voltage = 5;}
  //output the values
  if(dac1.begin() == true){if (dac1.setVoltage(DAC1_Voltage, MCP4725_EEPROM_MODE, MCP4725_POWER_DOWN_OFF) != true){return(false);}} //Collision on i2c bus));}
  if(dac2.begin() == true){if (dac2.setVoltage(DAC2_Voltage, MCP4725_EEPROM_MODE, MCP4725_POWER_DOWN_OFF) != true){return(false);}} //Collision on i2c bus));}
  return(true);
}


//eeprom initialisieren
void EEPROM_Init(){
  // Set up the initial (default) values for what is to be stored in EEPROM
  eepromVar1.Sleeptime_uS = 30000000;
  eepromVar1.esp_eingerichtet = false;
  
  EEPROM.begin(sizeof(MyEEPROMStruct));//initialisiere EEPROM
  // Check if the EEPROM contains valid data from another run
  // If so, overwrite the 'default' values set up in our struct
  if(EEPROM.percentUsed()>=0){
    EEPROM.get(0, eepromVar1);
  }
}

//activate deep sleep
void sleep(){
  ESP.deepSleep(eepromVar1.Sleeptime_uS);
  yield();
}
