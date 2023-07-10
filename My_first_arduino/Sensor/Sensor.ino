#define ON true
#define OFF false

typedef struct WaterLevel_t {
  bool pumpOff;
  int pumpTimer;
  float ultrasonicLevel;
  bool ultrasonicTrigTimer;
  int ultrasonicEchoPin;
} WaterLevel_t;

typedef enum WaterLevelTankStates_t {
  IDLE,
  PUMPING_EMPTY,
  PUMPING_FILL,
  DELAY,
  //LIDOTT_MEASURE,
} WaterLevelTankStates_t;

WaterLevelTankStates_t state = IDLE;
WaterLevelTankStates_t lastState = IDLE;
bool newState = true;

WaterLevel_t waterLevelTank;

int desiredReading = 60;
float offset = 0.1; // Lower and upper bounds

void setup() {
  Serial.begin(9600);
  
  init_ultrasonics();
  init_pumps();

  Serial.println("Init complete.");
}

void loop() {

  delay(500);

  static char temp[100] = {0};
  sprintf(temp, "state: %d, laststate: %d", state, lastState);
  Serial.println(temp);

  if (state != lastState) {
    newState = true;
    lastState = state;
  }

  switch (state) {
    case IDLE:
      if (newState) {
        Serial.println("IDLE");
      }

      // Take an ultrasonic measurement
      waterLevelTank.ultrasonicLevel = measureDistance();

      sprintf(temp, "Ultrasonic level: %f", waterLevelTank.ultrasonicLevel);
      Serial.println(temp);

      // Determine if the tank needs to be emptied - Go to Pumping state if required
      if (waterLevelTank.ultrasonicLevel > 30) {
        state = PUMPING_EMPTY;
      } else {
        // Otherwise, take a Lidott measurement
        state = PUMPING_FILL;
      }
      break;

    case PUMPING_EMPTY:
      if (newState) {
        Serial.println("PUMPING_EMPTY");
        pump_action(1, ON);
      }

      // Take an ultrasonic measurement
      waterLevelTank.ultrasonicLevel = measureDistance();

      // Determine if the tank needs to be emptied - Go to Pumping state if required
      if (waterLevelTank.ultrasonicLevel < 30) {
        pump_action(1, OFF);
        state = IDLE;
      }
      break;

    case PUMPING_FILL:
      if (newState) {
        Serial.println("PUMPING_FILL");
        pump_action(2, ON);
      }
      
      waterLevelTank.ultrasonicLevel = measureDistance();

      // Determine if the tank needs to be filled
      if (waterLevelTank.ultrasonicLevel < desiredReading) {
        pump_action(2,ON);
      } else {
        // Tank reached or exceeded desired reading
        pump_action(2, OFF);
        desiredReading += 10;
        state = DELAY;
        
        if (desiredReading > 70) {

          state = IDLE;
        }
      }
    case DELAY:
      if (newState){
        Serial.println("DELAY");
      } 
      //Go back to filling up tank
      state = PUMPING_FILL;
  };

  delay(5000);

  newState = false;

}
