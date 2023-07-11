const int trigPin = 9;
const int echoPin = 10;
const int num_samples = 10;

void init_ultrasonics() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  return distance; // Return the measured distance
}

float get_average_distance(int num_samples) {
  float average = 0;
  for (int i = 0; i < num_samples; i++) {
    average += measureDistance();
    delay(100); // Delay for 100ms between measurements
  }
   Serial.print("Distance: ");
  Serial.print(average / num_samples);
  Serial.println(" cm");
  return average / num_samples;
}
