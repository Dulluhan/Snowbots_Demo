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
#define CONS 1.6
#define THRESH 5
#define DAMP 0.1
Servo LeftM;//5
Servo RightM;
int lx,ly,az = 128;
int FWD,TRN = 0;
int lx_h,lx_l,az_h,az_l = 0;
int x, y, z, x_offset, y_offset, z_offset = 0;
int high,low;
void setup() {
  Serial.begin(115200);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  LeftM.attach(6);
  RightM.attach(7);
  
  //while(!pulseIn(4,HIGH)){}
  set_off();
  x_offset = analogRead(0);       // read analog input pin 0
  y_offset = analogRead(1);       // read analog input pin 1
  z_offset = analogRead(2); 
  high = low = 1850;
  
}


void loop() {
 
//test();

FWD = pulseIn(2,HIGH);
TRN = pulseIn(3,HIGH);
if (FWD != 0 && FWD != 0 && FWD < 2590 && FWD > 1220 && TRN < 2560 && TRN > 1230){
traxxas( FWD, TRN);}

}

void traxxas(int FWD, int TRN){
 

/*if (TRN > high){high = TRN;} 
if (TRN < low) {low = TRN;}
Serial.print (high); Serial.print("  "); Serial.println(low);*/

ly = map(FWD, 2586, 1232, 0, 255);
az = map(TRN, 2565, 1234, 0, 255);

if (abs(ly - 128) < 30){ly = 128;}
if (abs(az - 128) < 30){az = 128;}
drive();
}
void test(){
    send_accel();
    int loop_rate = 1;
    int count = 0;
    if (y > THRESH){
      while(y>THRESH+5){
        send_accel();
        
        ly = 128; az = 255;
        drive();
        
    }}
    else if (y < -THRESH){
      while(y < -(THRESH+5)){
        send_accel();
        
        ly = 128; az = 0;
        drive();
        
      
    }}
    ly = 128; az = 128;  drive(); 
    
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



void convert(){
  
 if (ly > 255)ly = 255; else if (ly < 0)ly = 0;
 if (az > 255)az = 255; else if (az < 0)az = 0;
 if (ly != 128){ly = map (ly, 0, 255, 110,70);}else ly = 90;
 if (az != 128){az = map (az, 0, 255, 100,80);}else az = 90;
}


void drive(){
convert();
 if(ly == 90){
   if (az == 90){
     servo_write(LeftM,90);
     servo_write(RightM,90);
   }
   else if (az < 90){
   servo_write(LeftM,90+(90-az)*CONS);
   servo_write(RightM,az);
   }
   else{servo_write(LeftM,90+(90-az)*CONS);
     servo_write(RightM,az);}
 }
 else{
   servo_write(LeftM,90-(90-ly)*CONS);
   servo_write(RightM,ly);
 }
 x = y = z = 0;
}

void servo_write(Servo motor, int throttle){
  throttle = map(throttle, 180, 0, 1000, 2000);
  motor.writeMicroseconds(throttle);
}
