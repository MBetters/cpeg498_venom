import urllib2
import time
import serial
import json

#A custom server is used to send signals to the Raspberry Pi
info_url = "http://128.4.25.156/info"
ser = serial.Serial('/dev/ttyACM0', 9600)

ser.timeout = 1
time.sleep(1)
last_action = ''

while True:
    try:
        latest_info = urllib2.urlopen(info_url).read()
        parsed_info = json.loads(latest_info)
        action = parsed_info['action']

        if action != last_action:
            ser.write(bytes(action))

        last_action = action
    except Exception as e:
        print e
        pass

    time.sleep(1)
