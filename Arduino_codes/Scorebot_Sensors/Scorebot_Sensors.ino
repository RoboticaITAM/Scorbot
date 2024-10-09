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


}

void loop() {
  /*ledcWrite(PWM_Base, 255);
  digitalWrite(DIR_Base, 0);
  ledcWrite(PWM_Hombro, 255);
  digitalWrite(DIR_Hombro, 1);*/
  //ledcWrite(PWM_Codo, 255);
  digitalWrite(DIR_Codo, 1);
  Ticks_encoders();
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
