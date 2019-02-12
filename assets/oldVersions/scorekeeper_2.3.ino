
// ----------LIBRARIES--------------

#include <FastLED.h>

#define LED_PIN     3
#define NUM_LEDS    43
#define BRIGHTNESS  100
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif



// --------CONSTANTS (won't change)---------------


int buttonPins[]= {8,9,10,11};

boolean overTimeBlink = 1;

unsigned long minuteDuration = 6000;  // number of millisecs in one minute

unsigned long buttonInterval = 10; // number of millisecs between button readings

const int winningScore = 15;
const int gamePoint = 14;

int redDigits[][13] = {
{5,6,7,8,9,10,11,12,13,14,15,16,-1},
{7,8,9,10,11,-1,-1,-1,-1,-1,-1,-1,-1},
{5,6,7,8,9,17,14,13,12,11,-1,-1,-1},
{5,6,7,8,9,10,11,12,13,17,15,-1,-1},
{7,8,9,10,11,5,16,15,17,-1,-1,-1,-1},
{7,6,5,16,15,17,9,10,11,12,13,-1,-1},
{7,6,5,16,15,14,13,12,11,10,9,17,-1},
{5,6,7,8,9,10,11,-1,-1,-1,-1,-1,-1},
{5,6,7,8,9,10,11,12,13,14,15,16,17},
{5,6,7,8,9,10,11,17,16,15,-1,-1,-1}, 
/*1x*/  {0,1,2,3,4,-1,-1,-1,-1,-1,-1,-1,-1}
  };

int blueDigits[][13] = {
{30,31,32,33,34,35,36,37,38,39,40,41,-1},
{32,33,34,35,36,-1,-1,-1,-1,-1,-1,-1,-1},
{30,31,32,33,34,42,39,38,37,36,-1,-1,-1},
{30,31,32,33,34,35,36,37,38,40,42,-1,-1},
{32,33,34,35,36,42,41,40,30,-1,-1,-1,-1},
{32,31,30,41,40,42,34,35,36,37,38,-1,-1},
{32,31,30,42,41,40,39,38,37,36,35,34,-1},
{36,35,34,33,32,31,30,-1,-1,-1,-1,-1,-1},
{30,31,32,33,34,35,36,37,38,39,40,41,42},
{42,41,40,36,35,34,33,32,31,30,-1,-1,-1},
{25,26,27,28,29,-1,-1,-1,-1,-1,-1,-1,-1}
};

//------------ VARIABLES (will change)---------------------


unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long previousButtonMillis = 0; // time when button press last checked
unsigned long previousTimeUpdate = 0;
unsigned long gameStartMillis = 0;

int redDisplay = 0;
int blueDisplay = 0;
int redScore = 0;
int blueScore = 0;
int displayGameTime = 0;
int elapsedGameTime = 0;
int buttons[4];
int prevButtons[] = {0,0,0,0};
unsigned long prevButtonMillis[] = {0,0,0,0};
 


//========================================

void setup() {
  digitalWrite(12,HIGH);
  pinMode(12,OUTPUT);
  Serial.begin(9600);
  Serial.println("Scoreboard Booting Up");  // so we know what sketch is running
  pinMode(7,OUTPUT);
  digitalWrite(7,LOW);

  for (byte i=0; i<4; i++) {
    pinMode(buttonPins[i], INPUT);
    digitalWrite(buttonPins[i], HIGH);
  }
  
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  
  for(int m = 0; m < 30; m++) {
    fill_rainbow(leds, NUM_LEDS, random8(0,255), NUM_LEDS);
    FastLED.show();
    delay(100);
  }
  for(int k = 0; k < NUM_LEDS; k++) {
    leds[k] = CRGB::Black;
  }
  FastLED.show();
  for(int j = 0; j < 13; j++) {
    leds[redDigits[0][j]] = CRGB::Red;
    leds[blueDigits[0][j]] = CRGB::Blue;
  }
  FastLED.show();
  Serial.println("Game Start:");
  
}

//========================================

void loop() {
  currentMillis = millis();   // capture the latest value of millis()
  if((redScore >= winningScore) || (blueScore >= winningScore)){
    if(redScore > blueScore) {
        winningCycle(0);
    }else{
        winningCycle(1);
    }
  }else {
  updateElapsedGameTime();
  updateScores(false);
  FastLED.show(); 
  }              
  readButtons();
}

//========================================

void winningCycle(boolean blueWinner) {
  int color;
  if(blueWinner){
    color = 155;
  } else {
    color = 0;
  }
  for(int m = 0; m < 100; m++) {
    confetti(color);
    FastLED.show();
    FastLED.delay(4);
  }
  showScore();
  delay(10000);
  
}

void showScore() {
  int redOnes = redScore % 10;
  int blueOnes = blueScore % 10;
  turnOffRed();
  turnOffBlue();
      for(int i = 0; i < 13; i++) {
          leds[redDigits[redOnes][i]] = CRGB::Red;
          leds[blueDigits[blueOnes][i]] = CRGB::Blue;
        }
      if (redScore > 9) {
          leds[0] = CRGB::Red;
          leds[1] = CRGB::Red;
          leds[2] = CRGB::Red;
          leds[3] = CRGB::Red;
          leds[4] = CRGB::Red;
      }
      if (blueScore > 9) {
          leds[25] = CRGB::Blue;
          leds[26] = CRGB::Blue;
          leds[27] = CRGB::Blue;
          leds[28] = CRGB::Blue;
          leds[29] = CRGB::Blue;
      }
  FastLED.show();
}

void gameAnimation(boolean blueWinner) {
  int board[]={0,1,2,9,10,11,12,13,14,15,17,18,19,20,25,26,27,34,35,36,37,38,39,40,42};
  int sand[]={3,16,8,21,22,23,28,41,33};
  int puck[]={4,5,6,7,24,29,30,31,32};
  CRGB winnerColor, loserColor;
  if(blueWinner){
    winnerColor = CRGB::Blue;
    loserColor = CRGB::Red;  
  } else {
    winnerColor = CRGB::Red;
    loserColor = CRGB::Blue;
  }
  fillSingleColor(board,25,CRGB::Tan);
  fillSingleColor(sand,9,CRGB::Black);
  fillSingleColor(puck,9,CRGB::Black);
  FastLED.show();

}

void slidePuck(CRGB color) {
  int puckSpeed = 100;
  leds[4] = color;
  FastLED.show();
  FastLED.delay(puckSpeed/4);
  leds[4]=CRGB::Black;
  leds[5]=color;
  FastLED.show();
  FastLED.delay(puckSpeed/4);
  leds[4]=CRGB::Black;
  leds[5]=CRGB::Black;
  leds[6]=color;
  FastLED.show();
  FastLED.delay(puckSpeed/3);
  leds[4]=CRGB::Black;
  leds[5]=CRGB::Black;
  leds[6]=CRGB::Black;
  leds[7]=color;
  FastLED.show();
  FastLED.delay(puckSpeed/3);
    
  
}

void fillSingleColor(int TheArray[], int n, CRGB color)
{
   for(int i = 0; i < n; i++){
       leds[i]=color;
   }
}

void readButtons() {
  for(int i = 8; i < 12; i++) {
    buttons[i-8] = digitalRead(i);
    Serial.println(buttons[i-8]);
  }
  for(int j = 0; j < 4; j++) {
    if(buttons[j] != prevButtons[j]){
      prevButtonMillis[j] = millis();
    }
    if(prevButtonMillis[j] - currentMillis > buttonInterval){
      if(buttons[j] != prevButtons[j]){
        if(buttons[j] == 1){
          switch(j) {
            case 0:
              if(buttons[3] == 1) {
                
              }
              blueScore++;
            break;
            case 1:
              if(blueScore > 0){
                blueScore--;
                if(blueScore == (winningScore-1)){
                  updateScores(true);
                }
              }
            break;
            case 2:
              if(redScore > 0){
                redScore--;
                  if(redScore == (winningScore-1)){
                  updateScores(true);
                }
              }
            break;
            case 3:
              redScore++;
            break;
          }
        }
      }
      }
      prevButtons[j] = buttons[j];
    }
}


void updateScores(boolean scoreReset) {
    if(redScore != redDisplay || scoreReset) {
      turnOffRed();
      int ones = redScore % 10;
//      Serial.println("Red Score:");
//      Serial.println(redScore);
//      Serial.println("Red Display:");
//      Serial.println(redDisplay);
//      Serial.println(ones);
      if(redScore == 20) {
        leds[5] = CRGB::Red;
        leds[6] = CRGB::Red;
        leds[7] = CRGB::Red;
        leds[8] = CRGB::Red;
        leds[9] = CRGB::Red;
        leds[13] = CRGB::Red;
        leds[14] = CRGB::Red;
        leds[15] = CRGB::Red;
        leds[16] = CRGB::Red;
        leds[17] = CRGB::Red;
      } else {
        for(int i = 0; i < 13; i++) {
          leds[redDigits[ones][i]] = CRGB::Red;
        }
      if (redScore > 9) {
          leds[0] = CRGB::Red;
          leds[1] = CRGB::Red;
          leds[2] = CRGB::Red;
          leds[3] = CRGB::Red;
          leds[4] = CRGB::Red;
      }
      }
      redDisplay = redScore;
    }
  if(blueScore != blueDisplay || scoreReset) {
      turnOffBlue();
      int ones = blueScore % 10;
      Serial.println("Blue Score:");
      Serial.println(blueScore);
      Serial.println("Blue Display:");
      Serial.println(blueDisplay);
      Serial.println(ones);
      if(blueScore == 20) {
        leds[30] = CRGB::Blue;
        leds[31] = CRGB::Blue;
        leds[32] = CRGB::Blue;
        leds[33] = CRGB::Blue;
        leds[34] = CRGB::Blue;
        leds[42] = CRGB::Blue;
        leds[40] = CRGB::Blue;
        leds[41] = CRGB::Blue;
        leds[39] = CRGB::Blue;
        leds[38] = CRGB::Blue;
      } else {
        for(int i = 0; i < 13; i++) {
          leds[blueDigits[ones][i]] = CRGB::Blue;
        }
      if (blueScore > 9) {
          leds[25] = CRGB::Blue;
          leds[26] = CRGB::Blue;
          leds[27] = CRGB::Blue;
          leds[28] = CRGB::Blue;
          leds[29] = CRGB::Blue;
      }
      }
      blueDisplay = blueScore;
    }

  
  }


void turnOffRed() {
  for(int i = 0; i < 18; i++) {
    leds[i] = CRGB::Black;
  }
}

void turnOffBlue() {
  for(int i = 25; i < 43; i++) {
    leds[i] = CRGB::Black;
  }
}

void turnOffTime() {
  for(int i = 18; i < 25; i++) {
    leds[i] = CRGB::Black;
  }
}



void updateElapsedGameTime() {
  if (currentMillis - previousTimeUpdate >= minuteDuration) {
    elapsedGameTime++;
//    Serial.println("Elapsed Game Time:");
//    Serial.println(elapsedGameTime);
    previousTimeUpdate = currentMillis;
  }
  if (((elapsedGameTime % 5 == 0) && elapsedGameTime != displayGameTime) || elapsedGameTime >15) {
    turnOffTime();
    switch(elapsedGameTime) {
      case 5:
        for(int i = 18; i < 19; i++){
        leds[i] = CRGB::Green;
        }
        break;
      case 10:
        for(int i = 18; i < 20; i++){
        leds[i] = CRGB::Green;        }
        break;
      case 15:
        for(int i = 18; i < 21; i++){
        leds[i] = CRGB::Green;
        }
        break;
      case 16:
        {
        fill_gradient(leds,18,CHSV(HUE_GREEN,255,255),21,CHSV(40, 255, 255));
        }
        break;
      case 17:
        {
        fill_gradient(leds,18,CHSV(HUE_GREEN,255,255),22,CHSV(30, 255, 255));
        }
        break;
      case 18:
        {
        fill_gradient(leds,18,CHSV(HUE_GREEN,255,255),23,CHSV(20, 255, 255));
        }
        break;
      case 19:
        {
        fill_gradient(leds,18,CHSV(HUE_GREEN,255,255),24,CHSV(10, 255, 255));
        digitalWrite(7,HIGH);
        delay(100);
        digitalWrite(7,LOW);
        }
        break;
     case 20:
        {
        digitalWrite(7,HIGH);
        delay(100);
        digitalWrite(7,LOW);
        delay(200);
        digitalWrite(7,HIGH);
        delay(100);
        digitalWrite(7,LOW);
        delay(200);
        digitalWrite(7,HIGH);
        delay(500);
        digitalWrite(7,LOW);
        
        }
    }
  }
  if(elapsedGameTime >= 20) {
    if(overTimeBlink) {
      for(int i = 18; i < 25; i++){
        leds[i] = CRGB::Red;
      }
    } else {
      for(int i = 18; i < 25; i++){
        leds[i] = CRGB::Green;
      }
    }
    overTimeBlink = !overTimeBlink;
    FastLED.delay(100);
    
  }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, random8(0,255), 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti(uint8_t gHue) 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(27), 200, 255);
  addGlitter(30);
}



