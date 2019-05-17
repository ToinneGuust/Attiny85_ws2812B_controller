#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <Arduino.h>

struct Configuration
{
  uint32_t setColor;
  byte setBrightness;
  unsigned long setDuration;
} configuration;

static byte buttonPin = 0;
static byte WS2801BPIN = 1;
static byte LDRpin = A1;
static byte ledPin = 3;
static byte sensitivityPin = A2;
static byte numberOfPixels = 10;

unsigned long intervalColorChange = 2300; //2300 * 255 ~ cycle all colors every 10 minutes
byte colorWheelPosition = 0;
unsigned long colorWheelLastChangeMillis;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numberOfPixels, WS2801BPIN, NEO_GRB + NEO_KHZ800);

uint32_t setColor = 255; //blue color
byte setBrightness = 255; //full brightness
unsigned long setDuration = 7200000; //2 hours

boolean menuActive = false;
byte menuItem = 0;
unsigned long menuStartMillis;
unsigned long menuTimeout = 30000;

boolean LDRtrigger = true;
unsigned long nightLightStartMillis;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  for (int j = 0; j < 2; j++)
  {
    for (int i = 0; i < 256; i++)
    {
      uint32_t tempColor = Wheel(i);

      for (int i = 0; i < strip.numPixels(); i++)
      {
        strip.setPixelColor(i, tempColor);
      }
      strip.show();
      delay(5);
    }
  }

  LoadSettings();

  strip.clear();
  strip.show();
  delay(1000);

  showColorOnAllLEDs(setColor, setBrightness);
  digitalWrite(ledPin, HIGH);
  delay(1000);

  strip.clear();
  strip.show();
  digitalWrite(ledPin, LOW);
  delay(1000);

  showColorOnAllLEDs(setColor, setBrightness);
  digitalWrite(ledPin, HIGH);
  delay(1000);

  strip.clear();
  strip.show();
  digitalWrite(ledPin, LOW);
}

void loop() {
  if (digitalRead(buttonPin) == LOW || menuActive)
  {
    manageMenu();
  }
  else
  {
    manageLEDStrip();
  }

}

void showColorOnAllLEDs(uint32_t color, byte brightness)
{
  strip.clear();
  strip.setBrightness(brightness);
  for (int i = 0; i < strip.numPixels(); i ++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void manageLEDStrip()
{
  int LDRValue = analogRead(LDRpin);
  int LDRtriggerValue = analogRead(sensitivityPin);

  if (LDRValue > LDRtriggerValue)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }

  //Start night light LDR was triggered
  if (LDRValue < LDRtriggerValue && LDRtrigger)
  {
    LDRtrigger = false;
    nightLightStartMillis = millis();
  }

  //Reset LDR trigger if lights go on
  if (LDRValue > LDRtriggerValue && LDRtrigger == false)
  {
    LDRtrigger = true;
    nightLightStartMillis = 0;
    //delay(10000); //Wait 10 seconds for possible flicker from lamps to go away.
  }

  //
  if (LDRValue < LDRtriggerValue && millis() - nightLightStartMillis < setDuration)
  {
    //Change the color
    if (intervalColorChange < millis() - colorWheelLastChangeMillis)
    {
      colorWheelPosition++;
      colorWheelLastChangeMillis = millis();
    }

    //Calculate brightness
     byte calculatedBrightness = (uint32_t)255 - (((millis() - nightLightStartMillis)*(uint32_t)setBrightness)/setDuration);

    //Show the color
    showColorOnAllLEDs(Wheel(colorWheelPosition), calculatedBrightness);
 
  } else {
    strip.clear();
    strip.show();
  }

}


