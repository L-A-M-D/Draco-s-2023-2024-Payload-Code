// Define the pin for the potentiometer and the motor
const int potPin = A0;  // Potentiometer pin
const int motorPin = 9; // PWM compatible pin for the motor

void setup() {
  // Initialize Serial communication at 9600 baud rate
  Serial.begin(9600);
  
  // Set the motor pin as an output
  pinMode(motorPin, OUTPUT);
  pinMode(potPin, INPUT);
}

void loop() {
  // Read the value from the potentiometer
  int potValue = analogRead(potPin);

  // Map the potentiometer value (0-1023) to PWM range (0-255)
  int pwmOutput = map(potValue, 0, 1022, 0, 255);

  // Output the PWM signal to control the motor speed
  analogWrite(motorPin, pwmOutput);
  
  // Print the potentiometer value and the corresponding PWM output
  Serial.print("Potentiometer Value: ");
  Serial.print(potValue);
  Serial.print("   PWM Output: ");
  Serial.println(pwmOutput);
  
  // Small delay to avoid rapid changes in PWM
  delay(10);
}


