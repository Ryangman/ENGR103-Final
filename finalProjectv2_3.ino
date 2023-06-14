#include <Adafruit_CircuitPlayground.h>


const int CAP_THRESHOLD = 500;
volatile bool switchFlag = 0;
volatile bool patternComplete = 0;
int switchPin = 7;
int capValues[6];
int colorPattern[4];
int inputPattern[4];
int inputLEDSeries[4];

int score;
int errorCount = 0;
int levelCounter = 1;
float midi[127];
int A_four = 440; 

void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();

  Serial.begin(9600);
  while(!Serial);

  generateMIDI();
  randomSeed(0);

  attachInterrupt(digitalPinToInterrupt(switchPin), checkSwitch, CHANGE);
}


void loop() {
  // put your main code here, to run repeatedly:
  while(levelCounter < 6){

    if(switchFlag){
      levelCounter = 1;
      errorCount = 0;
      
      switchFlag = !switchFlag;
    }
    Serial.print("Level: ");
    Serial.println(levelCounter);

    patternGenerator();
    patternAnimation();

    // Clear any lagging LEDs before input
    CircuitPlayground.clearPixels();
    
    //Pause between Allowing Inputs
    delay(250);
   
    for(int i = 0; i < 10; i++){
      CircuitPlayground.setPixelColor(i,255,255,255);
      
    }
    


    Serial.print("Correct Pattern: ");
    for(int i = 0; i < sizeof(colorPattern)/sizeof(int); i++){
      Serial.print(colorPattern[i]);
    }

    //Read and ennumerate input pattern
    capacitanceReader();
  
    Serial.println();
    Serial.print("Input Pattern: ");
    for(int i = 0; i < sizeof(inputPattern)/sizeof(int); i++){
      Serial.print(inputPattern[i]);
    }

  
    
    //Compares Patterns and adjusts score
    patternCompare();

    delay(2500);
  }
  
  scoreSystem();
  delay(25000);

}

void checkSwitch() {
  switchFlag = 1;
}

void patternGenerator(){
  for(int i = 0; i < sizeof(colorPattern)/sizeof(int); i++){
    colorPattern[i] = random(1,7);
    // Serial.print(colorPattern[i]);
    
    delay(5);
  }
}

void patternAnimation(){
  for(int i = 0; i < sizeof(colorPattern)/sizeof(int); i++){
    
    CircuitPlayground.clearPixels();
    delay(100); // Prevents repeated numbers from not blinking
    
    int ledPath;
    // Translates LED order to correct Digital Pin
    switch(colorPattern[i]){
      case 1:
        ledPath = 6;
        break;
      case 2:
        ledPath = 8;
        break;
      case 3:
        ledPath = 9;
        break;
      case 4:
        ledPath = 0;
        break;
      case 5:
        ledPath = 1;
        break;
      case 6:
        ledPath = 3;
        break;
      case 7:
        ledPath = 4;
        break;
    }
    
    int levelsRemaining =  5-levelCounter;
    //Powers Correct LED
    CircuitPlayground.setPixelColor(ledPath,119, 56, 207);
    CircuitPlayground.playTone(midi[60], 50);
    delay(levelsRemaining * 100);
  }
}
void capacitanceReader(){
  CircuitPlayground.clearPixels();
  int inputCounter = 0;
  int inputLED = 0;
  int currentPin = 0;
  while(inputCounter < 4){
  
      for(int i = 0; i < 6; i++){
        
        // translates for loop vars to cap sensor digital pin
        switch(i){
          case 0:
            currentPin = 6;
            
            break;
          case 1:
            currentPin = 9;
            
            break;
          case 2:
            currentPin = 10;
          
            break;
          case 3:
            currentPin = 3;
            
            break;
          case 4:
            currentPin = 2;
            
            break;
          case 5:
            currentPin = 0;
            
            break;
          case 6:
            currentPin = 1;
           
            break;
          }
        capValues[i] = CircuitPlayground.readCap(currentPin);
        
        delay(5);
      }
      // Creates Array of touched cap sensors
      
      for(int i = 1; i < 7; i++){
        int ledPath = 0;
        if(capValues[i-1] > CAP_THRESHOLD){
          CircuitPlayground.clearPixels();
          inputPattern[inputCounter] = i;
          inputCounter++;

          switch(i){
            case 1:
              ledPath = 6;
              break;
            case 2:
              ledPath = 8;
              break;
            case 3:
              ledPath = 9;
              break;
            case 4:
              ledPath = 0;
              break;
            case 5:
              ledPath = 1;
              break;
            case 6:
              ledPath = 3;
              break;
            case 7:
              ledPath = 4;
              break;
          }
          CircuitPlayground.setPixelColor(ledPath,119, 56, 207 );

          CircuitPlayground.playTone(midi[60], 50);
          delay(250);
        }
      }
  }

  
}

void patternCompare(){
  // Finds total Error
  int initErrorCount = errorCount;
  for(int i = 0; i < sizeof(colorPattern)/sizeof(int); ++i){
    // Serial.print("Color Pattern: ");
    // Serial.println(colorPattern[i]);
    // Serial.print("Input Pattern: ");
    // Serial.println(inputPattern[i]);
    
    if(colorPattern[i] != inputPattern[i]){
      errorCount++;
      Serial.print("Error at Index");
      Serial.println(i);
    }
  }
  if(errorCount == initErrorCount){
    for(int i = 0; i < 10; i++){
      CircuitPlayground.setPixelColor(i,0,255,0);
      delay(25);
    }
  } else{
    for(int i = 0; i < 10; i++){
      CircuitPlayground.setPixelColor(i,255,0,0);
      delay(25);
    }
  }
  levelCounter++;
}

void scoreSystem(){
  CircuitPlayground.clearPixels();
  score = map(errorCount, 0, 15, 10, 1);
  Serial.println(score);

  for(int i = 0; i < score; i++){
      CircuitPlayground.setPixelColor(i,0,map(i, 1, 10, 55,255),0);
      delay(100);

      CircuitPlayground.playTone(midi[i*10], 50);

    }

}
//Function from Week 5 Lectures
void generateMIDI(){
  for (int x = 0; x < 127; ++x)
  {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    //Serial.println(midi[x]);
  }
}