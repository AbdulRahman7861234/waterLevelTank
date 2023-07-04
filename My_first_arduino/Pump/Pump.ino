#define OFF false
#define ON true



// Pin assignment
const int relayPin = 9;

// Variable to track relay state
bool relay_1 = OFF;

void setup() {
  pinMode(relayPin, OUTPUT);  // Set relay pin as output
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  // Check if there is any input available
  if (Serial.available()) {
    // Read the input
    char input = Serial.read();

    // Process the input
    if (input == '1') {
      relay_1 = true;
      digitalWrite(relayPin, LOW);  // Turns relay_1 on
    } else if (input == '0') {
      relay_1 = false;
      digitalWrite(relayPin, HIGH);  // Turns relay_1 off
    }
  }

  // Your other code here

  // Delay between iterations
  delay(100);
}
