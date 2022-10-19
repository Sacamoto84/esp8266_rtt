#include "define.h"

LoginPass lp;

char   ipchar[20];

//Глобальный настройки
int SerialBitrate  = 921600;  //Битрейт
int Serial2Bitrate = 4000000; //Битрейт
int timeout        = 1000;    //Задержка новос строки
int echo           = 1;       //Эхо на Serial
int broadcast      = 0;       //Использовать броадкаст пакеты

int all_TX_to_UDP;
int all_RX_from_UDP;

void EXT_reset(void)
{
  Serial1.println("EXT Reset"); 
  digitalWrite(EXT_RESET, 0); 
  delay(1);
  digitalWrite(EXT_RESET, 1); 
  delay(50); 
}

