#include <SoftwareSerial.h>   //Software Serial Port
#define RxD 2
#define TxD 3
 
#define DEBUG_ENABLED  1
 
SoftwareSerial BLE(RxD,TxD);
 
void setup() 
{ 
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBleConnection();
 
} 
 
void loop() 
{ 
  char recvChar;
  
    if(BLE.available()){//check if there's any data sent from the remote BLE 
      recvChar = BLE.read();
      Serial.print(recvChar);
    }
    if(Serial.available()){//check if there's any data sent from the local serial terminal, you can add the other applications here
       BLE.print("AT+BAUD?"); 
    }
  
} 
 
void setupBleConnection()
{
  BLE.begin(9600); //Set BLE BaudRate to default baud rate 9600
  BLE.print("AT+CLEAR"); //clear all previous setting
  BLE.print("AT+ROLE0"); //set the bluetooth name as a slaver
  BLE.print("AT+SAVE1");  //don't save the connect information
}
