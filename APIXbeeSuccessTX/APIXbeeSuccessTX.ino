

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


char dispNum='4';
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

void flashLed(int pin, int times, int wait) {

  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times) {
      delay(wait);
    }
  }
}

void setup() {
payload[0] = dispNum;// DISPENSER NUMBER
payload[1] = 'x';

  
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);

  pinMode(Xbee_sleep ,OUTPUT);
  
  Serial.begin(9600);
  xbee.setSerial(Serial);

  scale.set_scale();
  scale.tare();  //Reset the scale to 0
  
  long zero_factor = scale.read_average(); //Get a baseline reading
  delay(100);

}

void loop() {   
  //testOne();//this will send 
  //testTwo();// will send up to 10x till receipt
  codeOne();
}



void codeOne(){//first deploy
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  units = scale.get_units();
  if (units < 0)    units = 0.00;
  
  if(units >2000){//send
 
      int sending=1;
      
      digitalWrite(Xbee_sleep,LOW);//wake
      
      delay(500);
      xbee.send(zbTx);
      
    if (xbee.readPacket(1000)) {          
              if(xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE)
                  xbee.getResponse().getZBTxStatusResponse(txStatus);
                  
              if (txStatus.getDeliveryStatus() == SUCCESS) {
                  sending=0;//turn off flag
                  flashLed(statusLed, 1, 50); 
                  digitalWrite(Xbee_sleep,HIGH);//sleep      
              }
              else{
                    int counter=0;
                    while(sending==1&&counter<10){
                        if(sendAgain())//if true, stop sending again
                          sending=0;
                        counter++;               
                    }
                    if(counter==10)
                      if(DEBUG)Serial.print("Fail to send");
              }
              }//if right packet and success
              
         else{
          int counter=0;
              while(sending==1&&counter<10){
                  if(sendAgain())//if true, stop sending again
                    sending=0;
                  counter++; 
              }
              if(counter==10)
                if(DEBUG)Serial.print("Fail to send");
        }
  
  }
  
  
  digitalWrite(Xbee_sleep,HIGH);//high to sleep
  

}

void testTwo(){//sleep
  payload[0] = '6';
  payload[1] = 'x';
  int sending=1;
  digitalWrite(Xbee_sleep,LOW);//wake
  delay(500);
  xbee.send(zbTx);
  
  // after sending a tx request, we expect a status response
  // wait up to half second for the status response
  if (xbee.readPacket(1000)) {          
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
          if (txStatus.getDeliveryStatus() == SUCCESS) {
              sending=0;
              flashLed(statusLed, 1, 50); 
              if(DEBUG)Serial.println("success");
          }
          int counter=0;
          while(sending==1&&counter<10){
              if(DEBUG)Serial.println("n");
              if(sendAgain())//if true, stop sending again
                sending=0;
              counter++;               
          }
          if(counter==10)flashLed(statusLed, 10, 500);
           
    }
  }
  digitalWrite(Xbee_sleep,HIGH);//wake
  delay(2000);

}

bool sendAgain(){
   // digitalWrite(Xbee_sleep,LOW);//wake
    delay(1000);
    xbee.send(zbTx);

    if (xbee.readPacket(1000)) {          
      if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        flashLed(statusLed, 1, 50);// flash TX indicator
        return true;
      }
      else
        return false;
       // digitalWrite(Xbee_sleep,HIGH);//sleep
      
    }
    }

}

void testOne(){
  // break down 10-bit reading into two bytes and place in payload
  
  //digitalWrite(Xbee_sleep,LOW);//XBEE wake
  payload[0] = '6';
  payload[1] = 'x';

  xbee.send(zbTx);

  // flash TX indicator
  flashLed(statusLed, 1, 100);

  // after sending a tx request, we expect a status response
  // wait up to half second for the status response
  if (xbee.readPacket(500)) {
    // got a response!

    // should be a znet tx status              
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);

      // get the delivery status, the fifth byte
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        // success.  time to celebrate
        flashLed(statusLed, 5, 50);
      } else {        
        //can try print again      
      }
    }
  } else if (xbee.getResponse().isError()) {
    //can try print again
  } else {
    //can try print again
   // flashLed(errorLed, 2, 50);
  }

  delay(1000);
}


