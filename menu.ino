void manageMenu()
{

  //If a button is pushed reset the menu timeout
  if (digitalRead(buttonPin) == LOW)
  {
    menuActive = true;
    menuStartMillis = millis();
  }

  //Exit menu if menutimeout is reached
  if (digitalRead(buttonPin) == HIGH && millis() - menuStartMillis > menuTimeout )
  {
    exitMenu();
  }

  //If the menu is active and no option is chosen clear the ledstrip
  if (digitalRead(buttonPin) == HIGH && menuItem == 0 && menuActive == true)
  {
    strip.clear();
    strip.show();
  }

  //If no menuitem was chosen
  if (digitalRead(buttonPin) == LOW && menuItem == 0)
  {
    unsigned long tempMillis = millis();

    //Menuitem selection (1,2 or 3 leds light up to show the current selection
    while (digitalRead(buttonPin) == LOW)
    {
      //button pushed and then released between 1000ms and 2000ms => menuitem 1
      if (millis() - tempMillis > 1000 && millis() - tempMillis < 2000 && menuItem != 1)
      {
        menuItem = 1;
        strip.clear();
        strip.setBrightness(255);
        strip.setPixelColor(0, strip.Color(255, 255, 255));
        strip.show();
        delay(250); //to avoid a bouncing signal to trigger a value in the selected menu item
      }

      //button pushed and then released between 2000ms and 3000ms => menuitem 2
      if (millis() - tempMillis > 2000 && millis() - tempMillis < 3000 && menuItem != 2)
      {
        menuItem = 2;
        strip.clear();
        strip.setBrightness(255);
        strip.setPixelColor(0, strip.Color(255, 255, 255));
        strip.setPixelColor(1, strip.Color(255, 255, 255));
        strip.show();
        delay(250); //to avoid a bouncing signal to trigger a value in the selected menu item
      }

      //button pushed and then released between 2000ms and 3000ms => menuitem 3
      //      if (millis() - tempMillis > 3000 && millis() - tempMillis < 4000 && menuItem != 3)
      //      {
      //        menuItem = 3;
      //        strip.clear();
      //        strip.setBrightness(255);
      //        strip.setPixelColor(0, strip.Color(255, 255, 255));
      //        strip.setPixelColor(1, strip.Color(255, 255, 255));
      //        strip.setPixelColor(2, strip.Color(255, 255, 255));
      //        strip.show();
      //        delay(250); //to avoid a bouncing signal to trigger a value in the selected menu item
      //      }

      //button pushed longer then 4000ms go back to menuoption 0
      if (millis() - tempMillis > 3000)
      {
        menuItem = 0;
        tempMillis = millis();
        strip.clear();
        strip.show();
        delay(250); //to avoid a bouncing signal to trigger a value in the selected menu item
      }
    }

    //If a menu item was choosen and
    if (digitalRead(buttonPin) == HIGH && menuItem != 0)
    {
      menuStartMillis = millis();
      switch (menuItem) {
        case 1:
          showMenuItem1();
          break;
        case 2:
          showMenuItem2();
          break;
          //        case 3:
          //          showMenuItem3();
          //          break;
      }
    }


  }
}

//show colorwheel until the button is pushed
//void showMenuItem3()
//{
//  uint32_t tempColor;
//  while (digitalRead(buttonPin) == HIGH && millis() - menuStartMillis < menuTimeout)
//  {
//    unsigned long wheelDuration = menuTimeout / 3;
//    unsigned long elapsedMillis = (millis() - menuStartMillis) % wheelDuration;
//    unsigned long calculatedColor = (elapsedMillis * 255) / wheelDuration;
//    tempColor = Wheel(calculatedColor);
//    showColorOnAllLEDs(tempColor,255);
//  }
//
//  if (digitalRead(buttonPin) == LOW)
//  {
//    setColor = tempColor;
//    for (int i = 0; i < strip.numPixels(); i++)
//    {
//      strip.setPixelColor(i, tempColor);
//    }
//    strip.show();
//    delay(5000);
//    exitMenu();
//  }
//}

//select brightness
void showMenuItem1()
{
  byte tempBrightness = 255;
  while (digitalRead(buttonPin) == HIGH && millis() - menuStartMillis < menuTimeout)
  {
    float elapsedMillis = millis() - menuStartMillis;
    int numberOfLeds = (int)((elapsedMillis / (float)(menuTimeout / 3)) * strip.numPixels()) % (strip.numPixels() + 1);
    tempBrightness = (byte)(((float)255 / (float)strip.numPixels()) * (float)numberOfLeds);
    strip.clear();
    strip.setBrightness(tempBrightness);
    for (int i = 0; i < numberOfLeds; i++)
    {
      strip.setPixelColor(i, setColor);
    }
    strip.show();

    if (digitalRead(buttonPin) == LOW)
    {
      setBrightness = tempBrightness;
      showColorOnAllLEDs(setColor, setBrightness);
      delay(5000);
      exitMenu();
    }
  }
}
//select duration

void showMenuItem2()
{
  unsigned long tempDuration;

  while (digitalRead(buttonPin) == HIGH && millis() - menuStartMillis < menuTimeout)
  {
    float elapsedMillis = millis() - menuStartMillis;
    int numberOfLeds = (int)((elapsedMillis / (float)(menuTimeout / 3)) * strip.numPixels());
    while (numberOfLeds > strip.numPixels())
    {
      numberOfLeds -= strip.numPixels();
    }
    tempDuration = numberOfLeds  * 1800000; // 30 minutes for every led
    //tempDuration = (byte)numberOfLeds  * 10 * 1000; // 10 seconds for every led DEBUG
    strip.clear();
    for (int i = 0; i < numberOfLeds; i++)
    {
      strip.setPixelColor(i, setColor);
    }
    strip.show();
  }

  if (digitalRead(buttonPin) == LOW)
  {
    setDuration = tempDuration;
    delay(5000);
    exitMenu();
  }

}

//void showMenuItem3()
//{
//  unsigned long tempDuration;
//
//  while (digitalRead(buttonPin) == HIGH && millis() - menuStartMillis < menuTimeout)
//  {
//    for (int i = 1; i <=  strip.numPixels(); i++)
//    {
//      tempDuration = i * 30 * 60 * 1000; // 30 minutes for every led
//      strip.clear();
//      for (int j = 0; j < i; j++)
//      {
//        strip.setPixelColor(j, setColor);
//      }
//      strip.show();
//      delay(900);
//    }
//  }
//
//  if (digitalRead(buttonPin) == LOW)
//  {
//    setDuration = tempDuration;
//    delay(5000);
//    exitMenu();
//  }
//
//}


void exitMenu()
{
  menuActive = false;
  menuStartMillis = 0;
  menuItem = 0;
  SaveSettings();
}

uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

