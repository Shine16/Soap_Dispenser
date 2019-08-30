


void codeTwo(){//first deploy
  
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  units = scale.get_units();
  if (units < 0)    units = 0.00;
  
  if(units >2000){//weight sensed
 
      int sending=1;
      XbeeWake();
      sendTillResponse();

  
  }
  XbeeSleep();
  
}


void sendTillResponse(){

    int counter=0;
    int sendFlag=1;
    int timeoutTime=500;
    
    while(sendFlag==1&&counter<10){
      delay(1000);
      xbee.send(zbTx);
      
      if (xbee.readPacket(timeoutTime)) {          
          if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
            xbee.getResponse().getZBTxStatusResponse(txStatus);
            if (txStatus.getDeliveryStatus() == SUCCESS) {
              flashLed(statusLed, 1, 50);// flash TX indicator
              sendFlag=0;
              XbeeSleep();
              break;
            }
          }
      }     
         
      counter++;
      timeoutTime+=500;
         
      if(counter==10){
        if(DEBUG)
            Serial.print("Fail to send"); 
        XbeeSleep();
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

