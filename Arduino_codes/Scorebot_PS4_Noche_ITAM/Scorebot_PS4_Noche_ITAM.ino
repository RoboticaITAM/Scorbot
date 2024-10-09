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

#define PWM_ActA 16
#define DIR_ActA 17

#define PWM_ActB 14
#define DIR_ActB 27

#define PWM_Gripper 13
#define DIR_Gripper 12

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
volatile int Eje_IRX = 127;
volatile int Trigg_Izq = 127;
volatile int Trigg_Der = 127;
volatile int Bump_Izq = 127;
volatile int Bump_Der = 127;

volatile float degres = 0;
volatile float Base_deg = 0;
volatile float Hombro_deg = 0;


void setup() {
  pinMode(PWM_Base, OUTPUT);
  pinMode(PWM_Hombro, OUTPUT);
  pinMode(PWM_Codo, OUTPUT);
  pinMode(PWM_ActA, OUTPUT);
  pinMode(PWM_ActB, OUTPUT);
  pinMode(PWM_Gripper, OUTPUT);
  
  pinMode(DIR_Base, OUTPUT);
  pinMode(DIR_Hombro, OUTPUT);
  pinMode(DIR_Codo, OUTPUT);
  pinMode(DIR_ActA, OUTPUT);
  pinMode(DIR_ActB, OUTPUT);
  pinMode(DIR_Gripper, OUTPUT);

  Serial.begin(115200);

  ledcAttach(PWM_Base, frecuencia, resolucion);
  ledcAttach(PWM_Hombro, frecuencia, resolucion);
  ledcAttach(PWM_Codo, frecuencia, resolucion);
  ledcAttach(PWM_ActA, frecuencia, resolucion);
  ledcAttach(PWM_ActB, frecuencia, resolucion);
  ledcAttach(PWM_Gripper, frecuencia, resolucion);

  //ledcAttach(PWM_ActB, frecuencia, resolucion);

  PS4.begin();
  Serial.println("Ready.");

}

void loop() {
  int bumpers = Bump_Der-Bump_Izq;
  int triggers = Trigg_Der-Trigg_Izq;
  Serial.print("L2\t");
  Serial.print(Bump_Der+Bump_Izq);
  Serial.print("\tR2\t");
  Serial.println(Trigg_Der+Trigg_Izq);
  if (PS4.isConnected()) {
    getAxisData();
    setPower(Eje_LX,PWM_Base,DIR_Base);
    setPower(Eje_LY,PWM_Hombro,DIR_Hombro);
    setPower(Eje_RY,PWM_Codo,DIR_Codo);
    if(PS4.R1() || PS4.L1()){
      setPower(bumpers, PWM_ActA, DIR_ActA);
      setPower(bumpers, PWM_ActB, DIR_ActB);
    }
    else{
      setPower(Eje_RX, PWM_ActA, DIR_ActA);
      setPower(Eje_IRX, PWM_ActB, DIR_ActB);
    }
    

    setPower(triggers, PWM_Gripper, DIR_Gripper);
    delay(10);
  }
  delay(100);
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
  Eje_IRX = map(Eje_RX, 0, 255, 255, 0);
  Trigg_Der = PS4.R2();
  Trigg_Der = map(Trigg_Der, 0, 1, 127, 255);         
  Trigg_Izq = PS4.L2();         
  Trigg_Izq = map(Trigg_Izq, 0, 1, 0, 127);
  Bump_Der = PS4.R1();
  Bump_Der = map(Bump_Der, 0, 1, 127, 255);         
  Bump_Izq = PS4.L1();         
  Bump_Izq = map(Bump_Izq, 0, 1, 0, 127);
}

