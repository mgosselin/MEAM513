////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////
const byte flag_DEBUG = 1;  // debug flag (set = 1 to send information via Serial)
//
// Pin Usage (Global to allow access in setup()
//
const int pinMOTOR = 3;   // pin 3 supports PWM on Timer 2
const int pinIR = 10;     // IR converted to servo pulse
const int pinGO = 2;      // input selection switch position

////////////////////////////////////////////////////////////////////////////////
// the setup routine runs once when you press reset:
////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(pinIR, INPUT);
  pinMode(pinGO, INPUT);
  pinMode(pinMOTOR, OUTPUT);     
  analogWrite(pinMOTOR, 0);

  if (flag_DEBUG) {
    Serial.begin(9600);
    Serial.println("Train Leader Robot Running");
    delay(2000); // Give reader a chance to see the output.
  }
}

////////////////////////////////////////////////////////////////////////////////
// the loop routine runs over and over again until reset is pressed
////////////////////////////////////////////////////////////////////////////////
void loop() {
///////
// Declarations
///////
byte MotorDutyByte;
static double MotorDuty;

  if (digitalRead(pinGO)) {
    MotorDuty = getCommand(MotorDuty); // get speed command from PICAXE chip
  }
  else {
    MotorDuty = createCommand(MotorDuty);  // generate command internally
  }
  
//
// send command to motor
//
  MotorDutyByte = MotorDuty;
  MotorDutyByte = constrain(MotorDutyByte,0,120);
  analogWrite(pinMOTOR, MotorDutyByte);

//
// send a final line feed if debugging
//
  if (flag_DEBUG) {
     Serial.println();
  }
}
////////////////////////////////////////////////////////////////////////////////
// end of loop
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// READ Commanded Position From PICAXE Chip
//
double getCommand(double MD_old) {
  double MD;
  long IRTime;

  IRTime = pulseIn(pinIR, HIGH, 25000);

  if (flag_DEBUG) {
    Serial.print(" IRTime = "); Serial.print(IRTime,DEC);
  }

  if (IRTime == 0) {
    if (flag_DEBUG) {
      Serial.print(" IR Timeout");
    }
    MD = MD_old;  // do nothing because operation timed out = some problem with PICAXE
  }
  else if (IRTime < 100) {
    if (flag_DEBUG) {
      Serial.print(" STOP Command");
    }
    MD = 0;    // very short IR pulse --> user pressed STOP button
  }
  else {
    if (flag_DEBUG) {
      Serial.print(" Speed Command");
    }
    MD = 0.1*IRTime;  // moderate pulese --> PICAXE gives motor speed command directly
  }
  
  MD = constrain(MD,0.0,120.0);  // just in case something goes nuts
  if (flag_DEBUG) {
    Serial.print(" MD = "); Serial.print(MD,DEC);
  }
  
  return MD;
}

////////////////////////////////////////////////////////////////////////////////
// generate Commanded speed internally
//
double createCommand(double MD_old) {
  double MD;
  
  MD = MD_old + 0.01*(60.0 - MD_old);
 
 return MD;
} 
