void waitForHook() {
  // read the state hook switch into a local variable:
  int hookReading = digitalRead(hookPin);
  if (hookReading != hookState) {
    // (one time action)
    lastHookState = hookReading;                  // put the value read to the current hookstate.

    if (hookReading == LOW) {                   // check state is LOW
      mp3Wake();                              // Wake up MP3 since hook is picked up and set player to SD
      playTrackInFolder(99, 4);               // Play dailtone
    }
    else {                                    // if not low but high (still one time action)
      countedPulses = 0;                     //reset counting parameters
      pulseCount = 0;
      MP3stop();                             // Stop MP3Player
      mp3Sleep();
    }
  }

  if (hookState == LOW) {                       // if Hook is LOW waitforDial or checkNumPad dependig on the version
    waitForDial();  // every loop we check if hookstate is low and wait for dial.
    //checkPlaying();
  }
  hookState = hookReading;    //store current status in lastHookState for bounceTime check
  //random(1, 40);                              // just do a lot of randoms every cycle 1 to make random real random
}

void waitForDial()
{
  random(1, 40);                              // just do a lot of randoms every cycle 1 to make random real random
  int dialReading = digitalRead(dialPin);
  if (dialReading != lastDialState) {
    // reset the debouncing timer
    lastDialDebounceTime = millis();
  }
  if ((millis() - lastDialDebounceTime) > dialDebounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    // if the button state has changed:
    if (dialReading != dialState) {
      dialState = dialReading;
      // If dialstate goes from High to Low  , Dial starts
      if (dialState == LOW) {
        playingRandom = false ;
      }
      // if dialState goes from Low to High . Dial Ends
      if (dialState == HIGH)
      {
        countedPulses = pulseCount;
        if (countedPulses <= 1 )
        {
          countedPulses = 1;
        }
        if (countedPulses >= 10 )
        {
          countedPulses = 10;
        }
        pulseCount = 0;
        addLastNumber(countedPulses);  // add last dailed number for pin array
        addLastTime(millis());         // add last time for pin
        checkAll();
        playTrackInFolder(countedPulses, folderNumber);
        //reset folder if 112
        if (countedPulses == 112)
        {folderNumber = EEPROM_getValue(6001);}  
      }
    }

    if (dialState == LOW) {
      countPulse();
    }
  }
  lastDialState = dialReading;
}

void countPulse()
{
  int pulseReading = digitalRead(pulsePin);
  if (pulseReading != lastPulseState) {
    lastPulseDebounceTime = millis();
  }
  if ((millis() - lastPulseDebounceTime) > pulseDebounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (pulseReading != pulseState) {
      pulseState = pulseReading;
      // only toggle the LED if the new button state is LOW
      if (pulseState == HIGH) {
        //ledState = LOW;
        pulseCount =  pulseCount + 1;
      }
    }
  }
  lastPulseState = pulseReading;
}
