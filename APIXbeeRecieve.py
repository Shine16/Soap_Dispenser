


import serial
import time
#usbPort = "COM25"
usbPort = "COM27"

def connectXbeeSerial():
    try:
        port = serial.Serial(usbPort, baudrate=9600, timeout=3.0)
        print "Serial connected!"
        return port
    except:
        print "error in Serial"
        time.sleep(1)
        return connectXbeeSerial()
        

        

port = connectXbeeSerial()
print port

## variables for function
count=0
hrx=0



while True:
    rx=port.read()
    
    try:
        hrx =int(rx.encode('hex'), 16)
        count=count+1
    except:        
        if rx!="":
            print rx
            print type(rx)
            
    
    if hrx == int(0x7e) :  # find start byte
        print "last frame "+str(count)+" length"
        count=0
        #print "new frame"

        
        for x in range(15): # read past unused bytes
            count=count+1
            rx=port.read()
            
            
        rx=port.read() # read 17th byte (payload 0)
        count=count+1
        #print hex(int(rx.encode('hex'), 16))
        print rx
        
        if rx == 'c':
            print "dispenser "+rx
        if rx == 'b':
            print "dispenser "+rx
        if rx == 'a':
            print "dispenser "+rx    
        if rx == '9':
            print "dispenser "+rx
        if rx == '8':
            print "dispenser "+rx
        if rx == '7':
            print "dispenser "+rx            
        if rx == '6':
            print "dispenser "+rx
        if rx == '5':
            print "dispenser "+rx
        if rx == '4':
            print "dispenser "+rx    
        if rx == '3':
            print "dispenser "+rx
        if rx == '2':
            print "dispenser "+rx
        if rx == '1':
            print "dispenser "+rx

            
        #rx=port.read()
        #print rx
        #print hex(int(rx.encode('hex'), 16))






        
    #if
    #print rcv

#https://stackoverflow.com/questions/45505958/pyserial-reading-hex-value-from-mcu
        
#    //port.write("\r\nYou sent:" + repr(rcv))
#https://unix.stackexchange.com/questions/256039/how-do-i-print-the-data-received-on-a-serial-port

#https://pyserial.readthedocs.io/en/latest/shortintro.html

# 7E start delimiter
# Length (14) 00 0E
# frame type (90) recieve packet
# 64 bit source address 00 7D 33 A2 00 40 D7 CB 62
# 16 bit source address 46 C1
# recieve option 01
# checksum C0
# ASCII 6 (36)
# ASCII x (78)
#7E 00 0E 90 00 7D 33 A2 00 40
#D7 CB 62 46 C1 01 36 78 C0
