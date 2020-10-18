#include "ESP8266TrueRandom.h" // For Random - rain
#include <FastLED.h>


#define NUM_LEDS 70
#define DATA_PIN 2 // D4, 2


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
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness(50);
  Serial.begin(115200);

}
class Raindrop {
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


    int fall() {
      int colS = randomcolumn();
      if (lastcolumn[colS] == 0) {
        // This Column doesnt have a raindrop in the last 3
        //lastcolumn[colS] = startcolumn[colS] different approach
        lastcolumn[colS] = 1;
        Serial.print("ColS is: ");
        Serial.println(colS);
        int lednr = startcolumn[colS];
        leds[lednr] = CHSV(150, 255, 255);
        dropcolumn[colS] = startcolumn[colS];
      }
      else {
        Serial.print("Column ");
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

        int lednr = dropcolumn[colS];
        int endled = endcolumn[colS];

        // If current drop is at the start
        if (dropcolumn[colS] == startcolumn[colS]) {
          Serial.print("Drop in column ");
          Serial.print(colS);
          Serial.println("is at the start");
          leds[lednr] = CHSV(150, 255, 127);
          leds[lednr + 1] = CHSV(150, 255, 255);
          leds[endled] = CHSV(150, 255, 0);
        }
        // if current drop is at the end
        if (dropcolumn[colS] == endcolumn[colS] +1) {
          Serial.print("Drop in column ");
          Serial.print(colS);
          Serial.println("is at the end");
          // do nothing
           leds[lednr] = CHSV(150, 255, 0);
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
        Serial.print("Drop in column ");
          Serial.print(colS);
          Serial.println("is dropping");
        dropcolumn[colS] = dropcolumn[colS] + 1;
        leds[lednr - 1] = CHSV(150, 255, 0);
        leds[lednr] = CHSV(150, 255, 127);
        leds[lednr + 1] = CHSV(150, 255, 255);

      }


    }

    void rainshow() {

      //check first column
      //if led on, fall one down
      //if led on and +1 also on, fall two down and set first off

      // Fall that shit
      fall();
      // show that shit
      FastLED.show();
      FastLED.delay(20);


    }

};


void loop() {
  Serial.println("Start LOOP");
  Raindrop r;
  int i = 0;
  do {
    r.rainshow();
  } while (i == 0);
  Serial.println("END LOOP");
} // end loop
