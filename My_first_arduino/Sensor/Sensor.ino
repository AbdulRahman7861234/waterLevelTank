const int trigPin = 9;
const int echoPin = 10;
const int pump1Pin = 0;
const int pump2Pin = 1;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pump1Pin, OUTPUT);
  pinMode(pump2Pin, OUTPUT);
  digitalWrite(pump1Pin, HIGH);
  digitalWrite(pump2Pin, HIGH);
}

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);

  return distance;
}

void loop() {
  if (Serial.available()) {
    char input = Serial.read();

    if (input == 'd') {
      while (true) {
        float distance = measureDistance();
        delay(500);

        if (distance > 40) {
          digitalWrite(pump1Pin, LOW); // Turn pump1Pin on
          digitalWrite(pump2Pin, HIGH); // Turn pump2Pin off
        } else if (distance < 40) {
          digitalWrite(pump1Pin, HIGH); // Turn pump1Pin off
          digitalWrite(pump2Pin, LOW); // Turn pump2Pin on
        } else {
          digitalWrite(pump1Pin, HIGH); // Turn pump1Pin off
          digitalWrite(pump2Pin, HIGH); // Turn pump2Pin off
          break;
        }
      }
    }
  }
}
