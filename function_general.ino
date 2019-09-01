void checkPlaying()
{
  if (digitalRead(busyPin) == 1)
  {
    if (!playingRandom) {
      
     // if (amplifierstate){
     // delay(1000);
     // amplifier(0);
     // }
    }
    else
    {
      delay(1000);
      //amplifier(1);
      playTrackInFolder(1, 1);
    }
  }
}

void clearLastDialed()
{
  lastDialed[0] = 20;
  lastDialed[1] = 21;
  lastDialed[2] = 22;
}
//other common functions


void addLastNumber(int newNumber) {

  lastDialed[0] = lastDialed[1];
  lastDialed[1] = lastDialed[2];
  lastDialed[2] = newNumber;
}

void addLastTime(long newTime) {
  lastTime[0] = lastTime[1];
  lastTime[1] = lastTime[2];
  lastTime[2] = newTime;
}


void checkAll()
{

  switch (lastDialed[0])
  {
   case 1:
    //112
    if (lastDialed[1] == 1 && checkChangeTime())
      {

        if ( lastDialed[2] = 2 )
        {
          playAlarm();
        }
        clearLastDialed();
      }
      break;  
    case 2:

      if (lastDialed[1] == 1 && checkChangeTime())
      {
        EEPROM_storeVolume(lastDialed[2]);
        setMP3Volume(lastDialed[2]);
        playVolume();
        clearLastDialed();
      }
      break;

    case 3:

      if (lastDialed[1] == 1 && checkChangeTime())
      {

        if ( lastDialed[2] < 4 )
        {
          EEPROM_storeFolder(lastDialed[2]);
          playFolder(lastDialed[2]);
          folderNumber = lastDialed[2];
        }
        clearLastDialed();
      }
      break;

    case 4:

      if (lastDialed[1] == 1 && checkChangeTime())
      {
        //willekeurig
        if ( lastDialed[2] < 3 )
        {
          EEPROM_storePlayMode(lastDialed[2] - 1);
          playWillekeurig(lastDialed[2] - 1);
          playMode = lastDialed[2] - 1;
        }
        clearLastDialed();
      }
      break;


    case 5:

      if (lastDialed[1] == 1 && checkChangeTime())
      {
        //willekeurig
        if ( lastDialed[2] < 2 )
        {
          playingRandom = true;
          playContinu();
        }
        clearLastDialed();
      }
      break;


    case 9:

      if (lastDialed[1] == 9 && checkChangeTime())
      {
        //willekeurig
        if ( lastDialed[2] == 9 )
        {
          playReset();
          EEPROM_init(true);
          audioVolume = EEPROM_getValue(6000);
          folderNumber = EEPROM_getValue(6001);
          playMode = EEPROM_getValue(6002);
          mp3Wake();
          playVolume();                                           // play vulume status
          playFolder(folderNumber);                               // play folder number status
          playWillekeurig(playMode);
        }
        clearLastDialed();
      }
      break;

  }
}

bool checkChangeTime() {
  if (lastTime[2] - lastTime[0] < 10000)
    return true;
  else
    return false;
}
