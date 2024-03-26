// Define the pin for the motor
const int motorPin = 9; // PWM compatible pin for the motor

void setup() {
  // Initialize Serial communication at 9600 baud rate
  Serial.begin(9600);

  // Set the motor pin as an output
  pinMode(motorPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming byte
    int pwmValue = Serial.read();
    
    // Output the PWM signal to control the motor speed
    analogWrite(motorPin, pwmValue);

    // Print the received PWM value
    //Serial.print("Received PWM Value: ");
    //Serial.println(pwmValue);
  }
}

