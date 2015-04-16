
#include <SoftwareSerial.h>

// Use a L293D IC to control the 4 wheel
#define motorLeftEnable 6
#define motorRightEnable 10
#define motorLeftPole1 4
#define motorLeftPole2 5
#define motorRightPole1 8
#define motorRightPole2 9
#define bluetoothRx 12
#define bluetoothTx 11
 
  int xaxis;
  int yaxis;
  int xval;
  int yval;
  int xctllow = -90; //看電腦控制的x大小
  int xctlhigh = 90; //看電腦控制的x大小
  int yctllow = -90; //看電腦控制的y大小
  int yctlhigh = 90; //看電腦控制的y大小
  int vleft;
  int vright;
  SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup(){
  pinMode(motorLeftEnable, OUTPUT);
  pinMode(motorRightEnable, OUTPUT);
  pinMode(motorLeftPole1, OUTPUT);
  pinMode(motorLeftPole2, OUTPUT);
  pinMode(motorRightPole1, OUTPUT);
  pinMode(motorRightPole2, OUTPUT);
  Serial.begin(9600);
  bluetooth.begin(9600);
}
void loop(){
 ////////////////////////////////////////////////////////
  if(bluetooth.available()){
    int xOrY = 0;
    int xIndex = 0;
    int yIndex = 0;
    char xx[12];
    char yy[12];
    char inin;
    while (bluetooth.available() && (inin = bluetooth.read()) != '\n'){
      if (inin == ','){
       xOrY = 1;
      }
      else if (xOrY == 0){
        xx[xIndex] = inin;
        xIndex++;
      }else if (xOrY == 1){
        yy[yIndex] = inin;
        yIndex++;
      }
    }
    
    xx[xIndex] = '\0';
    yy[yIndex] = '\0';
    
//    String order = (String)bluetooth.read();
//    xaxis = getValue(order, ',', 0).toInt();
//    yaxis = getValue(order, ',', 1).toInt();
      xaxis = atoi(xx);
      yaxis = atoi(yy);
      Serial.print(xaxis);
      Serial.print(',');
      Serial.println(yaxis);
      xval = map(xaxis, xctllow, xctlhigh, -255, 255);
      yval = map(yaxis, yctllow, yctlhigh, -255, 255);//y前進後退
  }
  
  vleft = abs(yval);
  vleft = constrain(vleft, 0, 255);
  vright = abs(yval) - abs(xval);
  vright = constrain(vright, 0, 255);
  
  if(yval >= 0){    // turn right
    if(xval >= 0){
    motor(motorLeftPole1, motorLeftPole2, motorLeftEnable, 1, vleft);
    motor(motorRightPole1, motorRightPole2, motorRightEnable, 1, vright);
    }
    else if (xval < 0){  // turn left
      motor(motorLeftPole1, motorLeftPole2, motorLeftEnable, 1, vright);
      motor(motorRightPole1, motorRightPole2, motorRightEnable, 1, vleft);
    }
  }
  else if(yval < 0){    //backward
    if(xval >= 0){
    motor(motorLeftPole1, motorLeftPole2, motorLeftEnable, 2, vleft);
    motor(motorRightPole1, motorRightPole2, motorRightEnable, 2, vright);
    }
    else if(xval < 0){
    motor(motorLeftPole1, motorLeftPole2, motorLeftEnable, 2, vright);
    motor(motorRightPole1, motorRightPole2, motorRightEnable, 2, vleft);
    }
  }
  else{
    motor(motorLeftPole1, motorLeftPole2, motorLeftEnable, 0, vleft);
    motor(motorRightPole1, motorRightPole2, motorRightEnable, 0, vright);
  }
}

void motor( char in1, char in2, char EN,char state, int v){
    if(state == 1){
      analogWrite(EN, v);
      digitalWrite(in1, 0);
      digitalWrite(in2, 1);
    }
    else if(state == 2){
      analogWrite(EN, v);
      digitalWrite(in1, 1);
      digitalWrite(in2, 0);
    }
    else if(state == 0){
      analogWrite(EN, 0);
      digitalWrite(in1, 0);
      digitalWrite(in2, 0);
    }
}
String getValue(String data, char separator, int index){
    int maxIndex = data.length()-1;
    int j=0;
    String chunkVal = "";
    for(int i=0; i<=maxIndex && j<=index; i++){
      chunkVal.concat(data[i]);
      if(data[i]==separator){
        j++;
        if(j>index){
          chunkVal.trim();
          return chunkVal;
        }    
        chunkVal = "";    
      }  
    }  
}
