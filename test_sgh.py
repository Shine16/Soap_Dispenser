

import csv
import serial
import time
try:
    import RPi.GPIO as GPIO
except:
    print "not RPI"
    
#####Configure USB port of windows/Rpi here

#usbPort = "COM20"

#rpi
usbPort="/dev/ttyUSB0"

###################



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

#for LED GPIO
one =16
two =20
three =21
four =6
five =13
six =19
seven = 26

RPI = 0# flag for enabling GPIO

try:
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)

    GPIO.setup(one, GPIO.OUT)
    GPIO.setup(two, GPIO.OUT)
    GPIO.setup(three, GPIO.OUT)
    GPIO.setup(four, GPIO.OUT)
    GPIO.setup(five, GPIO.OUT)
    GPIO.setup(six, GPIO.OUT)
    GPIO.setup(seven, GPIO.OUT)
    #LED
    GPIO.output(one, GPIO.LOW)
    GPIO.output(two, GPIO.LOW)
    GPIO.output(three, GPIO.LOW)
    GPIO.output(four, GPIO.LOW)
    GPIO.output(five, GPIO.LOW)
    GPIO.output(six, GPIO.LOW)
    GPIO.output(seven, GPIO.LOW)
    RPI=1
except:
    print "not RPI, no GPIO"
    RPI=0

while True:
    rx='x'
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
        print "dispenser "+rx

        if rx == 'c':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP12.csv",'a+')
            except:
                f = open("C:/Data/SOAP12.txt",'a+')      
        if rx == 'b':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP11.csv",'a+')
            except:
                f = open("C:/Data/SOAP11.txt",'a+') 
        if rx == 'a':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP10.csv",'a+')
            except:
                f = open("C:/Data/SOAP10.txt",'a+') 
        if rx == '9':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP9.csv",'a+')
            except:
                f = open("C:/Data/SOAP9.txt",'a+')              
        if rx == '8':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP8.csv",'a+')
            except:
                f = open("C:/Data/SOAP8.txt",'a+')
        if rx == '7':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP7.csv",'a+')
            except:
                f = open("C:/Data/SOAP7.txt",'a+')          
        if rx == '6':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP6.csv",'a+')
            except:
                f = open("C:/Data/SOAP6.txt",'a+')
        if rx == '5':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP5.csv",'a+')
            except:
                f = open("C:/Data/SOAP5.txt",'a+')
        if rx == '4':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP4.csv",'a+')
            except:
                f = open("C:/Data/SOAP4.txt",'a+')
        if rx == '3':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP3.csv",'a+')
            except:
                f = open("C:/Data/SOAP3.txt",'a+')
        if rx == '2':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP2.csv",'a+')
            except:
                f = open("C:/Data/SOAP2.txt",'a+')
        if rx == '1':
            try:
                f = open("/home/pi/Desktop/DEC_SGH/SOAP1.csv",'a+')
            except:
                f = open("C:/Data/SOAP1.txt",'a+')
        print str(time.time())[0:10]
        f.write(str(time.time())[0:10]+'\n')
        f.close()

        
    if RPI == 2:#enable only if RPI
        try:
            if rx == '1':
                t1 = time.time()
                GPIO.output(one, GPIO.HIGH)
            if rx == '2':
                t2 = time.time()
                GPIO.output(two, GPIO.HIGH)
            if rx == '3':
                t3 = time.time()
                GPIO.output(three, GPIO.HIGH)
            if rx == '4':
                t4 = time.time()
                GPIO.output(four, GPIO.HIGH)
            if rx == '5':
                t5 = time.time()
                GPIO.output(five, GPIO.HIGH)
            if rx == '6':
                t6 = time.time()
                GPIO.output(six, GPIO.HIGH)
            if rx == '7':
                t7 = time.time()
                GPIO.output(seven, GPIO.HIGH)
        except:
            print "error in GPIO write"

        try:
            if GPIO.input(one) or GPIO.input(two) or GPIO.input(three) or GPIO.input(four) or GPIO.input(five) or GPIO.input(six) or GPIO.input(seven):
                if time.time() - t1 >=1 and t1 !=0:
                    GPIO.output(one, GPIO.LOW)
                    t1=0
                if time.time() - t2 >=1 and t2 !=0:
                    GPIO.output(two, GPIO.LOW)
                    t2=0
                if time.time() - t3 >=1 and t3 !=0:
                    GPIO.output(two, GPIO.LOW)
                    t3=0
                if time.time() - t4 >=1 and t4 !=0:
                    GPIO.output(two, GPIO.LOW)
                    t4=0
                if time.time() - t5 >=1 and t5 !=0:
                    GPIO.output(two, GPIO.LOW)
                    t5=0
                if time.time() - t6 >=1 and t6 !=0:
                    GPIO.output(two, GPIO.LOW)
                    t6=0
                if time.time() - t7 >=1 and t7 !=0:
                    GPIO.output(two, GPIO.LOW)
                    t7=0
        except:
            print "error in GPIO off"        




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
