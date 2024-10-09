#include <PS4Controller.h>
//enc codo 0-1000
//enc hombro 0-680
//enc base 0-1640
//-------920
#define Enc_Base_A 25
#define Enc_Base_B 26
#define Limit_Base 23
#define PWM_Base 18
#define DIR_Base 5

#define Enc_Hombro_A 27
#define Enc_Hombro_B 14
#define Limit_Hombro 33
#define PWM_Hombro 21
#define DIR_Hombro 19

#define Enc_Codo_A 12
#define Enc_Codo_B 13
#define Limit_Codo 32
#define PWM_Codo 4
#define DIR_Codo 15

#define Base_redux 0.187920
#define Hombro_redux 0.234924

volatile long Current_Base = 0.0; 
volatile long base_pulses = 0;

volatile long Current_Hombro = 0.0; 
volatile long hombro_pulses = 0;

volatile long Current_Codo = 0.0; 
volatile long codo_pulses = 0;

const int frecuencia = 9600;
const int canal_L = 4;
const int resolucion = 8;
volatile bool flag = false;

volatile int Eje_LY = 127;
volatile int Eje_LX = 127;
volatile int Eje_RY = 127;
volatile int Eje_RX = 127;
volatile float degres = 0;
volatile float Base_deg = 0;
volatile float Hombro_deg = 0;


void setup() {
  pinMode(Limit_Base, INPUT_PULLUP);
  pinMode(Enc_Base_A, INPUT_PULLUP);
  pinMode(Enc_Base_B, INPUT_PULLUP);

  pinMode(Limit_Hombro, INPUT_PULLUP);
  pinMode(Enc_Hombro_A, INPUT_PULLUP);
  pinMode(Enc_Hombro_B, INPUT_PULLUP);

  pinMode(Limit_Codo, INPUT_PULLUP);
  pinMode(Enc_Codo_A, INPUT_PULLUP);
  pinMode(Enc_Codo_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(Enc_Base_A), ticksBase, RISING);
  attachInterrupt(digitalPinToInterrupt(Enc_Hombro_A), ticksHombro, RISING);
  attachInterrupt(digitalPinToInterrupt(Enc_Codo_A), ticksCodo, RISING);

  pinMode(PWM_Base, OUTPUT);
  pinMode(PWM_Hombro, OUTPUT);
  pinMode(PWM_Codo, OUTPUT);
  
  pinMode(DIR_Base, OUTPUT);
  pinMode(DIR_Hombro, OUTPUT);
  pinMode(DIR_Codo, OUTPUT);

  Serial.begin(115200);

  ledcAttach(PWM_Base, frecuencia, resolucion);
  ledcAttach(PWM_Hombro, frecuencia, resolucion);
  ledcAttach(PWM_Codo, frecuencia, resolucion);

  PS4.begin();
  Serial.println("Ready.");

}

void loop() {
  Serial.print("Deg_Codo: ");
  Serial.print(degres);
  Serial.print("\t");  
  Ticks_encoders();
  if (PS4.isConnected()) {
    getAxisData();
    setPower(Eje_LX,PWM_Base,DIR_Base);
    setPower(Eje_LY,PWM_Hombro,DIR_Hombro);
    setPower(Eje_RY,PWM_Codo,DIR_Codo);
    delay(10);
  }
  Base_deg = Current_Base * Base_redux;
  Hombro_deg = Current_Hombro * Hombro_redux;
  degres = Current_Codo;
  
  //Ticks_encoders();
  delay(100);
}

void ticksBase() {
  flag = true;
  if (digitalRead(Enc_Base_B) == HIGH) { 
    base_pulses++;
  }
  else { 
    base_pulses--;
  }
  Current_Base = base_pulses;
}
void ticksHombro() {
  if (digitalRead(Enc_Hombro_B) == HIGH) { 
    hombro_pulses++;
  } 
  else { 
    hombro_pulses--;
  }
  Current_Hombro = hombro_pulses;
}
void ticksCodo() {
  if (digitalRead(Enc_Codo_B) == HIGH) { 
    codo_pulses++;
  } 
  else { 
    codo_pulses--;
  }
  Current_Codo = codo_pulses;
}
/*
void raw_Sensors(int enc_A, int enc_B, int limit){
  Serial.print("\tData\t");
  Serial.print(digitalRead(enc_A));
  Serial.print("\t");
  Serial.print(digitalRead(enc_B));
  Serial.print("\t");
  Serial.println(digitalRead(limit));
}*/

void Ticks_encoders(){
  //Serial.print("\tData\t");
  Serial.print(" Base: ");
  Serial.print(Current_Base);
  Serial.print("\tHombro: ");
  Serial.print(Current_Hombro);
  Serial.print("\tCodo: ");
  Serial.println(Current_Codo); 
}

void setPower(int Speed, int motorPWM, int motorDIR){
  int velAux = map(Speed,0,255,-255,255);
  ledcWrite(motorPWM, abs(velAux));
  if (Speed>127){
    digitalWrite(motorDIR, 1);
  }
  else if (Speed<=127){
    digitalWrite(motorDIR, 0);
  }
}
void getAxisData(){
  Eje_LY = PS4.LStickY();         
  Eje_LY = map(Eje_LY, -127, 127, 0, 255);
  Eje_LX = PS4.LStickX();         
  Eje_LX = map(Eje_LX, -127, 127, 0, 255); 
  Eje_RY = PS4.RStickY();         
  Eje_RY = map(Eje_RY, -127, 127, 0, 255); 
  Eje_RX = PS4.RStickX();         
  Eje_RX = map(Eje_RX, -127, 127, 0, 255);      
}