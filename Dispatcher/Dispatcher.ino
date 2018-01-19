/*
 * Скетч диспетчера
 * Диспетчер - промежуточное звено между контроллерами и сервером-смартфоном 
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "TM1637.h"

// дисплей
#define CLK 6 //определим константы как номера пинов на которые будут соединены с одноименными пинами на модуле
#define DIO 7 //это нужно будет позже
TM1637 tm1637(CLK,DIO);//создаем экземпляр объекта типа «TM1637», с которым будем далее работать и задаем пины.




//RF24 radio(9,10); // "создать" модуль на пинах 9 и 10 Для Уно
RF24 radio(9,53); // для Меги
byte gotByte;
float answer;

byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  //возможные номера труб

void setup(){
  Serial.begin(9600); //открываем порт для связи с ПК
  

  // display
  tm1637.init();// инициализация библиотеки «TM1637.h»
  tm1637.set(BRIGHT_TYPICAL);//установка яркости указанная константа равна 2, значение по умолчанию

  
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0,15);     //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openReadingPipe(1,address[0]);      //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  
  radio.powerUp(); //начать работу ??? Yflj kb 'nj
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
  
}

void loop() {
  byte pipeNo;                          
  
  while( radio.available(&pipeNo))
  {    

    
    // слушаем эфир со всех труб
    //radio.read( &gotByte, sizeof(gotByte) );         // чиатем входящий сигнал
    radio.read( &answer, sizeof(answer) );         // чиатем входящий сигнал

    Serial.print("Recieved: ");
    Serial.println(answer);

    tm1637.display((int)answer);


  }
  //Serial.println("No data");

  
}

