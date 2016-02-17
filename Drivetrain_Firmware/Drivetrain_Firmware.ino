#include <SoftwareSerial.h>
#include <stdlib.h>
int LeftM= 13;//5
int RightM = 6;
int lx,ly,az = 0;

void setup()  { 
  Serial.begin(9600);
  Serial.println("ready");
} 

void loop()  { 
  delay(100);
  serial_read();  
  convert();
  drive();
  lx = ly = az = 0;
}
void serial_read(){
  if (Serial.available()== 10){
    if (Serial.read() == 'B'){
    lx = (Serial.read()-'0')*100 + (Serial.read()-'0')*10 + (Serial.read()-'0');
    ly =(Serial.read()-'0')*100 + (Serial.read()-'0')*10 + (Serial.read()-'0');
    az = (Serial.read()-'0')*100 + (Serial.read()-'0')*10 + (Serial.read()-'0');
    }
}
}
void convert(){
 lx = map (lx, 0, 255, -100, 100);
 ly = map (lx, 0, 255, -100, 100);
 az = map (lx, 0, 255, -100, 100);
}


void drive(){
 if(lx == 0){
  if(ly == 0){
   if (az == 0){
   }
   else{
    analogWrite(LeftM,az);
    analogWrite(RightM,-az);   
   }
  }
  else{
   analogWrite(LeftM,ly);
   analogWrite(RightM,ly);
   }
 }
 else{
   analogWrite(LeftM,lx);
   analogWrite(RightM,lx);
 }
}


