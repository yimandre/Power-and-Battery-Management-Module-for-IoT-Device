import serial
import json
import sys
import requests
import time
import datetime
import argparse



ser = serial.Serial("/dev/ttyUSB0",9600)
ser.baudrate=9600
soc = {0,0}

while True:
        read_ser = ser.readline()
	batt1 = read_ser[0:5]
	batt2 = read_ser[6:11]
	batt3 = read_ser[12:17]
	print(batt1)
	print(batt2)
	print(batt3)

	poc=10
    time= 1

    headers = {'Content-type': 'application/json','Accept': 'text/plain'}
    link = "http://174.89.134.68:8080/SYSC4907/rest/data"
    data = {'soc': batt1,'temp':batt2,'humid':batt3,'powerCon': poc,'time':time}
    response = requests.post(link , data = json.dumps(data), headers = headers)
