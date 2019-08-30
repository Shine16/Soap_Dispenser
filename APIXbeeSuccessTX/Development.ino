
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

