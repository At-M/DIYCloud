// IoTCloud v 0.1
// https://github.com/At-M/IoTCloud
// made by At-M
// BH1750 Sensor: 0x23

#include "ESP8266TrueRandom.h" // For Random dropchance
#include <FastLED.h>
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

#define NUM_LEDS 70
#define LEDDATA_PIN 0 // D3, 0
CRGB leds[NUM_LEDS];

// Needs Bridge from D0 (GPIO16) TO RESET PIN!
/* LED-Setup (from left to right):
    A: 10
    B: 12
    C: 14
    D: 20
    E: 5
    F: 10
*/
//The setup function is called once at startup of the sketch
void setup() {
  FastLED.addLeds<WS2812B, LEDDATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness(50);
  Serial.begin(9600);

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin(D2, D1);

  lightMeter.begin();
}
class Dropanim {
  public:
    int startcolumn[7] = {0, 10, 22, 36, 56, 61}; // start of the individual rows
    int endcolumn[7] = {9, 21, 35, 55, 60, 70}; // end of the individual rows
    int lastcolumn[7] = {0, 0, 0, 0, 0, 0}; // which column has a raindrop at which pixel
    int dropcolumn[7] = {0, 0, 0, 0, 0, 0};
    int yspeed = 1; // falling speed


    int randomcolumn() {
      int randcolumn = ESP8266TrueRandom.random(0, 6);
      return randcolumn;
    }


    int fall(int color, int sat) {
      int colS = randomcolumn();
      if (lastcolumn[colS] == 0) {
        // This Column doesnt have a raindrop in the last 3
        //lastcolumn[colS] = startcolumn[colS] different approach
        lastcolumn[colS] = 1;
        //  Serial.print("ColS is: ");
        // Serial.println(colS);
        int lednr = startcolumn[colS];
        leds[lednr] = CHSV(color, sat, 255); //set led blue, full saturation and brightness
        dropcolumn[colS] = startcolumn[colS];
      }
      else {
        /* Serial.print("Column ");
          Serial.print(colS);
          Serial.println(" wasn't 0, dropping led for one");
          // PRINTING ARRAY
          Serial.print("Dropcolumn: [ ");
          Serial.print(dropcolumn[0]);
          Serial.print(" | ");
          Serial.print(dropcolumn[1]);
          Serial.print(" | ");
          Serial.print(dropcolumn[2]);
          Serial.print(" | ");
          Serial.print(dropcolumn[3]);
          Serial.print(" | ");
          Serial.print(dropcolumn[4]);
          Serial.print(" | ");
          Serial.print(dropcolumn[5]);
          Serial.println(" ] ");
          // END PRINTING ARRAY
        */
        int lednr = dropcolumn[colS];
        int endled = endcolumn[colS];

        // If current drop is at the start
        if (dropcolumn[colS] == startcolumn[colS]) {
          /*  Serial.print("Drop in column ");
              Serial.print(colS);
            Serial.println("is at the start");
          */
          leds[lednr] = CHSV(color, sat, 127);//set led blue, full saturation half brightness
          leds[lednr + 1] = CHSV(color, sat, 255);//set led blue, full saturation and brightness
          leds[endled] = CHSV(color, sat, 0); // set led dark
        }
        // if current drop is at the end
        if (dropcolumn[colS] == endcolumn[colS] + 1) {
          /* Serial.print("Drop in column ");
            Serial.print(colS);
            Serial.println("is at the end");
            // do nothing
          */
          leds[lednr] = CHSV(color, sat, 0); // set led dark
          dropcolumn[colS] = startcolumn[colS];
        }

        /*
          // If Dropcolumn is atleast 3 away, reset dropchance
          if (dropcolumn[colS] == startcolumn[colS] + 3) {
          // drop one;
          lastcolumn[colS] = 0;
          }
        */
        // drop one;
        //Serial.print("Drop in column ");
        //Serial.print(colS);
        // Serial.println("is dropping");
        dropcolumn[colS] = dropcolumn[colS] + 1;
        leds[lednr - 1] = CHSV(color, sat, 0);
        leds[lednr] = CHSV(color, sat, 127);
        leds[lednr + 1] = CHSV(color, sat, 255);

      }


    }

    void animshow(int color) {
      int ledcolor = 1;
      int sat = 0;
      // Fall that
      switch (color) {

        case 0:
          sat = 0; // saturation will go to white
          break;
        case 1:
          ledcolor = 150; // blue
          sat = 255;
          break;
        case 2:
          ledcolor = 0; // red
          sat = 255;
          break;
        default:
          ledcolor = color;
          sat = 255;
      }
      fall(ledcolor, sat);
      // show that
      FastLED.show();
      // FastLED.delay(20);


    }

};

void gettemp() {

  Serial.print("test");

}


void loop() {

  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);

  int intbro = 0;
  do {
    Dropanim d;
    d.animshow(1); // 0 = white, 1 = blue, 2= red
    intbro++;
  } while (intbro < 50);

}
