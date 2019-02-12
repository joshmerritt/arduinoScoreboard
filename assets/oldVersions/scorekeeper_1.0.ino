/*
  State change detection (edge detection)

 Often, you don't need to know the state of a digital input all the time,
 but you just need to know when the input changes from one state to another.
 For example, you want to know when a button goes from OFF to ON.  This is called
 state change detection, or edge detection.

 This example shows how to detect when a button or button changes from off to on
 and on to off.

 The circuit:
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 * LED attached from pin 13 to ground (or use the built-in LED on
   most Arduino boards)

 created  27 Sep 2005
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/ButtonStateChange

 */

// this constant won't change:
const int  buttonPin = A0;    // the pin that the pushbutton is attached to
const int tenLedPin = 3;
const int fiveLedPin = 4;
const int fourLedPin = 5;
const int threeLedPin = 6;
const int twoLedPin = 7;
const int oneLedPin = 8;

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = HIGH;         // current state of the button
int lastButtonState = LOW;     // previous state of the button
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers


void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  for(int i = 3; i <= oneLedPin; i++) {
    pinMode(i, OUTPUT);
  }

  // initialize serial communication:
  Serial.begin(9600);
}


void loop() {
  // read the pushbutton input pin:
 int reading = digitalRead(buttonPin);
 if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      Serial.println("A3");
       
        }
        // compare the buttonState to its previous state
        if (buttonState != lastButtonState) {
          // if the state has changed, increment the counter
          if (buttonState == HIGH) {
            // if the current state is HIGH then the button
            // wend from off to on:
            buttonPushCounter++;
          if(buttonPushCounter > 4){
            digitalWrite(fiveLedPin, HIGH);
            for(int cycleOff = 5; cycleOff < 9; cycleOff++){
              digitalWrite(cycleOff, LOW);
            }
            if(buttonPushCounter > 9){
              digitalWrite(tenLedPin, HIGH);
            }
          }
          for(int thisLed = (buttonPushCounter % 5); thisLed  > 0; thisLed--){
            Serial.println(thisLed);
            digitalWrite(9-thisLed, HIGH);
          }
            Serial.println("on");
            Serial.print("number of button pushes:  ");
            Serial.println(buttonPushCounter);
          } else {
            // if the current state is LOW then the button
            // wend from on to off:
            Serial.println("off");
          }
          // Delay a little bit to avoid bouncing
          delay(50);
        }
        // save the current state as the last state,
        //for next time through the loop
        lastButtonState = buttonState;
    }
  } 
}      
      
      









