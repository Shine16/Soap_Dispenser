
bool debugLoadCell(){
   units = scale.get_units();
   Serial.println(units);
   delay(1000);
   return true;
}
 


void senseJoinedNetwork(){
    XbeeWake();// just this, found to join network to be about 6s, modem orientation dependant
    

 
    /*xbee.readPacket();
    if (xbee.getResponse().isAvailable()) {


      //7E 00 02 8A 02 73
      if(xbee.getResponse().getFrameDataLength()==6)
        if((xbee.getResponse().getFrameData()[4], HEX)==0x02)      
          flashLed(statusLed, 3, 100);// flash TX indicator
 
    }*/


      //check AI - if joined will show 0
      //sendAtCommand();delay(1000);
      sendAtCommandNoPrint();delay(1000);
      
}


//without printout, only led 13, high if joined, low if not
// to find AI - FF if not joined, 00 if joined
// will detect if End device has joined network
void sendAtCommandNoPrint() {
  atRequest.setCommand(assocCmd);  
  XbeeWake();
  xbee.send(atRequest);

  // wait up to 5 seconds for the status response
  if (xbee.readPacket(500)) {    // got a response!
     if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
        xbee.getResponse().getAtCommandResponse(atResponse);
        if (atResponse.isOk()) {
                  if((atResponse.getValue()[0])==0){
                      digitalWrite(13,HIGH);
                      XbeeSleep();
                      for(int i=0;i<60;i++)
                        delay(60000);
                  }
                  else{
                      digitalWrite(13,LOW);
                       flashLed(statusLed, 2, 50);
                  }
                      
  
        } 
    } 
  }
  flashLed(statusLed, 1, 50);
 
}




// to find AI - FF if not joined, 00 if joined
// will detect if End device has joined network
void sendAtCommand() {
  atRequest.setCommand(assocCmd);  
  XbeeWake();
  // send the command
  xbee.send(atRequest);

  // wait up to 5 seconds for the status response
  if (xbee.readPacket(5000)) {    // got a response!
     if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
        xbee.getResponse().getAtCommandResponse(atResponse);
        XbeeSleep();delay(50);Serial.println();
        Serial.println(atResponse.getValue()[0]);
      if (atResponse.isOk()) {
                if((atResponse.getValue()[0])==0)
                    digitalWrite(13,HIGH);
                    else
                    digitalWrite(13,LOW);

          /*
            if (atResponse.getValueLength() > 0) {
              //Serial.println(atResponse.getValueLength(), DEC);          
              for (int i = 0; i < atResponse.getValueLength(); i++) {
                Serial.print(atResponse.getValue()[i], HEX);
                Serial.print(" ");
                if((atResponse.getValue()[0], HEX)==0x00)
                    digitalWrite(13,HIGH);
                    else
                    digitalWrite(13,LOW);
              }
    
              Serial.println("");
            }
            */
      } 
      /*else {
        Serial.print("Command return error code: ");
        Serial.println(atResponse.getStatus(), HEX);
      }*/
    } 
    
    /*else {
      //XbeeSleep();delay(50);Serial.println();
      Serial.print("Expected AT response but got ");
      Serial.print(xbee.getResponse().getApiId(), HEX);
    }   */
    
  }
  /*else { //read packet timeout fail
    // at command failed
    if (xbee.getResponse().isError()) {
      //XbeeSleep();delay(50);Serial.println();
      Serial.println("EC");  
      //Serial.println(xbee.getResponse().getErrorCode());
    } 
    else {
      //XbeeSleep();delay(50);Serial.println();
      Serial.println("NR");  
    }
  }*/
}




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

