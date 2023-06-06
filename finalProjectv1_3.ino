#include <Adafruit_CircuitPlayground.h>


const int CAP_THRESHOLD = 500;
volatile bool switchFlag = 0;
volatile bool patternComplete = 0;
int switchPin = 7;
int capValues[6];
int colorPattern[3];
int inputPattern[3];
int score = 10;
int errorCount = 0;

void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();

  Serial.begin(9600);

  while(!Serial);

  attachInterrupt(digitalPinToInterrupt(switchPin), checkSwitch, CHANGE);
}


void loop() {
  // put your main code here, to run repeatedly:
  patternGenerator(2);
  patternAnimation();

  // Clear any lagging LEDs before input
  CircuitPlayground.clearPixels();
  
  //Pause between Allowing Inputs
  delay(1000);
  Serial.println("Allowing Inputs");


  Serial.print("Correct Pattern: ");
  for(int i = 0; i < sizeof(colorPattern)/sizeof(int); i++){
    Serial.print(colorPattern[i]);
  }

  //Read and ennumerate input pattern
  capacitanceReader();

    Serial.print("Correct Pattern 2: ");
  for(int i = 0; i < sizeof(colorPattern)/sizeof(int); i++){
    Serial.print(colorPattern[i]);
  }
  
  
  Serial.println();
  Serial.print("Input Pattern: ");
  for(int i = 0; i < sizeof(inputPattern)/sizeof(int); i++){
    Serial.print(inputPattern[i]);
  }

    Serial.print("Correct Pattern 3: ");
  for(int i = 0; i < sizeof(colorPattern)/sizeof(int); i++){
    Serial.print(colorPattern[i]);
  }

  Serial.println();
  Serial.println("Checking Accuracy");
  
  //Compares Patterns and adjusts score
  patternCompare();

  // Serial.print("SCORE: ");
  // Serial.println(score);

  delay(10000);
}

void checkSwitch() {
  switchFlag = 1;
}

void patternGenerator(int levelCount){
  for(int i = 0; i < sizeof(colorPattern)/sizeof(int); i++){
    colorPattern[i] = random(1,7);
    // Serial.print(colorPattern[i]);
    
    delay(5);
  }
}

void patternAnimation(){
  for(int i = 0; i < sizeof(colorPattern)/sizeof(int); i++){
    
    CircuitPlayground.clearPixels();
    delay(500); // Prevents repeated numbers from not blinking
    
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
    //Powers Correct LED
    CircuitPlayground.setPixelColor(ledPath,119, 56, 207);
    delay(500);
  }
}
void capacitanceReader(){
  int inputCounter = 0;

  while(inputCounter < 3){
  
      for(int i = 0; i < 7; i++){
        int currentPin = 0;
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

      for(int i = 1; i < 8; i++){
        if(capValues[i-1] > CAP_THRESHOLD){
          inputPattern[inputCounter] = i;
          Serial.print(inputPattern[inputCounter]);
          inputCounter++;
          delay(250);
        }
      }
  }
  // Serial.println(" INPUT ARRAY FULL");
  
}

void patternCompare(){
  // Finds total Error
  errorCount = 0;
  Serial.println(colorPattern[0]);
  Serial.println(sizeof(colorPattern) / sizeof(int));
  for(int i = 0; i < sizeof(colorPattern)/sizeof(int); ++i){
    Serial.print("Color Pattern: ");
    Serial.println(colorPattern[i]);
    Serial.print("Input Pattern: ");
    Serial.println(inputPattern[i]);
    
    // if(colorPattern[i] != inputPattern[i]){
    //   errorCount++;
    //   Serial.print("Error at Index");
    //   Serial.println(i);
    // }
  }
}