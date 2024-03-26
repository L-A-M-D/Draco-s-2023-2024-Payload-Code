#include <MS5607.h>
#include <Wire.h>
#include <Servo.h>

Servo myservo;
const int ledPin = 13;  // Pin connected to the LED
int pos = 0;            // variable to store the servo position
char count;
char Mode;
void GroundControl();
char FlightMode();
bool RSOapprovement = false;
double Release_altitude = 400;
float P_val, T_val, H_val;

//MS5xxx sensor(&Wire);
MS5607 P_Sens;


void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);  // Initialize serial communication
  myservo.attach(9);   // attaches the servo on pin 9 to the servo object
  /*if (sensor.connect()>0){
    Serial.println("No me conecto boster...");
    delay(500);
    setup();
  }*/

  if (!P_Sens.begin()) {
    Serial.println("Error in Communicating with sensor, check your connections!");
  } else {
    Serial.println("MS5607 initialization successful!");
  }
}

void loop() {
  if (Serial.available() > 0) {  // If data is available to read from XCTU XBee
    count = Serial.read();       // Read the incoming byte
    if (count == 'G') {
      GroundControl();
    } else if (count == 'F') {
      FlightMode();
    }
    digitalWrite(ledPin, HIGH);  // Turn on the LED
    delay(500);                  // Wait for 1 second
    digitalWrite(ledPin, LOW);   // Turn off the LED
  }
}

char FlightMode() {
  Serial.print("\nEntered Flight Mode; Press R when RSO gives permission");
  do {
    if (Serial.available() > 0) {
      Mode = Serial.read();
      if (Mode == 'R') {
        while (true) {
          if (P_Sens.readDigitalValue()) {
            H_val = (P_Sens.getAltitude() * 3.28) - 76.9894;  //Conversion from meter to ft and Sea level to AGL
          } else {
            Serial.println("Error in reading digital value in sensor!");
          }
          Serial.print("Altitude    :  ");
          Serial.print(H_val);
          Serial.println(" feet AGL");
          delay(100);
          if(H_val == Release_altitude){
            Serial.println("Retention Open. Payload Deployed");
            break;
          }
        }
      }
    }
  } while (true);
}

void GroundControl() {
  Serial.print("\nEntered Ground Control Mode: 0 to Open, 1 to Close and C to Exit");
  do {
    if (Serial.available() > 0) {  // If data is available to read from XCTU XBee
      Mode = Serial.read();        // Read the incoming byte
      switch (Mode) {
        case '0':
          {
            for (pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees
              // in steps of 1 degree
              myservo.write(pos);  // tell servo to go to position in variable 'pos'
              delay(15);           // waits 15 ms for the servo to reach the position
            }
            Serial.print("\nRetention Open");
            break;  // Break to exit the switch statement
          }
        case '1':
          {
            for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
              // in steps of 1 degree
              myservo.write(pos);  // tell servo to go to position in variable 'pos'
              delay(15);
            }
            Serial.print("\nRetention Close");
            break;  // Break to exit the switch statement
          }
      }
      // Toggle the LED
      digitalWrite(ledPin, HIGH);  // Turn on the LED
      delay(1000);                 // Wait for 1 second
      digitalWrite(ledPin, LOW);   // Turn off the LED
    }
  } while (Mode != 'C');
  Mode = '0';
  Serial.print("\nExited Ground Control");
  return;
}
