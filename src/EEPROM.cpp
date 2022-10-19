#include "define.h"

//MARK : EEPROM READ 
//Прочесть IP адресс клиента и записать в массив ipchar
void EEPROM_READ_ip_client(void)
{
  mString <64> str;
  //Читаем из EEPROM адресс клиента
  for (int n = 0; n < 20; n++)
  {
    ipchar[n] = EEPROM.read(n + EEPROM_ADRESS_IP_CLIENT);
  }
  sprintf(str.buf, "EEPROM IP Client %s\n", ipchar);
  Serial1.print(str.c_str());
}

//Прочитать из EEPROM битрейт для Serial2
void EEPROM_READ_Serial2Bitrate(void)
{
  EEPROM.get(EEPROM_ADRESS_Serial2Bitrate, Serial2Bitrate); //Читаем битрейт
  Serial1.print(">EEPROM READ битрейт = ");
  Serial1.println(Serial2Bitrate);
  if ((Serial2Bitrate < 120) || (Serial2Bitrate > 10000000))
  {
     Serial2Bitrate = 115200;
     EEPROM.put(EEPROM_ADRESS_Serial2Bitrate, Serial2Bitrate);
     EEPROM.commit();
     Serial1.println(">EEPROM Ошибка битрейта сброс до 115200");
  }
}

//Прочитать из EEPROM таймаут
void EEPROM_READ_timeout(void)
{
  EEPROM.get(EEPROM_ADRESS_timeout, timeout); //Читаем битрейт

  Serial1.print(">EEPROM READ Таймаута = ");
  Serial1.println(timeout);
  if ((timeout < 0) || (timeout > 10000))
  {
     timeout = 1000;
     EEPROM.put(EEPROM_ADRESS_timeout, timeout);
     EEPROM.commit();
     Serial1.println(">EEPROM Ошибка таймаута сброс до 1000");
  }
}

//Прочитать из EEPROM echo
void EEPROM_READ_echo(void)
{
  EEPROM.get(EEPROM_ADRESS_echo, echo);
  
  ; //Читаем битрейт
  Serial1.print(">EEPROM READ Эхо = ");
  Serial1.println(echo);
  if ((echo < 0) || (echo > 1))
  {
     echo = 1;
     EEPROM.put(EEPROM_ADRESS_echo, echo);
     EEPROM.commit();
     Serial1.println(">EEPROM Ошибка таймаута сброс до 1");
  }
}

//Прочитать из EEPROM broadcast
void EEPROM_READ_broadcast(void)
{
  EEPROM.get(EEPROM_ADRESS_broadcast, broadcast); //Читаем битрейт
  Serial1.print(">EEPROM READ Броадкаст = ");
  Serial1.println(broadcast);
  if ((broadcast < 0) || (broadcast > 1))
  {
     broadcast = 1;
     EEPROM.put(EEPROM_ADRESS_broadcast, broadcast);
     EEPROM.commit();
     Serial1.println(">EEPROM Ошибка броадкаст сброс до 1");
  }
}

