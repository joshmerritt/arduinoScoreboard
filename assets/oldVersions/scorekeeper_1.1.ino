// SeveralThingsAtTheSameTime.ino

// An expansion of the BlinkWithoutDelay concept to illustrate how a script
//  can appear to do several things at the same time

// this sketch does the following
//    it blinks the onboard LED (as in the blinkWithoutDelay sketch)
//    it blinks two external LEDs (LedA and LedB) that are connected to pins 12 and 11.
//    it turns another Led (buttonLed connected to pin 10) on or off whenever a button
//       connected to pin 7 is pressed
//    it sweeps a servo (connected to pin 5) back and forth at different speeds


//  One leg of each LED should be connected to the relevant pin and the other leg should be connected to a
//   resistor of 470 ohms or more and the other end of the resistor to the Arduino GND. 
//   If the LED doesn't light its probably connected the wrong way round.

//  On my Uno and Mega the "button" is just a piece of wire inserted into pin 7. 
//   Touching the end of the wire with a moist finger is sufficient to cause the switching action
//   Of course a proper press-on-release-off button switch could also be used!

//  The Arduino is not capable of supplying enough 5v power to operate a servo
//    The servo should have it's own power supply and the power supply Ground should
//      be connected to the Arduino Ground.

// The sketch is written to illustrate a few different programming features.
//    The use of many functions with short pieces of code. 
//       Short pieces of code are much easier to follow and debug
//    The use of variables to record the state of something (e.g. onBoardLedState) as a means to
//       enable the different functions to determine what to do.
//    The use of millis() to manage the timing of activities
//    The definition of all numbers used by the program at the top of the sketch where 
//       they can easily be found if they need to be changed


//========================================

// ----------LIBRARIES--------------

#include <Servo.h>
#include <FastLED.h>

#define LED_PIN     13
#define NUM_LEDS    1
#define BRIGHTNESS  10
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;



// --------CONSTANTS (won't change)---------------

const int onBoardLedPin =  13;      // the pin numbers for the LEDs
const int led_A_Pin = 12;
const int led_B_Pin = 11;
const int buttonLed_Pin = 10;

const int buttonPin = 7; // the pin number for the button

const int minuteDuration = 60000;  // number of millisecs in one minute

const int onBoardLedInterval = 500; // number of millisecs between blinks
const int led_A_Interval = 2500;
const int led_B_Interval = 4500;

const int blinkDuration = 500; // number of millisecs that Led's are on - all three leds use this

const int buttonInterval = 300; // number of millisecs between button readings

const int winningScore = 21;



//------------ VARIABLES (will change)---------------------

byte onBoardLedState = LOW;             // used to record whether the LEDs are on or off
byte led_A_State = LOW;           //   LOW = off
byte led_B_State = LOW;
byte buttonLed_State = LOW;

unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long previousOnBoardLedMillis = 0;   // will store last time the LED was updated
unsigned long previousLed_A_Millis = 0;
unsigned long previousLed_B_Millis = 0;
unsigned long previousButtonMillis = 0; // time when button press last checked
unsigned long previousElapsedGameTime = 0;

int redDisplay = 0;
int blueDisplay = 0;
int redScore = 0;
int blueScore = 0;
int displayGameTime = 0;
int elapsedGameTime = 0;

//========================================

void setup() {

  Serial.begin(9600);
  Serial.println("Starting SeveralThingsAtTheSameTimeRev1.ino");  // so we know what sketch is running
  
      // set the Led pins as output:
  pinMode(onBoardLedPin, OUTPUT);
  pinMode(led_A_Pin, OUTPUT);
  pinMode(led_B_Pin, OUTPUT);
  pinMode(buttonLed_Pin, OUTPUT);
  
      // set the button pin as input with a pullup resistor to ensure it defaults to HIGH
  pinMode(buttonPin, INPUT_PULLUP);
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  
  
}

//========================================

void loop() {

      // Notice that none of the action happens in loop() apart from reading millis()
      //   it just calls the functions that have the action code

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  
  FillLEDsFromPaletteColors(startIndex);
  
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);


  currentMillis = millis();   // capture the latest value of millis()
                              //   this is equivalent to noting the time from a clock
                              //   use the same time for all LED flashes to keep them synchronized
  
  readButton();               // call the functions that do the work
  updateOnBoardLedState();
  updateLed_A_State();
  updateLed_B_State();
  switchLeds();

}

//========================================

void updateScores() {
  if(redScore != redDisplay || blueScore != blueDisplay) {
  
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void updateElapsedGameTime() {

  if (currentMillis - previousElapsedGameTime >= minuteDuration) {
    elapsedGameTime++;
  }
  if ((elapsedGameTime % 5 == 0) && elapsedGameTime != displayGameTime) {
    for (int i = 0; i < elapsedGameTime/5; i++) {
//      LIGHT PIN (i);
    }
    if (elapsedGameTime > 15) {
        for (int j = 0; j < (20-elapsedGameTime); j++) {
          
//        LIGHT PIN(j);
        }
    }
  }
}

void readButton() {
      // this only reads the button state after the button interval has elapsed
      //  this avoids multiple flashes if the button bounces
      // every time the button is pressed it changes buttonLed_State causing the Led to go on or off
      // Notice that there is no need to synchronize this use of millis() with the flashing Leds
  int b0 = 100;
  int b1 = 200;
  int b2 = 300;
  int b3 = 400;
  String red1 = "Red: ";
  String blue1 = "  Blue: ";
  String time1 = "  Minutes Elapsed: ";
  
  if (millis() - previousButtonMillis >= buttonInterval) {

    if (analogRead(buttonPin) > b0 && analogRead(buttonPin) <= b1) {
      redScore++;
    } else if(analogRead(buttonPin) <= b2) {
        redScore--;
    } else if(analogRead(buttonPin) <= b3) {
        blueScore--;
    } else {
        blueScore++;
    }
      buttonLed_State = ! buttonLed_State; // this changes it to LOW if it was HIGH                                   
      Serial.println(red1 + redScore + blue1 + blueScore + time1 + elapsedGameTime);
      
      previousButtonMillis += buttonInterval;
    }
  }

  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void updateOnBoardLedState() {

  if (onBoardLedState == LOW) {
          // if the Led is off, we must wait for the interval to expire before turning it on
    if (currentMillis - previousOnBoardLedMillis >= onBoardLedInterval) {
          // time is up, so change the state to HIGH
       onBoardLedState = HIGH;
          // and save the time when we made the change
       previousOnBoardLedMillis += onBoardLedInterval;
          // NOTE: The previous line could alternatively be
          //              previousOnBoardLedMillis = currentMillis
          //        which is the style used in the BlinkWithoutDelay example sketch
          //        Adding on the interval is a better way to ensure that succesive periods are identical

    }
  }
  else {  // i.e. if onBoardLedState is HIGH
  
          // if the Led is on, we must wait for the duration to expire before turning it off
    if (currentMillis - previousOnBoardLedMillis >= blinkDuration) {
          // time is up, so change the state to LOW
       onBoardLedState = LOW;
          // and save the time when we made the change
       previousOnBoardLedMillis += blinkDuration;
    } 
  }
}

//========================================

void updateLed_A_State() {

  if (led_A_State == LOW) {
    if (currentMillis - previousLed_A_Millis >= led_A_Interval) {
       led_A_State = HIGH;
       previousLed_A_Millis += led_A_Interval;
    }
  }
  else {
    if (currentMillis - previousLed_A_Millis >= blinkDuration) {
       led_A_State = LOW;
       previousLed_A_Millis += blinkDuration;
    } 
  }    
}

//========================================

void updateLed_B_State() {

  if (led_B_State == LOW) {
    if (currentMillis - previousLed_B_Millis >= led_B_Interval) {
       led_B_State = HIGH;
       previousLed_B_Millis += led_B_Interval;
    }
  }
  else {
    if (currentMillis - previousLed_B_Millis >= blinkDuration) {
       led_B_State = LOW;
       previousLed_B_Millis += blinkDuration;
    }
  }    
}

//========================================

void switchLeds() {
      // this is the code that actually switches the LEDs on and off

  digitalWrite(onBoardLedPin, onBoardLedState);
  digitalWrite(led_A_Pin, led_A_State);
  digitalWrite(led_B_Pin, led_B_State);
  digitalWrite(buttonLed_Pin, buttonLed_State);
}

