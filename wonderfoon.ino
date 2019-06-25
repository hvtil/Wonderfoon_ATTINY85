/*
  Wonderfoon Tiny85 0501T
  http://wonderfoon.nl
  ATTINY85 version
*/
#include <EEPROM.h>
#include <SoftSerial.h>     /* Allows Pin Change Interrupt Vector Sharing */

SoftSerial mySerial(6, 4); //RX, TX
// Variables for Dialdisc Hook and Amplifier

const int hookPin   = 2;     // the in for the telephone hook                 D6 12
const int dialPin   = 3;    //  the in for the telephone dialpulse (yellow)  D5 14
const int pulsePin  = 0;    // the in for the telephone dialbusy (green)      D7 13
const int ampPin    = 1;    // to turn amplifier on / off                   D2 15
const int busyPin    = 5;   // when player is playing this pim is 1
unsigned long lastDialDebounceTime = 0;   // the last time the dial pin was
unsigned long dialDebounceDelay = 50;     // the debounce time;
unsigned long lastPulseDebounceTime = 0;  // the last time the pulse pin was
unsigned long pulseDebounceDelay = 20;    // the debounce time;

//int num;      // dialpad number
//int lastNum;  //

//boolean isPlaying = false;  // mp3
int countedPulses;          // then number of pulses counted at the end of dialbusy
int pulseCount;             // number of pulses during counting
int audioVolume;            // Audio Volume
int hookState = HIGH;       // the current state of the hook
int dialState = HIGH;       // the current state of the dailbusy
int pulseState = HIGH;      // the current state of the pulse
int lastHookState = HIGH;   // the previous reading from the hook
int lastDialState = HIGH;   // the previous reading from dailbusy
int lastPulseState = HIGH;  // the previous reading from the pulse
bool amplifierstate = false;
int lastDialed[3] = {20, 21, 22};   // last 4 dialed digits for PIN
int folderNumber;                       // current folderNumber
int playMode;                           // current playmode

bool playingRandom = false ;             // random playing mode from app or keypad  *

unsigned long lastTime[3];                // time last for digits were dialed to determine pin
void setup() {
  
  Serial.begin(9600);                             // start serial for debug and mp3
  mySerial.begin(9600);
  //TinyWireM.begin();
  EEPROM_init(false);     // initialize to check if this is the first time the Wonderfoon is started addess 100 = 77

  audioVolume = EEPROM_getValue(6000);
  folderNumber = EEPROM_getValue(6001);
  playMode = EEPROM_getValue(6002);
  
                       // 2 debuglines that will always be displayed in logging.
  pinMode(hookPin, INPUT_PULLUP);                 //Set pins to input and add pullup resitor
  pinMode(dialPin, INPUT_PULLUP);
  pinMode(pulsePin, INPUT_PULLUP);
  pinMode(busyPin, INPUT_PULLUP);
  pinMode(ampPin, OUTPUT);                        //Set Aplifier pin to output

  mp3Wake();
  delay(200);
  //delay(1000);// set volume to read value ; 00 - 30
  playTrackInFolder(10, 4);                               // Wonderfoon has started
  delay(2000);
  //put mp3 in sleep mode to save battery
  playVolume();                                           // play vulume status
  playFolder(folderNumber);                               // play folder number status
  playWillekeurig(playMode);
  mp3Sleep();            
}

void loop() {
  waitForHook();
  checkPlaying();

}
