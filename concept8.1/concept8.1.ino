#include <Adafruit_CircuitPlayground.h>

float X, Y, Z, totalAccel;
const int ACCEL_THRESHOLD = 15;
int colorInit = 18;
int colorUpdated;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  CircuitPlayground.begin();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
}

void loop() {
  // put your main code here, to run repeatedly:
  colorWheel(colorInit);
  X, Y, Z = 0;
  for(int i=0; i < 10; i++){
      X += CircuitPlayground.motionX();
      Y += CircuitPlayground.motionY();
      Z += CircuitPlayground.motionZ();
      delay(1);
  }
  X /= 10;
  Y /= 10;
  Z /= 10;

  // Serial.println(X);
  // Serial.println(Y);
  // Serial.println(Z);
  
  totalAccel = sqrt(X*X + Y*Y + Z*Z);

  if(totalAccel >= ACCEL_THRESHOLD){
    Serial.println("SHAKEN");
    colorUpdated = map(totalAccel, 15, 100, 96, 255);
    colorWheel(colorUpdated);
  }
  // Serial.println(totalAccel);
  

  delay(100);
}

void colorWheel(int R){
  for(int i = 0; i < 10; i++){
    CircuitPlayground.setPixelColor(i, R ,0 ,0);
  }
}
