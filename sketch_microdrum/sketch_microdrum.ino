 
//==============================================================================
//=>                microDRUM/nanoDRUM firmware v1.4.0                        <= 
//=>                        www.microdrum.net                                 <=
//=>                         CC BY-NC-SA 3.0                                  <=
//=>                                                                          <=
//=> Massimo Bernava                                                          <=
//=> massimo.bernava@gmail.com                                                <=
//=> 2016-02-10                                                               <=
//=>           ADAPTADO PARA O ARDUINO MEGA SEM USO DOS MULTIPLEX             <=
//=>                               POR                                        <=
//=>                       PEDRO HENRIQUE KNUP                                <=
//=>                      facebook.com/pedroknup                              <=
//=>                            06/06/2016                                    <=
//=>                                E                                         <=
//=>                      MARCOS CESAR MONECCHI                               <=
//=>                            22/06/2016                                    <=
//=>                            Revisão 0.4                                   <=
//==============================================================================

//========CONFIGURE=============
#define USE_LCD           0     // Use LCD
#define USE_595           0     // 1 with nanoDrum and v0.8
#define USE_PROFILER      0     // Use Profiler (only for Debug)
#define FASTADC           1     // FASTADC = Prescaler_16, VERYFASTADC = Prescaler_8, VERYVERYFASTADC = Prescaler_4
#define SERIALSPEED       1     // 1 = 115200, 0 = 31250 (MIDI) , picoMIDI use 115200 therefore with nanoDrum and v0.8 use 1
#define USE_DEFAULT_NAME  1     // Use Default Name for pin in LCD Menu
#define USE_PISERIAL      0     // Use Raspberry Pi serial
#define USE_WAVTRIGGER    0     // Use WavTrigger
#define WT_16             0     // WT_16 = 16 Wav Trigger Input , WT_24 = 24 Wav Trigger Input
#define ENABLE_CHANNEL    0     // Enable MIDI Channel
#define MENU_LOG          1     // Enable Auto Tune (only with LCD)
#define NANO              0     // 1 = nanoDrum , 0 = microDrum
#define MEGA              1     // 1 = Usa Arduino Mega sem multiplex, 0 = microdrum
#define USE_DIG           0     // 1 = Usa os pinos digitais, 0 = Desabilitado
//==============================

#if defined(__arm__) 
  //TODO: https://github.com/sebnil/DueFlashStorage
#elif defined(__AVR__) 
  #include <EEPROM.h>
#endif

#if defined(__AVR__) 
#if USE_WAVTRIGGER
  #include <SoftwareSerial.h>
#endif
#if USE_PISERIAL
  #include <SoftwareSerial.h>
#endif
#endif
//#include <math.h>

#if USE_LCD
  #if USE_595
    #include "LiquidCrystal595.h"
  #else
    #include <LiquidCrystal.h>
  #endif
#endif

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define TIMEFUNCTION millis() //NOT micros() (thresold error)
#define ANALOGREAD(s,p) analogRead(s)

#if defined(__arm__) 
  #define fastWrite(_pin_, _state_) digitalWrite(_pin_, _state_);
#elif defined(__AVR__) 
//#define fastWrite(_pin_, _state_) ( _pin_ < 8 ? (_state_ ?  PORTD |= 1 << _pin_ : PORTD &= ~(1 << _pin_ )) : (_state_ ?  PORTB |= 1 << (_pin_ -8) : PORTB &= ~(1 << (_pin_ -8)  )))
  #define fastWrite(_pin_, _state_) (_state_ ?  PORTD |= 1 << _pin_ : PORTD &= ~(1 << _pin_ ))
#endif

#if ENABLE_CHANNEL
  #define fastNoteOn(_channel,_note,_velocity) { Serial.write(0x90 | _channel);Serial.write(_note);Serial.write(_velocity); }
  #define fastMidiCC(_channel,_number,_value) { Serial.write((0xB0 | _channel)); Serial.write(_number); Serial.write(_value); }
#else
  #define fastNoteOn(_channel,_note,_velocity) { Serial.write(0x90 | 0x09); Serial.write(_note); Serial.write(_velocity);}
  #define fastMidiCC(_channel,_number,_value) { Serial.write((0xB0 | 0x09)); Serial.write(_number); Serial.write(_value); }
#endif

//===========MODE============
enum mode:byte
{
  Off     = 0,
  Standby = 1,
  MIDI    = 2,
  Tool    = 3
};
//============================

//===GLOBAL========================
mode Mode=MIDI;
unsigned long GlobalTime;
//=================================

//===SETTING================
//===General================

const int delayTime=10;
byte GeneralXtalk=0;
#if NANO    // Verifica o modelo de arduino e seta o numero de pinos: NANO= 24, MEGA=16, OUTROS=48
const byte NPin=24;
#elif MEGA
const byte NPin=16;
#else
const byte NPin=48;
#endif

byte NSensor=2;
//===========================

//===HiHat==================
byte HHNoteSensor[]     = {0,100};
byte HHThresoldSensor[] = {80,7};
byte HHFootNoteSensor[] = {59,48};
byte HHFootThresoldSensor[] = {127,127};
//===========================

//===Xtalk===================
const byte NXtalkGroup=4;//Max number XTalk groups
int MaxXtalkGroup[NXtalkGroup] = {-1};
int MaxMultiplexerXtalk[8]={-1};
//===========================

#if MEGA
 // Define as notas do chocke //
 #define choke1 78
 #define choke2 80   
 #define choke3 82
 #define chokeRide 63
 
 // Define as Notas dos Plugs Digitais Auxiliares //
 #define Aux1 47
 #define Aux2 47
 #define Aux3 47
 #define Aux4 47
 #define Aux5 47
 #define Aux6 47
 #define Aux7 47                                  

 //Define os Pinos Digitais dos Chokes
 #define Choke1_Pin 51 
 #define Choke2_Pin 49
 #define Choke3_Pin 47
 #define ChokeRide_Pin 53

 //Define os Pinos Digitais Auxiliares
 #define Aux1_Pin 33
 #define Aux2_Pin 35
 #define Aux3_Pin 37
 #define Aux4_Pin 39
 #define Aux5_Pin 41
 #define Aux6_Pin 43
 #define Aux7_Pin 45

//Define o estado dos pinos digitais
 int Choke1_State = LOW;
 int Choke2_State = LOW; 
 int Choke3_State = LOW;
 int ChokeRide_State = LOW;
 int Aux1_State = LOW;
 int Aux2_State = LOW;
 int Aux3_State = LOW;
 int Aux4_State = LOW;
 int Aux5_State = LOW;
 int Aux6_State = LOW;
 int Aux7_State = LOW; 
 
 int currentSwitchState = LOW;
#endif

#if USE_LCD
//Define os caracteres especiais que serão apresentados na tela ao se tocar

//spChar define a matriz de 16 posições onde se deve colocar o tipo de sensor (kick, snare, etc)
//de acordo com o seu esquema

 #define chSnare  0
 #define chTom    1
 #define chCymbal 2
 #define chHH     3
 #define chKick   4
 
byte spChar[16] = {chHH,chSnare,chSnare,chKick,chTom,chTom,chTom,chTom,chCymbal,chCymbal,chHH,chCymbal,chCymbal,chCymbal,chCymbal,chCymbal};
long tmChar[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

byte lcdSnare[8] = {
  0b00000,
  0b01110,
  0b10001,
  0b11111,
  0b11011,
  0b11111,
  0b01110,
  0b00000
};
byte lcdTom[8] = {
  0b00000,
  0b01110,
  0b10001,
  0b10001,
  0b11011,
  0b11111,
  0b01110,
  0b00000
};
byte lcdCymball[8] = {
  0b00000,
  0b00001,
  0b01110,
  0b01100,
  0b01010,
  0b10010,
  0b00010,
  0b00000
};
byte lcdHH[8] = {
  0b00000,
  0b00100,
  0b11011,
  0b00100,
  0b11011,
  0b00100,
  0b00100,
  0b00000
};
byte lcdKick[8] = {
  0b00000,
  0b00100,
  0b00010,
  0b00001,
  0b00011,
  0b11101,
  0b11111,
  0b00000
};
#endif

