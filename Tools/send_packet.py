import serial
import sys
import time 
import struct 
import threading
import matplotlib.pyplot as plt 

comport = sys.argv[1]

channel = 0 #either 0 or 1
com_lock = threading.Lock()
fig, ax = plt.subplots(5,1,figsize=(8,8), sharex=True)
fig.show()
plt.pause(1)
plotdata = [[],[],[],[],[]]

def twos_complement(val):
    return 256 - (val%256)


with serial.Serial(comport, baudrate=115200, timeout=1) as ser:
        def send(msg):
            print(f"<< '{msg.hex()}'")    
            ser.write(msg)
        
        def recv():
            msg = ser.read(7)
            print(f">> '{msg.hex()}'")
            return list(msg)

        def calc_checksum(msg):
            return twos_complement( sum(msg) )

        def read_float(cmd_id):
            b1 = 1 << 7
            b1 += channel << 6
            b1 += cmd_id
            with com_lock:
                msg = [0xcc, b1, 0,0,0,0]
                msg.append(calc_checksum(msg))
                send(bytes(msg))
                time.sleep(0.01)
                resp  = recv()

            if resp[0] != msg[-1]:
                print("wrong cmd checksum")
                return -1
            if resp[1] != 1:
                print("Cmd no success")
                return -1
            v = struct.unpack( ">f", bytes(resp[2:6]) )
            return v[0]

        def write_float(cmd_id, value):
            b1 = 1 << 7
            b1 += channel << 6
            b1 += cmd_id
            with com_lock:
                msg = [0xcc, b1]
                msg += list( struct.pack(">f", value) ) 
                msg.append(calc_checksum(msg))
                send(bytes(msg))
                time.sleep(0.1)
                resp  = recv()

            if resp[0] != msg[-1]:
                print("wrong cmd checksum")
                return -1
            if resp[1] != 1:
                print("Cmd no success")
                return -1


        def update_loop():
            print("-------")
            v = read_float(5) #kp
            print(f"Current Kp: {v}")
            print("-------")
            time.sleep(.1)
            write_float(5+32, 0.001) #kp
            time.sleep(.1)
            write_float(6+32, 0.0006) #ki
            time.sleep(.1)
            write_float(7+32, 0.0003) #kd
            time.sleep(.1)
            write_float(8+32, 0.5) #kn
            print("-------")
            time.sleep(1)
            v = read_float(5) #kp
            print(f"Current Kp: {v}")
            print("-------")
            time.sleep(1)

            
            print("-------")
            v = get_target_cps()
            print(f"Current Target Cps: {v}")
            print("-------")
            time.sleep(0.1)
            v = read_float(0)
            print(f"Current Measured Cps: {v}")
            print("-------")
            time.sleep(1)

            target_speed = 1234
            for s in range(0,target_speed, 300):
                set_target_cps(s)
                time.sleep(0.1)
            set_target_cps(target_speed)
            print(f"Set Target Cps to {target_speed}")
            print("-------")
            time.sleep(1)

            v = get_target_cps()
            print(f"Current Target Cps: {v}")
            print("-------")
            time.sleep(0.1)
            v = read_float(0)
            print(f"Current Measured Cps: {v}")
            print("-------")
            time.sleep(1)

            for i in range(10):
                v = read_float(0)
                print(f"Current Measured Cps: {v}")
                print("-------")
                time.sleep(1)


            for s in range(target_speed, 0, -300):
                set_target_cps(s)
                time.sleep(0.1)
            set_target_cps(0)
            print(f"Set Target Cps to 0")
            print("-------")
            time.sleep(1)

            v = get_target_cps()
            print(f"Current Target Cps: {v}")
            print("-------")
            time.sleep(0.1)
            v = read_float(0)
            print(f"Current Measured Cps: {v}")
            print("-------")
            time.sleep(1)

        def set_target_cps(val):
            b1 = 1 << 7
            b1 += channel << 6
            b1 += 9+32 #get target cps + setter-offset

            with com_lock:
                msg = [0xcc, b1, (val>>24)%256, (val>>16)%256, (val>>8)%256, val%256]
                msg.append(calc_checksum(msg))
                send(bytes(msg))
                time.sleep(0.1)
                resp  = recv()
            if resp[0] != msg[-1]:
                print("wrong cmd checksum")
            if resp[1] != 1:
                print("Cmd no success")


        def get_target_cps():
            b1 = 1 << 7
            b1 += channel << 6
            b1 += 9 #get target cps
            with com_lock:
                msg = [0xcc, b1, 0,0,0,0]
                msg.append(calc_checksum(msg))
                send(bytes(msg))
                time.sleep(0.1)
                resp  = recv()
            if resp[0] != msg[-1]:
                print("wrong cmd checksum")
                return -1
            if resp[1] != 1:
                print("Cmd no success")
                return -1
            v = (resp[2] << 24) + \
                (resp[3] << 16) + \
                (resp[4] << 8)  + \
                (resp[5])
            return v 

        
        command_thread = threading.Thread(target=update_loop, daemon =True)
        command_thread.start()


        labels = ["Current Cps",
                    "Integrator",
                    "Filter",
                    "Gain",
                    "Setpoint Error"
        ]
        while(True):
            for i in range(5):
                ax[i].clear()
                plotdata[i].append( read_float(i) ) 
                ax[i].plot(plotdata[i], label = labels[i])
                ax[i].set_title(labels[i])
                ax[i].grid(True)
            fig.tight_layout()
            fig.canvas.draw()
            plt.pause(0.02)

        
        
        '''
        b1 = 1 << 7
        b1 += 0 << 6
        b1 += 9 #get target cps
        
        msg = [0xcc, b1, 0x00, 0x00, 0x00, 0x00]
        msg.append(calc_checksum(msg))

        msg_set = [0xcc, b1+32, 0x00, 0x00, 0x00, 0xff]
        msg_set.append(calc_checksum(msg_set))


        send(bytes(msg))
        recv()
        time.sleep(0.1)
        send(bytes(msg))
        recv()
        time.sleep(0.1)
        print("-------")

        
        send(bytes(msg_set))
        recv()
        time.sleep(0.1)
        send(bytes(msg))
        recv()
        time.sleep(0.1)
        print("-------")

        
        send(bytes(msg))
        recv()
        time.sleep(0.1)
        send(bytes(msg))
        recv()
        time.sleep(0.1)
        print("-------")
        '''
        
