

//env variables
//C:\Users\shien\AppData\Local\Programs\Python\Python37\Scripts\
//C:\Users\shien\AppData\Local\Programs\Python\Python37\

//C:\Python27
//C:\Python27\Scripts


//4 DT 
//5 SCK

//2 to DTR
//2 to 2K pulldown resistor to GND




#include <HX711.h>
#include <XBee.h>
#define Xbee_sleep 2
/*
This example is for Series 2 XBee
 Sends a ZB TX request with the value of analogRead(pin5) and checks the status response for success
*/


char dispNum='4';  //DISPENSER NUMBER

uint8_t payload[] = { 0 , 0 }; 
bool DEBUG = true;

float calibration_factor = 102.9; // this calibration factor is adjusted according to my load cell
float units;

HX711 scale(4,5);



// create the XBee object
XBee xbee = XBee();


// SH + SL Address of receiving XBee
//0013A200
//4109CE94
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x4109CE94);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();


int statusLed = 13;
int errorLed = 13;


void setup() {
payload[0] = dispNum;// DISPENSER NUMBER
payload[1] = 'x';

  
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);
  digitalWrite(13,LOW);
  
  pinMode(Xbee_sleep ,OUTPUT);
  
  Serial.begin(9600);
  xbee.setSerial(Serial);

  scale.set_scale();
  scale.tare();  //Reset the scale to 0
  
  //long zero_factor = scale.read_average(); //Get a baseline reading - maynot be needed
  delay(100);

}

void loop() {   
  //testOne();//this will send 
  //testTwo();// will send up to 10x till receipt
  //codeTwo();
  senseJoinedNetwork();
}



