#include <PinChangeInt.h>
#define TRIGGER_AVD  3
#define ECHO_AVD 4
int a=0;
int b=0;
int t0;
int now,begintime=0;

void setup(){
  Serial.begin(9600);
   pinMode(5,OUTPUT);
   pinMode(TRIGGER_AVD, OUTPUT);
   pinMode(ECHO_AVD, INPUT);
   PCintPort::attachInterrupt(ECHO_AVD,&quicfunc0, CHANGE);
   Serial.print("test CAPTEURS");

}

void loop(){

 
  now = millis();
  
  if ((now - begintime) > 1000) {
    Serial.println("Envoi d'une trame");
    digitalWrite(TRIGGER_AVD, HIGH);
    delayMicroseconds(10);                // il y a un trigger toute les secondes
    digitalWrite(TRIGGER_AVD, LOW); //pour tout les capteurs
   



  
  
 // Serial.print("distance :  ");
//      Serial.println( a * 340 / 20000);
  begintime = millis();
  }
  


}


void quicfunc0() {
  if (digitalRead(ECHO_AVD) == LOW) {
  a = millis() - t0;
     Serial.print( "temps a l'état haut: ");
      Serial.println( a);
  }

  if ( digitalRead(ECHO_AVD) == HIGH) {
   b = millis() - t0;
   //Serial.print("b "); temps a l'état bas
   //Serial.println( b );
  }
  t0 = millis();
}

