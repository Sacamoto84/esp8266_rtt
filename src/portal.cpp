#include "define.h"
#include "GyverPortal.h"
GyverPortal portal;

void build();
void buildAP();
void actionAP(GyverPortal& p);
void action(GyverPortal& p);

void portal_start_AP(void)
{
  // запускаем портал
  portal.attachBuild(build);
  portal.start(WIFI_AP);
  portal.attach(action);
}

void portal_start(void)
{
  portal.attachBuild(build);
  portal.attach(action);
  portal.start("esp");
}


void portal_tick(void)
{
    portal.tick();
}

// конструктор страницы
void build() {
 BUILD_BEGIN();
  GP.THEME(GP_LIGHT);

  GP.LABEL("Wifi");
  GP.BLOCK_BEGIN();
  GP.FORM_BEGIN("/login");
  GP.LABEL("SSID");
  GP.BREAK();
  GP.TEXT("lg", "Login", lp.ssid);
  GP.BREAK();
  GP.LABEL("Пароль");GP.BREAK();
  GP.TEXT("ps", "Password", lp.pass);
  GP.SUBMIT("Сохранить");
  GP.FORM_END();
  GP.BLOCK_END();

  GP.LABEL("Настройка");
  GP.BLOCK_BEGIN();
  //┌────────────────────────────────────────────────────┐
      GP.LABEL("IP адресс клиента (192.168.0.101)");   //│
      GP.TEXT("ipclient", "ipclient", ipchar);         //│
      GP.BUTTON_MINI("saveipclient", "Сохранить");     //│
      GP.BREAK();                                      //│
  //└────────────────────────────────────────────────────┘
    //┌──────────────────────────────────────────────────┐
      GP.BREAK();                                      //│  
      GP.CHECK("сheck_broadcast", broadcast );         //│
      GP.LABEL("Использовать broadcast");  
      GP.BREAK();                                      //│
  //└────────────────────────────────────────────────────┘
  //┌────────────────────────────────────────────────────┐
      GP.LABEL("Serial2 битрейт");                     //│
      GP.NUMBER("bitrate", "bitrate", Serial2Bitrate); //│
      GP.BUTTON_MINI("saveBR", "Сохранить");           //│
  //└────────────────────────────────────────────────────┘
  //┌────────────────────────────────────────────────────┐
      GP.BREAK();                                      //│ 
      GP.LABEL("Таймаут новой строки ms");             //│ 
      GP.NUMBER("timeout", "timeout", timeout);        //│ 
      GP.BUTTON_MINI("saveTimeout", "Сохранить");      //│ 
  //└────────────────────────────────────────────────────┘
  //┌────────────────────────────────────────────────────┐
      GP.BREAK();                                      //│  
      GP.CHECK("сheck_echo", echo );                   //│
      GP.LABEL("Эхо на Serial");                       //│
  //└────────────────────────────────────────────────────┘

  GP.BLOCK_END();

  GP.BLOCK_BEGIN();
  GP.BUTTON("rst", "ESP32 reset");   GP.BREAK();
  GP.BLOCK_END();

  BUILD_END();
}

void action(GyverPortal& p) {
int valNum = 99;
String valText;

//┌────────────────────────────────────────────────────┐
//  Изменение адреса IP клиента                      //│
if (p.click("ipclient")) {                           //│
      valText = portal.getString("ipclient");        //│
      Serial1.print("ipclient: ");                    //│
      Serial1.println(valText);                       //│
      valText.toCharArray(ipchar, 20, 0);            //│
    }                                                //│  
                                                     //│
//Кнопка сохранить в EEPROM IP клиента               //│
if (p.click("saveipclient")){                        //│ 
    Serial1.println("Сохранить в EEPROM IP клиента"); //│
    EEPROM.put(EEPROM_ADRESS_IP_CLIENT, ipchar);
    EEPROM.commit();                                 //│        
  }                                                  //│
//└────────────────────────────────────────────────────┘
//┌────────────────────────────────────────────────────┐
//Броадкас пакеты
if (portal.click("сheck_broadcast")) {
      bool valSwitch = portal.getCheck("сheck_broadcast");
      Serial1.print("Switch: ");
      Serial1.println(valSwitch);
      Serial1.println("Сохранить броадкаст");
      EEPROM.put(EEPROM_ADRESS_broadcast, valSwitch);  
      EEPROM.commit();                                
      broadcast = valSwitch;
    }
//└────────────────────────────────────────────────────┘

if (portal.click("сheck_echo")) {
      bool valSwitch = portal.getCheck("сheck_echo");
      Serial1.print("Switch: ");
      Serial1.println(valSwitch);
      Serial1.println("Сохранить Эхо");
      EEPROM.put(EEPROM_ADRESS_echo, valSwitch);  // сохраняем
      EEPROM.commit();                                 // записываем
      echo = valSwitch;
    }

//Изменение битрейда
if (p.click("bitrate")) {
      valNum = portal.getInt("bitrate");
      Serial1.print("bitrate: ");
      Serial1.println(valNum);
      Serial2Bitrate = valNum;
    }

//Изменение Таймаута
if (p.click("timeout")) {
      valNum = portal.getInt("timeout");
      Serial1.print("timeout: ");
      Serial1.println(valNum);
      timeout = valNum;
    }

//Кнопка сохранить в EEPROM битрейт
if (p.click("saveBR")){ 
    Serial1.println("Сохранить битрейт");
    EEPROM.put(EEPROM_ADRESS_Serial2Bitrate, Serial2Bitrate);  // сохраняем
    EEPROM.commit();                      // записываем
  }

//Кнопка сохранить в EEPROM таймаут
if (p.click("saveTimeout")){ 
    Serial1.println("Сохранить Таймаут");
    EEPROM.put(EEPROM_ADRESS_timeout, timeout);  // сохраняем
    EEPROM.commit();                      // записываем
  }

  if (p.click("rst")){
    Serial1.println("Перезагрузка"); ESP.restart();}
  
  if (p.form("/login")) {  
    Serial1.println("Сохранить login в EEPROM");
    p.copyStr("lg", lp.ssid);  // копируем себе
    p.copyStr("ps", lp.pass);
    EEPROM.put(0, lp);              // сохраняем
    EEPROM.commit();                // записываем
  }

}
