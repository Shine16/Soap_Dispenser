##/*pip install xbee
##Collecting xbee
##  Downloading https://files.pythonhosted.org/packages/0e/a3/464ff453523bd0a614df9f738afbdc02888bd59440ba35edcacb50805c85/XBee-2.3.2-py3-none-any.whl
##Requirement already satisfied: pyserial in c:\users\shien\appdata\local\programs\python\python37\lib\site-packages (from xbee) (3.4)
##Installing collected packages: xbee
##Successfully installed xbee-2.3.2
#https://python-xbee.readthedocs.io/en/latest/

#apimode Xbees

import serial

try:
    port = serial.Serial("COM25", baudrate=9600, timeout=3.0)
except:
    try:
        port = serial.Serial("COM25", baudrate=9600, timeout=3.0)
    except:
        print "not able to connect"
    
count=0
hrx=0

print port

while True:
    rx=port.read()
    
    try:
        hrx =int(rx.encode('hex'), 16)
        count=count+1
    except:
        
        if rx!="":
            print rx
            print type(rx)
        


    

    
    # find start byte
    if hrx == int(0x7e) :
        print "last frame "+str(count)+" length"
        count=0
        #print "new frame"

        #read out unused bytes
        for x in range(15):
            count=count+1
            rx=port.read()
            #print rx
            
        rx=port.read()
        count=count+1
        #print hex(int(rx.encode('hex'), 16))
        print rx
        if rx == '6':
            print "dispenser 6"

        if rx == '5':
            print "dispenser 5"
        if rx == '4':
            print "dispenser 4"    
        
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
