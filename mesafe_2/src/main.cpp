#include <Arduino.h>

uint32_t millisDist  = 0;

struct distanceSensor{
  float analogVal[5];

  void medianFilter(float*);
  float getFilteredValue(float*, uint8_t);
  float getDistance(float);

};

void distanceSensor::medianFilter(float* currentVal){
  float temp = 0;
  for(uint8_t i = 0; i < 5; i++){
   for(uint8_t j = 0; j < i + 1; j++){
      if(*(currentVal + i) < *(currentVal + j)){
        temp = *(currentVal + i);
        *(currentVal + i) = *(currentVal + j);
        *(currentVal + j) = temp;
      }
   }
  }
}

float distanceSensor::getFilteredValue(float* vals, uint8_t pin){
  for(uint8_t i = 0; i < 5; i++){
     *(vals + i) = analogRead(pin);    
   }
   medianFilter(vals);
   return *(vals + 2);
}

float distanceSensor::getDistance(float val){
  float distance;
  distance = (float) 8304 / (float)(val - 44);
  //if(distance < 4) distance = 3;
  if(distance > 30) distance = 31;
  return distance;
}

uint32_t millisPrev = 0;

distanceSensor *distance_1 = new distanceSensor();
distanceSensor *distance_2 = new distanceSensor();

void setup() {
 Serial.begin(9600);
}

void loop() {
  if(millis() > millisPrev + 2){
    printf("Dist1: %f\n", distance_1->getDistance(distance_1->getFilteredValue(distance_1->analogVal, 33)));
    printf("Dist2: %f\n", distance_2->getDistance(distance_2->getFilteredValue(distance_2->analogVal, 32)));
    millisPrev = millis();
  }
}