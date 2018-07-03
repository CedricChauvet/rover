#include <Servo.h>

boolean modeMouvement;
Servo servoBras;
Servo servoAvantBras;

Servo roues;
Servo servoDirection;
Servo servoMotrice;

#define COMM_ROUE     11//channel 1
#define COMM_GACHETTE 12//channel 2
#define TRIGGER_AVG  5
#define ECHO_AVG 6
#define TRIGGER_AVD  3
#define ECHO_AVD 4
#define TRIGGER_AR  1
#define ECHO_AR 2


int valSwitch,valSwitchAvant, valGachette, valRoue;
  boolean mode=false;

void setup()
{
  Serial.begin(9600);
  pinMode(COMM_ROUE, INPUT);
  pinMode(COMM_GACHETTE, INPUT);
  pinMode(TRIGGER_AVG, OUTPUT);
  pinMode(ECHO_AVG, INPUT);
  pinMode(TRIGGER_AVD, OUTPUT);
  pinMode(ECHO_AVD, INPUT);
  
  
  modeMouvement=false; 
  servoDirection.attach(10);
  servoMotrice.attach(9);
  servoBras.attach(8);
  servoAvantBras.attach(7);
}

long getDistance(int trigger, int echo)
{
  long duration, distance;

 digitalWrite(trigger, LOW);
  delayMicroseconds(2); // Added this line
 digitalWrite(trigger, HIGH);
  delayMicroseconds(10); // Added this line
 digitalWrite(trigger, LOW);

  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;
 
return distance; 
  
}

void loop()
{
  
  
  valSwitch=pulseIn(COMM_ROUE,HIGH, 20000);
  if ((valSwitchAvant==0 && valSwitch!=0) )
  modeMouvement=!modeMouvement;
  


  valGachette=pulseIn(COMM_GACHETTE,HIGH);
  valRoue=pulseIn(COMM_ROUE,HIGH);

  int valRoueServo=map(valRoue,850,1770,10,170);
  int valGachetteServo=map(valGachette,820,1770,10,170);
/*
//Serial.print(valSwitchAvant);
//Serial.print("\t");
Serial.print(valGachette);
Serial.print("\t");
Serial.print(valRoue);
Serial.print("\n");
*/
Serial.print(valRoueServo);
Serial.print("\t");
Serial.print(valGachette);
Serial.print("\t");
Serial.print(valGachetteServo);
Serial.print("\n");


/*int d = getDistance(TRIGGER_AVD, ECHO_AVD);
delay(300);*/

  if (modeMouvement)
  {
    Serial.println("mode mouvement");
    
    servoDirection.write(valRoueServo);
    
    if (170-valGachetteServo>90) //on va en avant
    {
      //if (getDistance(TRIGGER_AVG,ECHO_AVG)>10)
        
        servoMotrice.write(valGachetteServo);
           
    }
  //  Serial.println("on passe en mode roues");
  }
  else
  {
        Serial.println("mode bras");

    servoBras.write(valRoueServo);
    servoAvantBras.write(valGachetteServo);
    
   // Serial.println("on passe en mode bras");
  }
    
   valSwitchAvant=valSwitch;
 

}
