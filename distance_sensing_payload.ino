
#include <HCSR04.h>

UltraSonicDistanceSensor distanceSensor(5, 6);//initialisation class HCSR04 (trig pin , echo pin)
void setup() {
  
Serial.begin(9600);

}

void loop() {
  float distance = distanceSensor.measureDistanceCm(); // extracting distance measurment reading
  float distance_in = distance/2.54;
  float distance_ft = distance/30.48; //converting cm to ft
  Serial.print("the distance is: ");
  Serial.print(distance_in);
  Serial.print("in\n");
  delay(100); //sampling frequency 10Hz
  
  
  
}
