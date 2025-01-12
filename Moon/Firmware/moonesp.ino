#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME " Moon"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_NeoPixel.h>
#include "pitches.h"
// You should get Auth Token in the Blynk App.
char auth[] = "";

// Your WiFi credentials.
char ssid[] = ""; //Jméno sítě 
char pass[] = ""; //Heslo

#define PIN D4
#define speaker  D3
#define NUMPIXELS 13
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int red, green, blue;
bool ledState = LOW;
// notes in the melody:
int melody[] = {

  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {

  4, 8, 8, 4, 4, 4, 4, 4
};

BLYNK_WRITE(V1) // Red value assigned to V1
{
  red = param.asInt();
  updateLEDs();
}

BLYNK_WRITE(V2) // Green value assigned to V2
{
  green = param.asInt();
  updateLEDs();
}

BLYNK_WRITE(V3) // Blue value assigned to V3
{
  blue = param.asInt();
  updateLEDs();
}

void updateLEDs()
{
  for(int i=0; i<NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
  pixels.show();
}

BLYNK_WRITE(V4) // Button assigned to V4
{
  int pinValue = param.asInt();  // Get the value sent from the app
  if (pinValue == 1) {
    red = 0 ;
    green = 0;
    blue = 0;
    updateLEDs();
  } else {
    red = param.asInt();
    green = param.asInt();
    blue = param.asInt();
    updateLEDs();
  }
}
BLYNK_WRITE(V5) {
  int pinValue = param.asInt();  // Get the value sent from the app
  if (pinValue == 1) {
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.

    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

    int noteDuration = 1000 / noteDurations[thisNote];

    tone(speaker, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.

    // the note's duration + 30% seems to work well:

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    // stop the tone playing:

    noTone(8);

  }
  } else {
  }
}


void setup(){
  pinMode(speaker, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  pixels.begin();



}

void loop()
{
  Blynk.run();
}
