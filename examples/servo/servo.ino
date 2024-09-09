#include <Servo.h>

Servo moteur, servo;

unsigned long previousMillis = 0;

char buf[100];
int trigger, joystick;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  moteur.attach(3);
  moteur.write(90);
  servo.attach(5);
  servo.write(90);
}

void loop() {
  if(millis()-previousMillis>1000){
    moteur.write(90);
    servo.write(90);
    //Serial.println("timeout");
  }
  
  if(Serial.available()>0){
    previousMillis=millis();
    trigger=Serial.parseInt();  //message en uart de la forme "commande_moteur commande_servo"
    joystick=Serial.parseInt();
    if(trigger and joystick){ //on envoie les commandes seulement si les 2 sont valides
      moteur.write(trigger);
      servo.write(joystick);
      sprintf(buf, "%d %d", trigger, joystick);
      Serial.write(buf);
    }
    /*else{
      Serial.println("erreur lecture");
    }*/
    Serial.read();
  }
}
