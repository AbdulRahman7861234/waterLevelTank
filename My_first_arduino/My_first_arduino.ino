void setup() {
   //put your setup code here, to run once
  Serial.begin(9600);
  pinMode(D9, OUTPUT);
}
bool once = false;
int counter = 1;

void loop() {
   //put your main code here, to run repeatedly
  digitalWrite(D9,HIGH);
  delay(1000);
  digitalWrite(D9,LOW);
  delay(1000);

  if (once == false) {
    Serial.print("Hello Abdul\n\r");
    once = true;
  }

  while (counter < 100) {
    if (counter % 3 == 0 && counter % 5 == 0) {
      Serial.print("FizzBuzz\n\r");
    } else if (counter % 3 == 0) {
      Serial.print("Fizz\n\r");
    } else if (counter % 5 == 0) {
      Serial.print("Buzz\n\r");
    } else {
      Serial.println(counter);
    }

    counter += 1;
  }
}

