const int trigPin = 9;
const int echoPin = 10;
const int pump1Pin = 0;//Pump moving water into tank
const int pump2Pin = 1;// Pump moving water away from tank
int desiredReading = 0;
float offset = 0.1; // Lower and upper bounds

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
    int userInput = Serial.parseInt();
    if (Serial.read() == '\n') {
      desiredReading = userInput;
      Serial.print("Desired reading is now: ");
      Serial.println(desiredReading);
    }
  }

  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'on') {
      while (true) {
        float distance = measureDistance();
        delay(500);

        if (distance >= desiredReading - offset && distance <= desiredReading + offset) {
          digitalWrite(pump1Pin, HIGH); // Turn off pump1
          digitalWrite(pump2Pin, HIGH); // Turn off pump2
          break;
        } else if (distance > desiredReading + offset) {
          digitalWrite(pump1Pin, LOW); // Turn on pump1
          digitalWrite(pump2Pin, HIGH); // Turn off pump2
        } else if (distance < desiredReading - offset) {
          digitalWrite(pump1Pin, HIGH); // Turn off pump1
          digitalWrite(pump2Pin, LOW); // Turn on pump2
        }

        if (Serial.available()) {
          input = Serial.read();
          if (input == 'q') {
            digitalWrite(pump1Pin, HIGH); // Turn off pump1
            digitalWrite(pump2Pin, HIGH); // Turn off pump2
            break; // Exit the loop
          }
        }
      }
    }
  }
}
