// Термодатчик DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>

#ifndef TSENSORS

/*
 * Общий интерфейс для датчиков
 */
class TSensors
{
public:
  //virtual void create() {};
  virtual void start() {};
  virtual float getData() {};
  //int sensorType;

};

// Термодатчик
class TThemperatureSensor: public TSensors
{
public:
   TThemperatureSensor(int sensorPin);
   void create();
   void start();
   float getData();
   //String getSensorInfo();
      
private:
  int pin;
  OneWire* oneWire;
  DallasTemperature* dt;
};

TThemperatureSensor::TThemperatureSensor(int sensorPin) : pin(sensorPin)
{
  create();
}

void TThemperatureSensor::create()
{
  // OneWire library
  oneWire = new OneWire(pin);

  // Pass our oneWire reference to Dallas Temperature. 
  dt = new DallasTemperature(oneWire);   
}

void TThemperatureSensor::start()
{
  MESSAGE("DS18B20 Started at pin: " + String(pin));
  dt->begin(); // Start up the library
}

float TThemperatureSensor::getData()
{
  dt->requestTemperatures(); // Send the command to get temperatures
  MESSAGE("Request done.");

  float result = dt->getTempCByIndex(0);

  MESSAGE("Result = " + String(result));
  return result;
}

/*String TThemperatureSensor::getSensorInfo()
{
  return "DS18B20";
}*/

#endif