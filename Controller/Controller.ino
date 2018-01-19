/*   
 *    Скетч контроллера уровня 1
 *    Связующее звену между датчиками и контроллером уровня 2
 */

#include <SPI.h>          // библиотека для работы с шиной SPI

// Радиомодуль nRF24L01
#include "nRF24L01.h"     
#include "RF24.h"         



#define DEBUG
#include "Macros.h"
#include "Sensors.h"
#include "sh_types.h"


// ----------------------------------------------------------------------
// Настройки радиомодуля
RF24 radio(9, 10);  // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб


// Набор датчиков
TSensors *sensors[10];


void loadTransiver()
{
  // Радиомодуль
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик

  #ifdef DEBUG
  radio.printDetails();
  #endif
  
  MESSAGE("RADIO READY");
  delay(100);
}

void setup() 
{
  Serial.begin(9600); //открываем порт для связи с ПК
  MESSAGE("BOOT");
  loadTransiver();
}



// Производим создание/настройку датчиков
bool getSetup()
{
  TThemperatureSensor* ts = new TThemperatureSensor(2);
  sensors[0] = ts;
  ts->create();
  ts->start();

  return true;
}

// Ждем команду
int getCommand()
{
  static bool t = false;
  
  if (t)
  {
    return DC_SILENCE;
  }
  else
  {
    t = true;
    return DC_CONFIGURE; 

  }
  
  /*switch(debugStatus)
  {
    case DC_SILENCE:
    debugStatus = DC_CONFIGURE;
    break;
    
    case DC_CONFIGURE:
    debugStatus = DC_SILENCE;
    break;
  }*/
 
  // Чтение из радиомодуля 
  //return debugStatus;
}

/***************************************************************
 * Главная функция
 */
void loop() 
{
  static TDeviceStatus deviceStatus = DS_NULL;
  
  int controllerCommand = getCommand();
  if (controllerCommand != DC_SILENCE)
  {
    MESSAGE("COMMAND: " + String(controllerCommand));
  }
  
  if ( controllerCommand == DC_CONFIGURE )
  {
    MESSAGE("SETUP");
    if ( getSetup() )
    {
      deviceStatus = DS_READY;  // Для теста
      MESSAGE("READY");
    }
    return;
  }

  if ( deviceStatus != DS_READY )
  {
    MESSAGE("NOT READY");
    delay(100);
    return;  
  }

  if ( controllerCommand == DC_SILENCE || controllerCommand == DC_GET_DATA_RIGHT_NOW)
  {
    
    float temperature0 = sensors[0]->getData();
    byte t = (byte)temperature0;
    radio.write(&t, sizeof(t)); // Отправляем данные через радиомодуль
   
  }
  delay(10);
}
