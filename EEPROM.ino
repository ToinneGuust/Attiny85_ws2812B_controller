/////////////////////////////////
//   SAVES SETTINGS TO EEPROM  //
/////////////////////////////////
void SaveSettings()
{
  configuration.setColor = setColor;
  configuration.setBrightness = setBrightness;
  configuration.setDuration = setDuration;
  EEPROM_writeAnything(0, configuration);
}

/////////////////////////////////
//   SAVES SETTINGS TO EEPROM  //
/////////////////////////////////
void LoadSettings()
{
  EEPROM_readAnything(0, configuration);
  setColor = configuration.setColor;
  setBrightness = configuration.setBrightness;
  setDuration = configuration.setDuration;
}

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
  const byte* p = (const byte*)(const void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
  return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
  byte* p = (byte*)(void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}
