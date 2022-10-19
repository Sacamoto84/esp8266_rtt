#ifndef _define_h
#define _define_h

#include <Arduino.h>
#include <EEPROM.h>
#include "mString.h"

#include <Wire.h>

#include "ESPAsyncUDP.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "pins_arduino.h"
#include "HardwareSerial.h"
//#include "soc/soc_caps.h"
//#include "driver\uart.h"

//┌────────────────────────────────────────────────────┐
#define APP_TITLE  "UART to UDP V16 ESP8266\n"
#define SERIAL2_SIZE_RX  1024*8
//└────────────────────────────────────────────────────┘
//┌────────────────────────────────────────────────────┐
//  MARK: EEPROM ADRESS
#define EEPROM_ADRESS_Serial2Bitrate 40  //Адресс битрейта 
#define EEPROM_ADRESS_timeout        44
#define EEPROM_ADRESS_echo           48
#define EEPROM_ADRESS_IP_CLIENT      52
#define EEPROM_ADRESS_broadcast      72
//└────────────────────────────────────────────────────┘
//┌────────────────────────────────────────────────────┐
//MARK: Define
#define AP_SSID "TP-Link_BC0C"
#define AP_PASS "58133514"
#define RXD2 16
#define TXD2 17
#define RTS2 21

#define EXT_RESET   14// D5

//└────────────────────────────────────────────────────┘

struct LoginPass {
  char ssid[20];
  char pass[20];
};

extern LoginPass lp;

extern char ipchar[20];

//Глобальный настройки
extern int SerialBitrate;     //Битрейт
extern int Serial2Bitrate;    //Битрейт
extern int timeout;           //Задержка новос строки
extern int echo;              //Эхо на Serial
extern int broadcast;         //Использовать броадкаст пакеты

extern int all_TX_to_UDP;
extern int all_RX_from_UDP;

//
//#define SCREEN_WIDTH 128
//#define SCREEN_HEIGHT 64

//extern Adafruit_SSD1306 oled;
//

void EXT_reset(void);

//MARK: EEPROM
void EEPROM_READ_ip_client(void);
void EEPROM_READ_Serial2Bitrate(void); //Прочитать из EEPROM битрейт для Serial2
void EEPROM_READ_timeout(void);        //Прочитать из EEPROM таймаут
void EEPROM_READ_echo(void);           //Прочитать из EEPROM echo
void EEPROM_READ_broadcast(void);      //Прочитать из EEPROM broadcast

//MARK: OLED
//void oled_begin(void);
//void oled_refresh(void);

//MARK :Task
void TaskReadUART( void * parameter);

//MARK: Portal
void portal_start(void);
void portal_startAP(void);
void portal_tick(void);

#endif