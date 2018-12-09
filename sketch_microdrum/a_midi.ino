//==============================
//    MIDI
//==============================
void noteOn(byte channel, byte note, byte velocity) {
    Serial.write(0x90 | channel);
    Serial.write(note);
    Serial.write(velocity);
}

void noteOff(byte channel, byte note, byte velocity) {
    Serial.write(0x80 | channel);
    Serial.write(note);
    Serial.write(velocity);
}

void midiCC(byte channel, byte number, byte value) {
    Serial.write((0xB0 | channel));//Ottimizzazione
    Serial.write(number);
    Serial.write(value);
}
//=============SYSEX=========
void simpleSysex(byte cmd,byte data1,byte data2,byte data3)
{
  #if USE_LCD
  if(cmd==0x6F) DrawDiagnostic(data1,data2);
  #endif
          
  //OTTIMIZZA
  Serial.write(0xF0);
  Serial.write(0x77);
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2);
  Serial.write(data3);
  Serial.write(0xF7);
}
void Sysex(byte cmd,byte* message,byte size)
{
  Serial.write(0xF0);
  Serial.write(0x77);
  Serial.write(cmd);
  Serial.write(size);
  Serial.write(message, size);
  Serial.write(0xF7);
}
//============================

void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY)
{
  //status = MESSAGE + midichannel;
  Serial.write((MESSAGE));
  Serial.write(PITCH);
  Serial.write(VELOCITY);
}



