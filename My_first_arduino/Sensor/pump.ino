#define ON true
#define OFF false

const int pump1Pin = 0;//Pump moving water into tank
const int pump2Pin = 1;// Pump moving water away from tank

void init_pumps(void) {
  pinMode(pump1Pin, OUTPUT);
  pinMode(pump2Pin, OUTPUT);
  digitalWrite(pump1Pin, HIGH);
  digitalWrite(pump2Pin, HIGH);
}

void pump_action(uint8_t pump, bool state) {
  if (state == ON) {
    digitalWrite(pump - 1, LOW);
  } else if (state == OFF) {
    digitalWrite(pump - 1, HIGH);
  }
}
