#include <MS5607.h>
#include <Wire.h>
MS5607 P_Sens;
float H_val;
float prev_H_val = 0; // Previous altitude reading
unsigned long prev_time = 0; // Previous time reading

// Moving average filter parameters
const int numReadings = 20; // Number of readings to average
float readings[numReadings]; // Array to store readings
int currentIndex = 0; // Index for the current reading
float total = 0; // Total sum of readings

// Control system parameters
const float setpoint = -2.0; // Desired velocity setpoint (ft/s)
const float Kp = 12.0; // Proportional gain
const int pwmPin = 9; // PWM pin

void setup() {
  Serial.begin(9600);
  P_Sens.begin();
  prev_time = millis(); // Initialize prev_time

  // Initialize the array with 0
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
  pinMode(pwmPin, OUTPUT); // Set pwmPin as an output
}

void loop() {
  unsigned long current_time = millis(); // Current time
  unsigned long time_diff = current_time - prev_time; // Time elapsed since last reading
  if (P_Sens.readDigitalValue()) {
    H_val = (P_Sens.getAltitude()-9) * 3.38;  // Conversion from meters to feet and Sea level to AGL

    // Update moving average
    total = total - readings[currentIndex];
    readings[currentIndex] = H_val;
    total = total + readings[currentIndex];
    currentIndex = (currentIndex + 1) % numReadings;
    float filtered_H_val = total / numReadings; // Filtered altitude reading

    // Calculate velocity
    float velocity = (filtered_H_val - prev_H_val) / (time_diff / 1000.0); // Velocity calculation (ft/s)

    // Calculate control output (PWM)
    float error = setpoint - velocity; // Calculate error
    float control_output = constrain(Kp * error, 144, 255); // Apply proportional control and constrain within PWM range

    // Output PWM signal
    analogWrite(pwmPin, control_output); // Output control_output PWM signal to pwmPin

    // Print velocity and control output
    Serial.print("\nVelocity: ");
    Serial.print(velocity);
    Serial.println(" ft/s");

    Serial.print("Control Output (PWM): ");
    Serial.println(control_output);

    // Update previous altitude and time readings
    prev_H_val = filtered_H_val;
    prev_time = current_time;
  }
  delay(100); // Delay for 0.1 second
}
