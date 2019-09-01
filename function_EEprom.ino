 

void EEPROM_init(bool force) {
 
  byte val = EEPROM.read(6005);          // initialize first boot, and write 77 in adress 5 to make sure we know next time the phone starts it has already be started before.
  int initRead = val;
  if (initRead != 77 || force)
  { // if started the first time
 
    EEPROM.begin();
    EEPROM.write(6000,5);                // volume set to 10
    delay(500);
    EEPROM.write(6001, 1);                 //  folder set to 1
    delay(500);
    EEPROM.write(6002, 0);                 //  playrandom set to 1
    delay(500);
    
    EEPROM.write(6005, 77);              // set initialized to 77
    delay(500);
    EEPROM.end();
  }
    
}

void EEPROM_clear()
{
  EEPROM.begin();
  EEPROM.write(6000,254);
  EEPROM.write(6001,254);
  EEPROM.write(6002,254);
  EEPROM.write(6005,254);
  EEPROM.end();
}

void EEPROM_storeVolume(int volume) {  // store volume setting is EEPROM address 0
  audioVolume=volume;
  EEPROM.begin();
  EEPROM.write(6000, volume);
  EEPROM.end();
}

void EEPROM_storeFolder(int folder) {   //store music folder selection setting is EEPROM address 1
  
  EEPROM.begin(); 
  EEPROM.write(6001, folder);
  folderNumber = folder;
  EEPROM.end();
}

void EEPROM_storePlayMode(int pmode) {     //store random mode setting is EEPROM address 2
  
  EEPROM.begin();
  EEPROM.write(6002, pmode);
  playMode = pmode;
  EEPROM.end();
}


int EEPROM_getValue( int EEADRESS) {
  EEPROM.begin();
  return(EEPROM.read(EEADRESS));
  EEPROM.end();
}
