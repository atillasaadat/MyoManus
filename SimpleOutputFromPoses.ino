#include <MyoController.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <math.h>
Servo myServo;

#define LED 2
#define FIST_PIN 4
#define WAVEIN_PIN 5
#define WAVEOUT_PIN 6
#define FINGERSSPREAD_PIN 7
#define DOUBLETAP_PIN 8

MyoController myo = MyoController();

void setup() {
  Serial.begin(9600);
  pinMode(FIST_PIN, OUTPUT);
  pinMode(WAVEIN_PIN, OUTPUT);
  pinMode(WAVEOUT_PIN, OUTPUT);
  pinMode(FINGERSSPREAD_PIN, OUTPUT);
  pinMode(DOUBLETAP_PIN, OUTPUT);
  pinMode(A5,INPUT);
  myServo.attach(9);
  myServo.write(90);
  myo.initMyo();
}

double Thermister(int RawADC) {
  double Temp;
  Temp = log(((10240000/RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;           // Convert Kelvin to Celcius
  return Temp;
}
void printTemp(void) {
  double fTemp;
  double temp = Thermister(analogRead(A5));  // Read sensor
  fTemp = (temp * 1.8) + 32.0;    // Convert to USA
  Serial.println(fTemp);
  digitalWrite(LED,LOW);
  if (fTemp > 40) {
    digitalWrite(LED,HIGH);
  }
}

void loop()
{
   printTemp();
   myo.updatePose();
   switch ( myo.getCurrentPose() ) {
    case rest:
      digitalWrite(FIST_PIN,LOW); 
      digitalWrite(WAVEIN_PIN,LOW);
      digitalWrite(WAVEOUT_PIN,LOW);
      digitalWrite(FINGERSSPREAD_PIN,LOW);
      digitalWrite(DOUBLETAP_PIN,LOW);
      myServo.write(110);
      break;
    case fist:
      digitalWrite(FIST_PIN,HIGH);
      myServo.write(0);

      break;
    case waveIn:
      digitalWrite(WAVEIN_PIN,HIGH);
      break;
    case waveOut:
      digitalWrite(WAVEOUT_PIN,HIGH);
      break;
    case fingersSpread:
      digitalWrite(FINGERSSPREAD_PIN,HIGH);
      myServo.write(110);
      break;
    case doubleTap:
      digitalWrite(DOUBLETAP_PIN,HIGH);
      break;
   } 
   delay(100);
}
