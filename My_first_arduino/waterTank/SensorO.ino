#include <Arduino.h>

const int trigPin = 9;
const int echoPin = 10;

void SensorO_setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void SensorO_measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin to calculate the distance
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}

void SensorO_loop() {
  SensorO_measureDistance();
  delay(500);
}
