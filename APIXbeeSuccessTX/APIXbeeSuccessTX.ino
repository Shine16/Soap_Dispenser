
#include <HX711.h>
#include <XBee.h>
#define Xbee_sleep 2






char dispNum='6';  //DISPENSER NUMBER (CHANGE THIS BASED ON DISPENSER)






uint8_t shCmd[] = {'S','H'};
uint8_t slCmd[] = {'S','L'};
uint8_t assocCmd[] = {'A','I'};
AtCommandRequest atRequest = AtCommandRequest(shCmd);
AtCommandResponse atResponse = AtCommandResponse();
//AT code



uint8_t payload[] = { 0 , 0 }; 
bool DEBUG = true;

float calibration_factor = 102.9; // this calibration factor is adjusted according to my load cell
float units;

HX711 scale(4,5);



// create the XBee object
XBee xbee = XBee();

//S2B
//0013A200
//40A81F89
//XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40A81F89);

//S2C
// SH + SL Address of receiving XBee
//0013A200
//4109CE94
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x4109CE94);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();


int statusLed = 13;
int errorLed = 13;

long startTime = 0;

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
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  
  //long zero_factor = scale.read_average(); //Get a baseline reading - maynot be needed
  delay(100);
  
  startTime = millis();

  //enable for CodeThree
  while(detectNetwork());
  
}

void loop() {   
  //testOne();//this will send 
  //testTwo();// will send up to 10x till receipt
  //codeTwo();
  //senseJoinedNetwork();//test with 1 hr, SN register reading

  codeThree();//firmware with AT SN register reading
}





//env variables
//C:\Users\shien\AppData\Local\Programs\Python\Python37\Scripts\
//C:\Users\shien\AppData\Local\Programs\Python\Python37\

//C:\Python27
//C:\Python27\Scripts


//4 DT 
//5 SCK

//2 to DTR
//2 to 2K pulldown resistor to GND



