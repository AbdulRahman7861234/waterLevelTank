#define ON true
#define OFF false

typedef struct WaterLevel_t {
  bool pumpOff;
  int pumpTimer;
  int delayTimer;
  float ultrasonicLevel;
  bool ultrasonicTrigTimer;
  int ultrasonicEchoPin;
} WaterLevel_t;

typedef enum WaterLevelTankStates_t {
  IDLE,
  PUMPING_EMPTY,
  PUMPING_FILL,
  DELAY,
  LIDOTT_MEASURE,
} WaterLevelTankStates_t;

WaterLevelTankStates_t state = IDLE;
WaterLevelTankStates_t lastState = IDLE;
bool newState = true;
WaterLevelTankStates_t nextState = IDLE;

WaterLevel_t waterLevelTank;

int desiredReading = 50;
float offset = 0.1; // Lower and upper bounds

void setupTimer(WaterLevelTankStates_t a_nextState, int timer){
  state = DELAY;
  nextState = a_nextState;
  waterLevelTank.delayTimer= timer;
}


void setup() {
  Serial.begin(9600);
  
  init_Ultrasonics();
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
      waterLevelTank.ultrasonicLevel = get_readings();

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
      waterLevelTank.ultrasonicLevel = get_readings();

      // Determine if the tank needs to be emptied - Go to Pumping state if required
      if (waterLevelTank.ultrasonicLevel < 30) {
        pump_action(1, OFF);
        state = IDLE;
      } else {
      setupTimer(PUMPING_EMPTY, 5); 
      } 


      break;

    case PUMPING_FILL:
      if (newState) {
        Serial.println("PUMPING_FILL");
        pump_action(2, ON);
      }
      
      waterLevelTank.ultrasonicLevel = get_readings();

      // Determine if the tank needs to be filled
      if (waterLevelTank.ultrasonicLevel >= desiredReading){
        // Tank reached or exceeded desired reading
        pump_action(2, OFF);
        if (desiredReading > 100) {
          state = IDLE;
        } else {
          state = LIDOTT_MEASURE;
          desiredReading += 10;
        }

      } else {
        setupTimer(PUMPING_FILL, 5);
        }
      break;

    case DELAY:
      if (newState) {
        Serial.println("DELAY");
        //waterLevelTank.delayTimer = 0;
      } 
      waterLevelTank.delayTimer -= 1;

      // Delay for 5 seconds (5000ms)
      if (waterLevelTank.delayTimer == 0) {
        // Go back to filling up tank
        state = nextState;
      }
      break;
      case LIDOTT_MEASURE:
        setupTimer(LIDOTT_MEASURE, 5000);


    break;
  }

  delay(1);
  newState = false;
}
