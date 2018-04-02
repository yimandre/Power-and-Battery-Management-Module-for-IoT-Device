import json
import sys
import requests
import time
import datetime
import argparse

#JuneLooi
#Uploading data to the server

batt1=90 #
batt2=80 #
batt3=20 
temp= 18
humid= 30

#local= 192.168.2.27
#public=174.89.134.68

headers = {'Content-type': 'application/json','Accept': 'text/plain'}
link = "http://192.168.2.27:8080/SYSC4907/rest/data"
datetime1 = str(datetime.datetime.today())
datetime2 = datetime1.replace(" ","T")
time = datetime1
data = {'soc1': batt1,'soc2':batt2,'soc3':batt3,'temp': temp,'humid': humid,'time':time   } #
response = requests.post(link , data = json.dumps(data), headers = headers)
#data.update({"Timestamp":datetime2})
#passtext(data)
#time.sleep(sleepTime)
