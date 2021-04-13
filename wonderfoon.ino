/*
  Wonderfoon Tiny85 0801T
  // 0801 amplifier switching removed 
  //      Set pin PB5 to high to prevent reset
  //      added 112 alarm number message.
  //      make Random() realy random by adding random every cycle.9
  
  http://www.wonderfoon.eu
  ATTINY85 version
*/

#include <EEPROM.h>
#include <SoftSerial.h>     /* Allows Pin Change Interrupt Vector Sharing */

SoftSerial mySerial(6, 4); //RX, TX
// Variables for Dialdisc Hook and Amplifier

const int hookPin   = 2;     // the in for the telephone hook                 D6 12
const int dialPin   = 3;     //  the in for the telephone dialpulse (yellow)  D5 14
const int pulsePin  = 0;     // the in for the telephone dialbusy (green)      D7 13
const int resetPin  = 5;     // define agains unexpected behaviour
const int busyPin   = 1;     // when player is playing this pim is 1
unsigned long lastDialDebounceTime = 0;   // the last time the dial pin was
unsigned long dialDebounceDelay = 50;     // the debounce time;
unsigned long lastPulseDebounceTime = 0;  // the last time the pulse pin was
unsigned long pulseDebounceDelay = 15;    // the debounce time;

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
  EEPROM_init(false);     // initialize to check if this is the first time the Wonderfoon is started addess 100 = 77

  audioVolume = EEPROM_getValue(6000);
  folderNumber = EEPROM_getValue(6001);
  playMode = EEPROM_getValue(6002);
  
  pinMode(hookPin, INPUT_PULLUP);                 //Set pins to input and add pullup resitor
  pinMode(dialPin, INPUT_PULLUP);
  pinMode(pulsePin, INPUT_PULLUP);
  pinMode(busyPin, INPUT_PULLUP);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, 1);
  //randomSeed(analogRead(A0));
  
  mp3Wake();
  delay(200);
  playTrackInFolder(10, 4);                               // Wonderfoon has started
  delay(2000);
  playVolume();                                           // play vulume status
  playFolder(folderNumber);                               // play folder number status
  playWillekeurig(playMode);
  mp3Sleep();
  //Wonderfoon ready for use            
}

void loop() {
  waitForHook();
  checkPlaying();
}
