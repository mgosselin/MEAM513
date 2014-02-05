////////////////////////////////////////////////////////////////////////////////
// GLOBALS - STUDENTS MAY CHANGE THESE
////////////////////////////////////////////////////////////////////////////////
const byte flag_DEBUG = 1;  // debug flag (set = 1 to Debug)
const double PingCalibration = 0.016; // Calibration constant for Ping Sensor
////////////////////////////////////////////////////////////////////////////////
// GLOBALS - CHANGED BY CODE (NOT CONSTANT)
////////////////////////////////////////////////////////////////////////////////
byte flag_STOP = 0;  // Stop Logic
//
// Pin Usage (Global to allow access in setup()
//
const int pinPING = 8;    // Ping Sensor Input
const int pinMOTOR = 3;   // pin 3 supports PWM on Timer 2
const int pinIR = 10;     // IR converted to servo pulse
const int pinPOT = A0;    // to read potentiometer setting
const int pinGO = 2;      // selection PICAXE or internal distance command

////////////////////////////////////////////////////////////////////////////////
// the setup routine runs once when you press reset:
////////////////////////////////////////////////////////////////////////////////
void setup() {                
  pinMode(pinPING, INPUT);
  pinMode(pinIR, INPUT);
  pinMode(pinGO, INPUT);
  pinMode(pinMOTOR, OUTPUT);     
  analogWrite(pinMOTOR, 0);

  if (flag_DEBUG) {
    Serial.begin(9600);
    Serial.println("RW 1");
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
  static double desired_distance_cm=10.0;   // How Far We Want to Follow
  static double actual_distance_cm=10.0;    // How Far We Are Following
  word Pot_Setting = 0;              // Potentiometer Adjustment from A-to-D
  double Pot_Percentage = 0.0;       // Use floating point value to avoid type conversion issues coding
  unsigned long RunTime=0;             // How Long Has Loop Been Executing [microseconds]
  unsigned long PingTime;
  double MotorDuty = 0.0;            // return double from control code to avoid type conversion issues
  byte MotorDutyByte = 0;

  //
  // get potentiometer setting
  //
  Pot_Setting = getPot();
  Pot_Percentage = 0.09775*Pot_Setting;  // convert 0-1023 integer into 0-100 real number

  //
  // get commanded distance from PICAXE chip
  //
  if (digitalRead(pinGO)) {
    desired_distance_cm = getCommand(desired_distance_cm);
  }
  else {
    desired_distance_cm = createCommand(desired_distance_cm);
  }

  //
  // get actual distance with PING
  //
  actual_distance_cm = getActual(actual_distance_cm);

  //
  // call control function & constrain motor command to avoid excessive speed
  //
  MotorDuty = My_Control_Function(desired_distance_cm, actual_distance_cm, Pot_Percentage);
  MotorDuty = constrain(MotorDuty,0.0,100.0);
  MotorDutyByte = MotorDuty;

  //
  // deal with Stop Command
  //
  if (flag_STOP) {
    MotorDuty = 0.0;
    MotorDutyByte = 0;
  }

  //
  // send command to motor
  //
  analogWrite(pinMOTOR, MotorDuty);

  //
  // delay 0.1 seconds to slow system down and make control problem more difficult!
  //
  while ( (RunTime + 100000) > micros()) {
  }
  RunTime = micros();

  ////////////////////////////////////////////////////////////////////////////////
  // end of loop
  ////////////////////////////////////////////////////////////////////////////////
  if (flag_DEBUG) {
    Serial.println();
  }
}

////////////////////////////////////////////////////////////////////////////////
// Read potentiometer setting (returns 0 to 1023)
////////////////////////////////////////////////////////////////////////////////
word getPot() {
  word Pot_Value;
  Pot_Value = analogRead(pinPOT);
  if (flag_DEBUG) {
    Serial.print("Pot = "); 
    Serial.print(Pot_Value,DEC); 
    Serial.print(",");
  }
  return Pot_Value;
}

////////////////////////////////////////////////////////////////////////////////
// READ Commanded Distance From PICAXE Chip
////////////////////////////////////////////////////////////////////////////////
double getCommand(double D_old) {
  long IRTime;
  double D_cm;

  IRTime = pulseIn(pinIR, HIGH, 25000);

  if (flag_DEBUG) {
    Serial.print(" IR = "); 
    Serial.print(IRTime,DEC); 
    Serial.print(",");
  }

  if (IRTime == 0) {
    D_cm = D_old;  // do nothing because operation timed out = some problem with PICAXE
  }
  else if (IRTime < 2200)  {
    flag_STOP = 0;       // return to following
    D_cm = 0.01*IRTime;  // convert pulse width into following distance (100 us = 1cm)
  }
  else {
    flag_STOP = 1;    // commanded distance greater than 22 cm = STOP NOW!
    D_cm = D_old;     // keep previous distance
    if (flag_DEBUG) {
      Serial.print(" .....STOP REQUESTED.....");
    }
  }
  Serial.print("D_cm = "); 
  Serial.print(D_cm,DEC); 
  Serial.print(",");  
  return D_cm;
}

////////////////////////////////////////////////////////////////////////////////
// Read actual distance from PING sensor
////////////////////////////////////////////////////////////////////////////////
double getActual(double A_old) {
  double A_cm;
  long PingTime;

  pinMode(pinPING, OUTPUT);     
  digitalWrite(pinPING, HIGH);
  delayMicroseconds(5);
  digitalWrite(pinPING, LOW);
  pinMode(pinPING, INPUT);
  PingTime = pulseIn(pinPING, HIGH, 25000);
  if (PingTime > 0) {
    A_cm = PingCalibration*PingTime;  // approximate calibration
  }
  else {
    A_cm = A_old;    // keep last value if timeout
  }
  if (flag_DEBUG) {
    Serial.print(" Ping = "); 
    Serial.print(PingTime,DEC); 
    Serial.print(",");
    Serial.print(" A_cm = "); 
    Serial.print(A_cm,DEC); 
    Serial.print(",");
  }
  return A_cm;
}


////////////////////////////////////////////////////////////////////////////////
// generate Commanded distance internally
////////////////////////////////////////////////////////////////////////////////
double createCommand(double D_old) {
  double D_cm;

  D_cm = 10.0;

  return D_cm;
} 

////////////////////////////////////////////////////////////////////////////////
// THIS IS WHERE STUDENT CODE GOES
////////////////////////////////////////////////////////////////////////////////
double My_Control_Function(double d_command, double d_actual, double Pot) {
  double Control_Command;
  
  double dt;
  double kp;
  double ki;
  double kd;
  double error;
  static double last_error = 0;
  static double sum_error = 0;
  double fric_comp = 35;

  // put your control generating code here
  dt = 0.1;
  kp = 5;
  kd = 8;
  ki = 0;
  error = d_actual - d_command;
  sum_error += error*dt;

  Control_Command = kp*error + kd*(error-last_error) + ki*sum_error;
  if (Control_Command > 100) {
	Control_Command = 100;
	sum_error -= error;
  } else if (Control_Command < 0) {
	Control_Command = 0;
	sum_error -= error;
  }
  last_error = error;
    
  //Control_Command = Pot;

  if (flag_DEBUG) {
    Serial.print("DFIL = "); 
    Serial.print(d_filter); 
    Serial.print(",");
    Serial.print("CCMD = "); 
    Serial.print(Control_Command,DEC); 
    Serial.print(",");
  }
  return Control_Command;
}

