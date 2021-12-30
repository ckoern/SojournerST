import serial
import sys
import time 
comport = sys.argv[1]


def twos_complement(val):
    return (-1)*(256 - (val&256))


with serial.Serial(comport, baudrate=115200, timeout=1) as ser:
        def send(msg):
            print(f"<< '{msg}'")    
            ser.write(msg)
        
        def recv():
            print(f">> '{ser.read(7)}'")

        def calc_checksum(msg):
            return twos_complement( sum(msg), 8 )

        b1 = 1 << 7
        b1 += 0 << 6
        b1 += 9 #get target cps
        
        msg = [0xcc, b1, 0x00, 0x00, 0x00, 0x00]
        msg.append(calc_checksum(msg))

        send(bytes(msg))
        recv()
        time.sleep(0.1)
        send(bytes(msg))
        recv()
        
        
