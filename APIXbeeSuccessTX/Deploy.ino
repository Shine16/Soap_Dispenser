


void codeThree(){// firmware with AT read of AI register
  
  if((millis()-startTime)>900000){//every 15 mins      
      startTime = millis();
      while(detectNetwork());
         
      
  }
  
  //Read from load cell
  units = scale.get_units();
  if (units < 0)    units = 0.00;
  if(units >2000){//weight sensed for 5kg load cell
  //if(units >50){//weight sensed for 20kg load cell        
      XbeeWake();
      sendTillResponse();  
      digitalWrite(13,HIGH);
  }
  XbeeSleep();
  

 
}


bool detectNetwork(){
   delay(1000);
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
                      return false;                  
                  }
                  else{
                      digitalWrite(13,LOW);
                       flashLed(statusLed, 2, 50);
                       return true;
                  }
                      
  
        } 
    } 
  }
  flashLed(statusLed, 1, 50);
  return true;
}



void codeTwo(){//first deploy
  

  units = scale.get_units();
  if (units < 0)    units = 0.00;

  //Serial.println(units); // delay(100);
  
  //if(units >2000){//weight sensed
  if(units >50){//weight sensed for 20kg load cell 
       
      XbeeWake();
      sendTillResponse();
  
  }
  XbeeSleep();
  
}


void sendTillResponse(){

    int counter=0;
    int sendFlag=1;
    int timeoutTime=1000;
    
    while(sendFlag==1&&counter<60){
      delay(800);
      xbee.send(zbTx);
      delay(200);
      
      if (xbee.readPacket(1000)) {          
          if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
            xbee.getResponse().getZBTxStatusResponse(txStatus);
            if (txStatus.getDeliveryStatus() == SUCCESS) {
              flashLed(statusLed, 1, 50);// flash TX indicator
              sendFlag=0;
              break;
            }
          }
      }
      else{     
         flashLed(statusLed, 3, 100);// flash TX indicator
         delay(500);
      }
      counter++;
      timeoutTime+=200;
      flashLed(statusLed, 2, 100);// flash TX indicator

         
      if(counter==60){
        if(DEBUG)
            flashLed(statusLed, 6, 300);// flash TX indicator
        break;
      }           
    }//close while
    
}

 
void XbeeSleep(){
  digitalWrite(Xbee_sleep,HIGH);//high to Xbee sleep
}


void XbeeWake(){
  digitalWrite(Xbee_sleep,LOW);//low to Xbee sleep
}

