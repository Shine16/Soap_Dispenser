


void codeTwo(){//first deploy
  
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
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
              XbeeSleep();
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

