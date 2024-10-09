/*#define lPwm 7
#define lDir 6

#define rPwm 8
#define rDir 9*/

#define MLenc_A 25
#define MLenc_B 26
#define Limit 32
#define mPwm 13

float N = 360.0;
volatile long L_pulses = 0;
volatile unsigned CurrentTime = 0;
volatile unsigned PreviousTime = 0;
double DeltaTime = 0;
float rps_L;
volatile long CurrentPulsesL = 0.0; 
volatile long PreviousPulsesL = 0.0; 
volatile long DeltaPulsesL = 0; 

const int frecuencia = 9600;
const int canal_L = 4;
const int resolucion = 8;

void setup() {
  // put your setup code here, to run once:
  /*pinMode(lDir, OUTPUT);
  pinMode(lPwm, OUTPUT);
  pinMode(rDir, OUTPUT);
  pinMode(rPwm, OUTPUT);
*/
  pinMode(mPwm, OUTPUT);
  

  pinMode(Limit, INPUT_PULLUP);
  pinMode(MLenc_A, INPUT_PULLUP);
  pinMode(MLenc_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(MLenc_A), encoderL, RISING);
  Serial.begin(115200);
  ledcAttach(mPwm, frecuencia, resolucion);
}

void loop() {
 // speeds();
  // put your main code here, to run repeatedly:
  /*analogWrite(rPwm, 255);
  digitalWrite(rDir, 0);
  analogWrite(lPwm, 255);
  digitalWrite(lDir, 0);
*/
  ledcWrite(mPwm, 255);

  Serial.print("\tData\t");
  Serial.print(rps_L);
  Serial.print("\t");
  Serial.print(L_pulses);
  Serial.print("\t");
  Serial.print(digitalRead(MLenc_A));
  Serial.print("\t");
  Serial.print(digitalRead(MLenc_B));
  Serial.print("\t");
  Serial.println(digitalRead(Limit)); 
  // if (digitalRead(Limit) == 0){
  //   L_pulses = 0;
  // }

  delay(100);
}

void encoderL() {
  if (digitalRead(MLenc_B) == HIGH) {  // si B es HIGH, sentido horario
    L_pulses++;                        // incrementa POSICION en 1
  } else {                             // si B es LOW, sentido anti horario
    L_pulses--;                        // decrementa POSICION en 1
  }

  CurrentPulsesL = L_pulses;
}

void speeds() {
  CurrentTime = micros();
  //Se divide entre 100,000 para tener la lectura cada 0.1 segundos
  DeltaTime = (double)(CurrentTime - PreviousTime) / 100000.0;

  DeltaPulsesL = CurrentPulsesL - PreviousPulsesL;
    rps_L = (float)(DeltaPulsesL*N)/60;


  if (DeltaTime > 0.1){
    PreviousTime = CurrentTime;
    PreviousPulsesL = CurrentPulsesL;
  }
}
