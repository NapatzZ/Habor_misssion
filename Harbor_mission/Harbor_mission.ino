#include <popx2.h>
#include <ADS7828.h>
#define S0 A0
#define S1 A1
#define S2 A2
#define S3 A3
#define numsensorsF 8
#define DEBUG_MODE 0 

int _MaxF[numsensorsF] = { 980 , 980 , 981 ,	978 ,	982 ,	981 ,	980 ,	984 };
int _MinF[numsensorsF] = { 330 ,	331 ,	540 ,	355 ,	516 ,	543 ,	443 ,	804	 };
int sensor_values[numsensorsF];
int last_value = 0;
ADS7828 adc;

void Motors(int m1, int m2) {
  motor(1, m1);
  motor(2, m2);
}

void ReadFornt() {
  for (byte i = 0; i < numsensorsF; i++) {
    Serial.print(readF(i));
    Serial.print('\t');
  }
  Serial.println();
}

int readF(int analog_CH) {
  int val = 0;
  if (analog_CH < 8) {
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    int muxChannel[8][3] = { { 1, 1, 1 }, { 1, 1, 0 }, { 1, 0, 1 }, { 1, 0, 0 }, { 0, 1, 1 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } };
    digitalWrite(A3, muxChannel[analog_CH][0]);
    digitalWrite(A2, muxChannel[analog_CH][1]);
    digitalWrite(A1, muxChannel[analog_CH][2]);
    val = analogRead(A0);
  }
  return val;
}
void ReadLine() {
  for (int i = 0; i < numsensorsF; i++) {
    sensor_values[i] = map(readF(i), _MaxF[i], _MinF[i], 0, 1000);
    if (sensor_values[i] > 500) sensor_values[i] = 1000;
    else if (sensor_values[i] < 300) sensor_values[i] = 0;
  }
}
void Readfd() {
  while (true) {
    ReadLine();
    for (byte i = 0; i < 8; i++) {
      Serial.print(sensor_values[i]); Serial.print('\t');
    }
    Serial.println();
  }
}
int Position_front() {
  int on_line = 0;
  unsigned long avg = 0;
  unsigned int sum = 0;
  unsigned long value = 0;
  ReadLine();
  for (int i = 0; i < numsensorsF; i++) {
    value = sensor_values[i];
    if (value > 200) on_line = 1;
    if (value > 50) {
      avg += value * (i * 1000);
      sum += value;
    }
  }
  if (on_line == 0) {
    if (last_value < ((numsensorsF - 1) * 1000) / 2) return 0;
    else return (numsensorsF - 1) * 1000;
  }
  else last_value = avg / sum;
  return last_value;
}

void PID(int BaseSpeed, float Kp, float Ki, float Kd) {
  int integral = 0;
  int output = 0;
  int last_error = 0;
  int leftmotor;
  int rightmotor;
  int cal_M;
  while (1) {
    int error = Position_front() - 3500;
    if (sensor_values[0] > 500 && sensor_values[7] > 500) {
      Motors(100, 100);
      delay(15);
      AO();
      break;
    }
    output = (error * Kp) + ((error - last_error) * Kd);
    last_error = error;
    leftmotor = BaseSpeed + output;
    rightmotor = BaseSpeed - output;
    leftmotor = constrain(leftmotor, -100, 100);
    rightmotor = constrain(rightmotor, -100, 100);
    Motors(leftmotor, rightmotor);
  }
}



void setup() {
  Wire.begin();
  Wire.setClock(1000000L);
  adc.begin(0);
  Serial.begin(9600);
  OK();
  motor(100,50);
  delay(300);
  PID(100, 0.006, 0, 0.5);
  _TL();
  PID(100, 0.05, 0, 0.5);
  delay(15);
  PID(100, 0.05, 0, 0.5);
  _TR();
  PID(100,0.045, 0, 0.5);
  delay(10);
  PID(100, 0.045, 0,0.5);
  delay(15);
  PID(100, 0.045, 0,0.5);
   PID(100, 0.045, 0,0.5);
   delay(10);
  _TR();
  PID(100, 0.045, 0,0.5);

  
}

void loop() {   
  // ReadFornt();
   
}