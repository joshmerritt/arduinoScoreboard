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




#define NOTE_E6  1319 
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_D7  2349
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_G7  3136
#define NOTE_A7  3520

#define melodyPin 5



// --------CONSTANTS (won't change)---------------

unsigned long timeOutLength = 1800000;
int buttonPins[]= {7,8,9,10,11};

boolean overTimeBlink = 1;

unsigned long minuteDuration = 60000;  // 60000 = number of millisecs in one minute

unsigned long buttonInterval = 10; // number of millisecs between button readings

const int winningScore = 15;
const int gamePoint = 14;
int board[25]={0,1,2,9,10,11,12,13,14,15,17,18,19,20,25,26,27,34,35,36,37,38,39,40,42};
int sand[9]={3,16,8,21,22,23,28,41,33};
int puck[9]={4,5,6,7,24,29,30,31,32};

int redDigits[][13] = {
{5,6,7,8,9,10,11,12,13,14,15,16,-1},
{7,8,9,10,11,-1,-1,-1,-1,-1,-1,-1,-1},
{5,6,7,8,9,17,14,13,12,11,15,-1,-1},
{5,6,7,8,9,10,11,12,13,17,-1,-1,-1},
{7,8,9,10,11,5,16,15,17,-1,-1,-1,-1},
{7,6,5,16,15,17,9,10,11,12,13,-1,-1},
{7,6,5,16,15,14,13,12,11,10,9,17,-1},
{5,6,7,8,9,10,11,-1,-1,-1,-1,-1,-1},
{5,6,7,8,9,10,11,12,13,14,15,16,17},
{5,6,7,8,9,10,11,17,16,15,-1,-1,-1}, 
{0,1,2,3,4,-1,-1,-1,-1,-1,-1,-1,-1}
  };

int blueDigits[][13] = {
{30,31,32,33,34,35,36,37,38,39,40,41,-1},
{32,33,34,35,36,-1,-1,-1,-1,-1,-1,-1,-1},
{30,31,32,33,34,42,39,38,37,36,40,-1,-1},
{30,31,32,33,34,35,36,37,38,42,-1,-1,-1},
{32,33,34,35,36,42,41,40,30,-1,-1,-1,-1},
{32,31,30,41,40,42,34,35,36,37,38,-1,-1},
{32,31,30,42,41,40,39,38,37,36,35,34,-1},
{36,35,34,33,32,31,30,-1,-1,-1,-1,-1,-1},
{30,31,32,33,34,35,36,37,38,39,40,41,42},
{42,41,40,36,35,34,33,32,31,30,-1,-1,-1},
{25,26,27,28,29,-1,-1,-1,-1,-1,-1,-1,-1}
};

int blueLEDs[] = {25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42};

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
int buttons[5];
int prevButtons[] = {0,0,0,0,0};
unsigned long prevButtonMillis[] = {0,0,0,0,0};
boolean gameStart = 0;
boolean timeWarning = 0;
boolean oneMinuteWarning = 0;
boolean victorySong = 1;
int soundOff;
boolean resetPushed = 0;


//========================================

void setup() {
  digitalWrite(12,HIGH);
  pinMode(12,OUTPUT);
  digitalWrite(12,HIGH);
  Serial.begin(9600);
  Serial.println("Scoreboard Booting Up");  // so we know what sketch is running
  pinMode(6,OUTPUT);
  digitalWrite(6,LOW);
  pinMode(5,OUTPUT);
  digitalWrite(5,LOW);

  for (byte i=0; i<5; i++) {
    pinMode(buttonPins[i], INPUT);
    digitalWrite(buttonPins[i], HIGH);
  }
  
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  for(int m = 0; m < 20; m++) {
    fill_rainbow(leds, NUM_LEDS, random8(0,255), NUM_LEDS);
    FastLED.show();
    delay(100);
  }
  for(int k = 0; k < NUM_LEDS; k++) {
    leds[k] = CRGB::Black;
  }
  FastLED.show();
//  slidePuck(CRGB::Purple);
    boolean prediction = 0;
  if(analogRead(A0)%2 == 0){
    prediction = 1;
  }
  Serial.println("Game prediction:");
  Serial.println(prediction);
  gameAnimation(prediction);
  FastLED.show();
  for(int k = 0; k < NUM_LEDS; k++) {
    leds[k] = CRGB::Black;
  }
  for(int j = 0; j < 13; j++) {
    leds[redDigits[0][j]] = CRGB::Red;
    leds[blueDigits[0][j]] = CRGB::RoyalBlue;
  }
  FastLED.show();

  
}

//========================================

void loop() {
  currentMillis = millis();   // capture the latest value of millis()
  if(gameStart) {
//    gameAnimation(1);
  }
/*  if(elapsedGameTime > 30) {
    for(int j = 0; j < 100; j++) {
    int winner = random8(0,1);
    gameAnimation(winner);
    updateScores(false);
    
    readButtons();
    }
    
  }*/
  if((redScore >= winningScore) || (blueScore >= winningScore)){
    if(redScore > blueScore) {
        updateScores(false);
        FastLED.show();
        winningCycle(0);
    }else{
        updateScores(false);
        FastLED.show();
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
  if(victorySong) {
    delay(100);
    sing();
    victorySong = 0;
  }
  for(int m = 0; m < 500; m++) {
    confetti(color);
    FastLED.show();
    FastLED.delay(4);
    readButtons();
    if(redScore < 15 && blueScore < 15) {
      break;
    }
  }
  showScore();
  FastLED.delay(1000);
  
}



void readButtons() {
  
  soundOff = analogRead(A5);
  for(int i = 7; i < 12; i++) {
    buttons[i-7] = digitalRead(i);
//    Serial.println(buttons[i-8]);
    Serial.println("test");
  }
  for(int j = 0; j < 5; j++) {
    if(buttons[j] != prevButtons[j]){
      prevButtonMillis[j] = millis();
    }
    if(prevButtonMillis[j] - currentMillis > buttonInterval){
      if(buttons[j] != prevButtons[j]){
        if(buttons[j] == 1){
          switch(j) {
            case 0:
              blueScore++;
              if(soundOff == 0) {
                digitalWrite(5,HIGH);
                FastLED.delay(125);
                digitalWrite(5,LOW);
              }
              resetPushed = false;
            break;
            case 1:
              if(blueScore > 0){
                blueScore--;
                if(blueScore == (winningScore-1)){
                  updateScores(true);
                  updateTimeDisplay();
                }
              if(soundOff == 0) {
                digitalWrite(6,HIGH);
                FastLED.delay(100);
                digitalWrite(6,LOW);
              }
              }
              resetPushed = false;
            break;
            case 2:
              if(redScore > 0){
                redScore--;
                  if(redScore == (winningScore-1)){
                  updateScores(true);
                  updateTimeDisplay();
                }
                if(soundOff == 0) {
                  digitalWrite(6,HIGH);
                  FastLED.delay(100);
                  digitalWrite(6,LOW);
                }
              }
              resetPushed = false;
            break;
            case 3:
              redScore++;
              if(soundOff == 0) {
                digitalWrite(5,HIGH);
                FastLED.delay(125);
                digitalWrite(5,LOW);
              }
              resetPushed = false;
            break;
            case 4:
              if(resetPushed) {
                digitalWrite(12,LOW);
              } else {
                resetPushed = true;
              }
              break;
        }
      }
      }
      prevButtons[j] = buttons[j];
    }
}
}

void updateTimeDisplay() {
    turnOffTime();
    if(elapsedGameTime > 5 && elapsedGameTime < 10) {
        for(int i = 18; i < 19; i++){
        leds[i] = CRGB::Green;
        }
    }
    if(elapsedGameTime > 9 && elapsedGameTime < 15) {
        for(int i = 18; i < 20; i++){
        leds[i] = CRGB::Green;        }
    }
    switch(elapsedGameTime) {
      case 5:
        leds[18] = CRGB::Green;
        break;
      case 6:
        leds[18] = CRGB::Yellow;
        break;
      case 7:
        leds[18] = CRGB::Orange;
        break;
      case 8:
        leds[18] = CRGB::DarkOrange;
        break;
      case 9:
        leds[18] = CRGB::Red;
        break;                                
      case 10:
        for(int i = 18; i < 20; i++){
        leds[i] = CRGB::Green;        }
        break;
      case 11:
        leds[18] = CRGB::Green;
        leds[19] = CRGB::Yellow;
        break;
      case 12:
        leds[18] = CRGB::Green;
        leds[19] = CRGB::Orange;
        break;
      case 13:
        leds[18] = CRGB::Green;
        leds[19] = CRGB::DarkOrange;
        break;
      case 14:
        leds[18] = CRGB::Green;
        leds[19] = CRGB::Red;
        break;
      case 15:
        for(int i = 18; i < 21; i++){
          leds[i] = CRGB::Green;
        }       
        break;
      case 16:
        for(int i = 18; i < 22; i++){
          leds[i] = CRGB::Yellow;
        }
        break;
      case 17:
        for(int i = 18; i < 23; i++){
          leds[i] = CRGB::Orange;
        }
        break;
      case 18:
        for(int i = 18; i < 24; i++){
          leds[i] = CRGB::Red;
        }
        break;
      case 19:
        turnOffTime();
        for(int i = 18; i < 25; i++){
          leds[i] = CRGB::Purple;
        }
        break;
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
/*      Serial.println("Blue Score:");
      Serial.println(blueScore);
      Serial.println("Blue Display:");
      Serial.println(blueDisplay);
      Serial.println(ones);
*/      if(blueScore == 20) {
        leds[30] = CRGB::RoyalBlue;
        leds[31] = CRGB::RoyalBlue;
        leds[32] = CRGB::RoyalBlue;
        leds[33] = CRGB::RoyalBlue;
        leds[34] = CRGB::RoyalBlue;
        leds[42] = CRGB::RoyalBlue;
        leds[40] = CRGB::RoyalBlue;
        leds[41] = CRGB::RoyalBlue;
        leds[39] = CRGB::RoyalBlue;
        leds[38] = CRGB::RoyalBlue;
      } else {
        for(int i = 0; i < 13; i++) {
          leds[blueDigits[ones][i]] = CRGB::RoyalBlue;
        }
      if (blueScore > 9) {
          leds[25] = CRGB::RoyalBlue;
          leds[26] = CRGB::RoyalBlue;
          leds[27] = CRGB::RoyalBlue;
          leds[28] = CRGB::RoyalBlue;
          leds[29] = CRGB::RoyalBlue;
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
    Serial.println("Elapsed Game Time:");
    Serial.println(elapsedGameTime);
    previousTimeUpdate = currentMillis;
  }
    turnOffTime();
    switch(elapsedGameTime) {
      case 5:
        leds[18] = CRGB::Green;
        break;
      case 6:
        leds[18] = CRGB::Yellow;
        break;
      case 7:
        leds[18] = CRGB::Orange;
        break;
      case 8:
        leds[18] = CRGB::DarkOrange;
        break;
      case 9:
        leds[18] = CRGB::Red;
        break;                                
      case 10:
        for(int i = 18; i < 20; i++){
        leds[i] = CRGB::Green;        }
        break;
      case 11:
        leds[18] = CRGB::Green; 
        leds[19] = CRGB::Yellow;
        break;
      case 12:
        leds[18] = CRGB::Green; 
        leds[19] = CRGB::Orange;
        break;
      case 13:
        leds[18] = CRGB::Green; 
        leds[19] = CRGB::DarkOrange;
        break;
      case 14:
        leds[18] = CRGB::Green; 
        leds[19] = CRGB::Red;
        break;
      case 15:
        for(int i = 18; i < 21; i++){
        leds[i] = CRGB::Green;
        }
        break;
      case 16:
        for(int i = 18; i < 22; i++){
          leds[i] = CRGB::Yellow;
        }
        break;
      case 17:
        for(int i = 18; i < 23; i++){
          leds[i] = CRGB::Orange;
        }
        break;
      case 18:
        for(int i = 18; i < 24; i++){
          leds[i] = CRGB::Red;
        }
        break;
      case 19:
        {
        for(int i = 18; i < 25; i++){
          leds[i] = CRGB::Purple;
        }
        if(!oneMinuteWarning){
          oneMinuteWarning = 1;
          if(soundOff==0) {
            digitalWrite(6,HIGH);
            delay(600);
            digitalWrite(6, LOW);
          }
        }
        }
        break;
     case 20:
        {
        if(!timeWarning){  
            timeWarning = 1;
            if(soundOff==0) {
                digitalWrite(5,HIGH);
                delay(300);
                digitalWrite(5,LOW);
                delay(600);
                digitalWrite(5,HIGH);
                delay(600);
                digitalWrite(5,LOW);
                delay(600);
                digitalWrite(6,HIGH);
                digitalWrite(5,HIGH);
                delay(150);
                digitalWrite(6, LOW);
                delay(150);
                digitalWrite(6,HIGH);
                delay(150);
                digitalWrite(6, LOW);
                delay(150);
                digitalWrite(6,HIGH);
                delay(150);
                digitalWrite(6, LOW);
                delay(150);
                digitalWrite(6,HIGH);
                delay(150);
                digitalWrite(6, LOW);
                delay(150);
                digitalWrite(6,HIGH);
                delay(150);
                digitalWrite(6, LOW);
                delay(150);
                digitalWrite(6,HIGH);
                delay(150);
                digitalWrite(6, LOW);
                delay(150);
                digitalWrite(6,HIGH);
                delay(150);
                digitalWrite(6, LOW);
                delay(150);
                digitalWrite(5,LOW);
            }      

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

void showScore() {
  int redOnes = redScore % 10;
  int blueOnes = blueScore % 10;
  turnOffRed();
  turnOffBlue();
  for(int i = 0; i < 18 ; i++){
       leds[blueLEDs[i]].fadeLightBy(64);
  }
      for(int i = 0; i < 13; i++) {
          leds[redDigits[redOnes][i]] = CRGB::Red;
          leds[blueDigits[blueOnes][i]] = CRGB::RoyalBlue;
        }
      if (redScore > 9) {
          leds[0] = CRGB::Red;
          leds[1] = CRGB::Red;
          leds[2] = CRGB::Red;
          leds[3] = CRGB::Red;
          leds[4] = CRGB::Red;
      }
      if (blueScore > 9) {
          leds[25] = CRGB::RoyalBlue;
          leds[26] = CRGB::RoyalBlue;
          leds[27] = CRGB::RoyalBlue;
          leds[28] = CRGB::RoyalBlue;
          leds[29] = CRGB::RoyalBlue;
      }
  FastLED.show();
}

void gameAnimation(boolean blueWinner) {
  int board[25]={0,1,2,9,10,11,12,13,14,15,17,18,19,20,25,26,27,34,35,36,37,38,39,40,42};
  int sand[9]={3,16,8,21,22,23,28,41,33};
  int puck[9]={4,5,6,7,24,29,30,31,32};
  CRGB winnerColor, loserColor;
  if(blueWinner){
    winnerColor = CRGB::RoyalBlue;
    loserColor = CRGB::Red;  
  } else {
    winnerColor = CRGB::Red;
    loserColor = CRGB::RoyalBlue;
  }
  for(int i = 0; i < 25; i++){
       leds[board[i]]=CRGB::Brown;
  }
  FastLED.show();
  slidePuck(loserColor);
  FastLED.delay(1000);
  slidePuck(winnerColor);
  leds[32]=loserColor;
  FastLED.show();
  FastLED.delay(500);
  leds[32]=CRGB::Black;
  FastLED.delay(3000);
}

void scorePoints(CRGB color, int points) {
    fill_solid(leds, NUM_LEDS, CRGB::Yellow);
  
}

void slidePuck(CRGB color) {
  int board[]={0,1,2,9,10,11,12,13,14,15,17,18,19,20,25,26,27,34,35,36,37,38,39,40,42,3,16,8,21,22,28,41,33};
  int puckSpeed = 1000;
  leds[4] = color;
  for(int i = 0; i < 33; i++){
       leds[board[i]]=CRGB::Yellow;
  }
  FastLED.show();
  FastLED.delay(puckSpeed/4);
  leds[4]=CRGB::Black;
  leds[5]=color;
  FastLED.show();
  FastLED.delay(puckSpeed/4);
  leds[5]=CRGB::Black;
  leds[6]=color;
  FastLED.show();
  FastLED.delay(puckSpeed/3);
  leds[6]=CRGB::Black;
  leds[7]=color;
  FastLED.show();
  FastLED.delay(puckSpeed/3);
  leds[7]=CRGB::Black;
  leds[24]=color;
  FastLED.show();
  FastLED.delay(puckSpeed/2);
  leds[24]=CRGB::Black;  
  leds[29]=color;
  FastLED.show();
  FastLED.delay(puckSpeed/2);
  leds[29]=CRGB::Black;  
  leds[30]=color;
  FastLED.show();
  FastLED.delay(puckSpeed);
  leds[30]=CRGB::Black;  
  leds[31]=color;
  FastLED.show();  
}


void fillSingleColor(int TheArray[], int n, CRGB color) {
   for(int i = 0; i < n; i++){
       TheArray[i]=color;
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


void sing(){      
   int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7, 
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0, 

  NOTE_C7, 0, 0, NOTE_G6, 
  0, 0, NOTE_E6, 0, 
  0, NOTE_A6, 0, NOTE_B6, 
  0, NOTE_AS6, NOTE_A6, 0, 

  NOTE_G6, NOTE_E7, NOTE_G7, 
  NOTE_A7, 0, NOTE_F7, NOTE_G7, 
  0, NOTE_E7, 0,NOTE_C7, 
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6, 
  0, 0, NOTE_E6, 0, 
  0, NOTE_A6, 0, NOTE_B6, 
  0, NOTE_AS6, NOTE_A6, 0, 

  NOTE_G6, NOTE_E7, NOTE_G7, 
  NOTE_A7, 0, NOTE_F7, NOTE_G7, 
  0, NOTE_E7, 0,NOTE_C7, 
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12, 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12, 

  12, 12, 12, 12,
  12, 12, 12, 12, 
  12, 12, 12, 12, 
  12, 12, 12, 12, 

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
   
   // iterate over the notes of the melody:
     int size = sizeof(melody) / sizeof(int);
     for (int thisNote = 0; thisNote < size; thisNote++) {

       // to calculate the note duration, take one second
       // divided by the note type.
       //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
       int noteDuration = 1000/tempo[thisNote];

       buzz(melodyPin, melody[thisNote],noteDuration);

       // to distinguish the notes, set a minimum time between them.
       // the note's duration + 30% seems to work well:
       int pauseBetweenNotes = noteDuration * 1.30;
       delay(pauseBetweenNotes);

       // stop the tone playing:
       buzz(melodyPin, 0,noteDuration);

    
  }
}

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13,HIGH);
  long delayValue = 1000000/frequency/2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to 
  //// get the total number of cycles to produce
  for (long i=0; i < numCycles; i++){ // for the calculated length of time...
    digitalWrite(targetPin,HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin,LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13,LOW);

}
