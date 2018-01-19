typedef enum {
  DC_SILENCE = 0, // Нет комманд 
  DC_CONFIGURE,   // Команда сконфигурировать устройство. Произвести запись настроек в EEPROM
  DC_GET_DATA_RIGHT_NOW, 
  DC_START,
  DC_STOP,
  DC_REBOOT
} TCommand;

typedef enum {
  DS_NULL = 0, // 
  DS_READY
} TDeviceStatus;
