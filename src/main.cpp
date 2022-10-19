#include <Arduino.h>
#include "define.h"
#include "ESPAsyncUDP.h"
#include "uart.h"
#include "ESP8266WiFi.h"

#include <ArduinoOTA.h>

#include "user_interface.h"
#include "lwip/opt.h"
#include "lwip/inet.h"
#include "lwip/udp.h"
#include "lwip/igmp.h"

#include <stdio.h>
#include <string.h>

AsyncUDP udp;

uart_t* U0;

void parsePacket(AsyncUDPPacket packet);
void TaskReadUART();


int length   = 0;
int rxBytes  = 0;
#define BUF_SIZE 2048
char buf[BUF_SIZE];



void setup() {

  pinMode(EXT_RESET, OUTPUT_OPEN_DRAIN);
  digitalWrite(EXT_RESET, 1); 

  EEPROM.begin(100); 
  //SerialBitrate = 2000000;
  Serial1.begin(SerialBitrate);

  EEPROM_READ_Serial2Bitrate(); //Прочитать из EEPROM битрейт для Serial2
  


  EEPROM_READ_ip_client();      //Читаем из EEPROM адресс клиента -> ipchar
  EEPROM_READ_timeout();   //Прочитать из EEPROM таймаут
  EEPROM_READ_echo();      //Прочитать из EEPROM echo
  EEPROM_READ_broadcast(); //Прочитать из EEPROM broadcast

  EEPROM.get(0, lp);  
  Serial1.print(">EEPROM READ SSID:");  Serial1.println(lp.ssid);  Serial1.print(">EEPROM READ PASS:"); Serial1.println(lp.pass);

  //Подключение к Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(lp.ssid, lp.pass);

  int count  = 0;
  bool needAP = false; //Если сети нет создаем точку доступа
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial1.print(".");
    count++;
    if (count>20)
    {
      //Сеть не найдена
      Serial1.println("Wifi STA не найдена");
      needAP = true; //Нужно создать точку доступа
      break; //Выходим
    }
  }

  //Создаем точку доступа
  if (needAP)
  {
    Serial1.println("Запуск точки доступа");
    // запускаем точку доступа
    WiFi.mode(WIFI_AP);
    WiFi.softAP("AP ESP32");

    // запускаем портал
    portal_start();

    // работа портала
    while (true){portal_tick();}  
  }

  Serial1.println();
  Serial1.print("Connected! Local IP: ");
  Serial1.println(WiFi.localIP());

  //подключаем конструктор и запускаем
  portal_start();

  if(udp.listen(8889)) {
     Serial.println("udp.listen(8889)");
     udp.onPacket([](AsyncUDPPacket packet) {
      Serial1.println(">Принят Пакет");
      if ( strstr((const char*)packet.data(), "Reset") )
      {
        digitalWrite(EXT_RESET, 0); 
        int c = 0;   
        unsigned long timing1;
        timing1 = millis();
        while ((millis() - timing1) < 10)
        {
          c++;
        }
        digitalWrite(EXT_RESET, 1); 
        udp.broadcastTo("Перезагрузка OK", 8888);
      }
     });
  }

 if (broadcast == 0)
 {
    mString <32> s;
    s += APP_TITLE;
    IPAddress ipclient;
    ipclient.fromString(ipchar);
    udp.writeTo((uint8_t* )s.c_str(), s.length() , ipclient , 8888);      
  }
  else
    udp.broadcastTo(APP_TITLE, 8888);

  ArduinoOTA.begin();

  //Serial2Bitrate = 2000000;
  U0 = uart_init(UART0, Serial2Bitrate, UART_8N2 , UART_RX_ONLY, 0 , SERIAL2_SIZE_RX, 0 );
  uart_swap(U0, 15);

 Serial1.print("U0 RX Beffer Size : ");
 Serial1.println(uart_get_rx_buffer_size(U0));
 
}

unsigned long timing; // Переменная для хранения точки отсчета

void loop() {

  TaskReadUART();

  if (millis() - timing > 1000){ // Вместо 10000 подставьте нужное вам значение паузы 
    timing = millis(); 
    portal_tick();
    ArduinoOTA.handle();
  }

  //Serial1.print("342342u56r4544");
}

unsigned long timingReadUART; // Переменная для хранения точки отсчета
unsigned long timingTemp;
void TaskReadUART() {
  
   int avalible = uart_rx_available(U0);
   rxBytes = 0;
   memset(buf,0, BUF_SIZE);

   if ((avalible > 1023) || (millis() - timingReadUART > 50))
   {
     rxBytes = uart_read(U0, &buf[0], avalible);
   }

  if ( (rxBytes > 0) && ( rxBytes != -1 ) && (buf[0] != 0)) 
  {
    timingReadUART = millis();
    all_TX_to_UDP += rxBytes;

    //Поищем конец строки
    char str[128];
    memset(str,0, 128);
    for(int i = 0 ; i < 127 ; i++)
    {
      //if (uart_rx_available(U0) == 0)
      //  break;
      int c = uart_read_char(U0);
      if (c == -1) {
         timingTemp = millis();
         while((millis() - timingTemp)< 20)
         {
         }
         if (uart_rx_available(U0) == 0)
           break;
         else
           c = uart_read_char(U0);
      }
      
      str[i] = (char)(c & 0xFF);
      if (str[i] == '\n') break;


    }
    strcat(buf, str);
    
    //Serial.print("✅ !TaskReadUART>> RAW buf >>"); 
    //Serial.println(buf);
    
    if (echo) {
      Serial1.println("🌐 >>UDP_tx_task>>Эхо:"); 
      Serial1.println(buf);
    }

    if (broadcast == 0)
    {
      IPAddress ipclient;
      ipclient.fromString(ipchar);
      udp.writeTo((uint8_t* )buf, strlen(buf) , ipclient , 8888); 
    }
    else
      udp.broadcastTo(buf, 8888);  
  } 

}