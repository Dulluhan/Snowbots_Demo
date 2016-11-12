  /*Firmware for old remote for UBC Snowbots
 *Test version
 *Author: Vincent Yuan
 *Date: March 25
*/
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <Servo.h>
#include <math.h>
#define TRIM 20
#define LEFT_TURN 9
#define RIGHT_TURN -3
#define CONS -3
#define THRESH 5
#define DAMP 0.1
Servo LeftM;//5
Servo RightM;
int lx,ly,az = 128;
int R1,R2,R3,R4,B2,B4,Mode = 0;
int lx_h,lx_l,az_h,az_l = 0;
int x, y, z, x_offset, y_offset, z_offset = 0;
void setup() {
  Serial.begin(115200);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  LeftM.attach(9);
  RightM.attach(10);
  
  //while(!pulseIn(4,HIGH)){}
  set_off();
  x_offset = analogRead(0);       // read analog input pin 0
  y_offset = analogRead(1);       // read analog input pin 1
  z_offset = analogRead(2); 
}


void loop() {
 
test();

}
void test(){
    send_accel();
    int loop_rate = 1;
    int count = 0;
    if (y > THRESH){
      while(y>THRESH+5){
        send_accel();
        count++;
      }
      while (count > 0){
        ly = 128; az = 255;
        drive();
        count--;
      }
    }
    else if (y < -THRESH){
      while(y < -(THRESH+5)){
        send_accel();
        count++;
      }
      while(count > 0){
        ly = 128; az = 0;
        drive();
        count--;
      }
      
    }
    ly = 128; az = 128;  drive(); delay(200);
    
}

void send_accel(){
  x = (analogRead(0)-x_offset);       
  y = (analogRead(1)-y_offset);       
  z = (analogRead(2)-z_offset);
}

void set_off(){
  int lx_mid = 1420; //pulseIn(3,HIGH); 
  int az_mid = 1460; //pulseIn(5,HIGH);
  lx_h = lx_mid+150; lx_l = lx_mid-150;
  az_h = az_mid+150; az_l = az_mid-150;
}


 
void serial_read(){
  if (Serial.available()>9){
    if (Serial.read() == 'B'){
    int B9 = (Serial.read()-'0')*100 + (Serial.read()-'0')*10 + (Serial.read()-'0');
    B2 =(Serial.read()-'0')*100 + (Serial.read()-'0')*10 + (Serial.read()-'0');
    B4 = (Serial.read()-'0')*100 + (Serial.read()-'0')*10 + (Serial.read()-'0');
   //Serial.print("B4: "); Serial.print(B4);
   
    }
    else{B2 = B4 = 128;}
    //B2 = 128;
}
else{  
B2 = B4 = 128;}
//Serial.end();
//Serial.begin(9600);
  Serial.flush();
}

void convert(){
  
 if (ly > 255)ly = 255; else if (ly < 0)ly = 0;
 if (az > 255)az = 255; else if (az < 0)az = 0;
 if (ly != 128){ly = map (ly, 0, 255, 100,80);}else ly = 90;
 if (az != 128){az = map (az, 0, 255, 105,75);}else az = 90;
}


void drive(){
convert();
 if(ly == 90){
   if (az == 90){
     LeftM.write(90-CONS);
     RightM.write(90);
   }
   else if (az > 90){
   LeftM.write(az);//needs to be 100
   RightM.write(az);//80 then 82
   }
   else{LeftM.write(az);
   RightM.write(az);}
 }
 /*else{
   if (ly < 90){
   LeftM.write(ly-10);//if 80 //then 70
   RightM.write(ly+CONS);//if 80 then 78
   }
   else{LeftM.write(ly+7);//+10
   RightM.write(ly-CONS+2);}//+2
 }*/
 x = y = z = 0;
}
