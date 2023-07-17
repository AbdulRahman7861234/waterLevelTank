const int trigPin = 9;
const int echoPin = 11;
const int bufferSize = 10;

int buffer[bufferSize];
int writeIndex = 0;
int readIndex = 0;

void Ultrasonic_pins() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

float get_readings() {
  float distance = measureDistance();
  put(distance);
  
  if (writeIndex == 0 && readIndex != 0) {
    float average = get_average_distance();
    Serial.print("Average: ");
    Serial.println(average);
    return average;
  }
  
  delay(100); // Delay for 1 second between measurements
}

void init_Ultrasonics() {
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
  
  return distance;
}

void put(float item) {
  buffer[writeIndex] = item;
  writeIndex = (writeIndex + 1) % bufferSize;
  if (writeIndex == readIndex) {
    readIndex = (readIndex + 1) % bufferSize;
  }
}

float get_average_distance() {
  float sum = 0;
  int count = 0;
  
  int currentIndex = readIndex;
  while (currentIndex != writeIndex) {
    sum += buffer[currentIndex];
    count++;
    currentIndex = (currentIndex + 1) % bufferSize;
  }
  
  return sum / count;
}
