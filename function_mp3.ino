
//void amplifier(bool onoff) {
//     amplifierstate = onoff;
   //  digitalWrite(ampPin, 0);
     
     //digitalWrite(ampPin, !onoff);
//}

void setMP3Volume(int volume)
{
  
  execute_CMD(0x06, 0, volume+9); // Set the volume (0x00~0x30)
  
  //execute_CMD(0x10, 1, 31); // Set the volume (0x00~0x30)
}

void mp3Wake()
{
  execute_CMD(0x09, 0, 2); // Set Playmode to SD
  delay(500);
//  amplifier(1);                           // switch on the amplifier
  setMP3Volume(audioVolume);
}

void mp3Sleep()
{
  execute_CMD(0x09, 0, 3); // Set MP3 player in sleep mode
  execute_CMD(0x0A, 0, 0); // Set MP3 player in power loss
//  amplifier(0);
}

void playTrackInFolder(int track, int folder)
{
//  amplifier(1);   
  if ((!playMode && !playingRandom) || folder == 4) // if not Playmode = random or playinRandom is true
  execute_CMD(0x0F, folder, track);
  else 
  execute_CMD(0x0F, random(1, 4), random(1, 11));
}

void MP3stop() 
{
  execute_CMD(0x16, 0, 0);
  mp3Sleep();                            // Put MP3 is sleep mode since hook is down
//  amplifier(0);                          // amplfier of  CHECK THIS
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
  // Calculate the checksum (2 bytes)
  word checksum = -(0xFF + 0x06 + CMD + 0x00 + Par1 + Par2);
  // Build the command line
  byte Command_line[10] = { 0x7E, 0xFF, 0x06, CMD, 0x00,
                            Par1, Par2, highByte(checksum), lowByte(checksum), 0xEF
                          };
  //Send the command line to the module
  for (byte k = 0; k < 10; k++)
  {
    mySerial.write( Command_line[k]);
  }
  delay(500);
}

void playFolder(int folder) {
  playTrackInFolder(folder + 10, 4);
  delay(2000);
}
void playAlarm() {
  setMP3Volume(10);
  playTrackInFolder(112, 4);
  delay(14000);
  setMP3Volume(audioVolume);
  countedPulses = 112;
  folderNumber = 4;
}
void playVolume() {
    playTrackInFolder(audioVolume + 20, 4);
  delay(2000);
  
}

void playWillekeurig(int pm) {
  
  playTrackInFolder(pm + 3, 4);
  delay(2000);
}

void playContinu(){
playTrackInFolder(5,4);
  delay(2000);
}
void playReset(){
playTrackInFolder(6,4);
  delay(2000);
}
