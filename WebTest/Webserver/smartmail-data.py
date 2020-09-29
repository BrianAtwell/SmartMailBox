#!/usr/bin/env python
import os
import time
import atexit
import json
import cgi
import cgitb; cgitb.enable()  # for troubleshooting
import random

print "Content-type: application/json\n\n"


form = cgi.FieldStorage()

vertIR = {
	"IRAnalog" : 4095,
	"IRState": 0,
	"IRLimitHigh": 4095,
	"IRLimitLow": 3000,
	"IRLimitTLow": 2000,
	"IRDir" : 0
}

horiIR = {
	"IRAnalog" : 4095,
	"IRState": 0,
	"IRLimitHigh": 4095,
	"IRLimitLow": 3000,
	"IRLimitTLow": 2000,
	"IRDir" : 0
}

DoorState=0
sensorTime=0
error=0

def ReadLimits():
	try:
		with open("../limits.txt", "r") as file:
			lines=file.read().splitlines()
			print(lines)
			horiIR["IRLimitHigh"]=int(lines[0])
			horiIR["IRLimitLow"]=int(lines[1])
			vertIR["IRLimitHigh"]=int(lines[2])
			vertIR["IRLimitLow"]=int(lines[3])
	except IOError:
		error=1

def ReadMailData():
	try:
		with open("../mailData.txt", "r") as file:
			lines=file.read().splitlines()
			if len(lines)==8:
				sensorTime=int(lines[0])
				horiIR["IRAnalog"]=int(lines[1])
				vertIR["IRAnalog"]=int(lines[2])
				horiIR["IRState"]=int(lines[3])
				vertIR["IRState"]=int(lines[4])
				DoorState=int(lines[5])
				horiIR["IRDir"]=int(lines[6])
				vertIR["IRDir"]=int(lines[7])
	except IOError:
		error=2
	
def WriteMailData():
	try:
		with open("../mailData.txt", "w") as file:
			file.write(str(sensorTime)+'\n')
			file.write(str(horiIR["IRAnalog"])+'\n')
			file.write(str(vertIR["IRAnalog"])+'\n')
			file.write(str(horiIR["IRState"])+'\n')
			file.write(str(vertIR["IRState"])+'\n')
			file.write(str(DoorState)+'\n')
			file.write(str(horiIR["IRDir"])+'\n')
			file.write(str(vertIR["IRDir"])+'\n')
	except IOError:
		error=3
	
def RandomizeIRData(lobj):
	if lobj["IRDir"] == 0:
		if lobj["IRAnalog"] > lobj["IRLimitTLow"]:
			lobj["IRAnalog"] = lobj["IRAnalog"]-random.randint(1, abs(lobj["IRAnalog"]-lobj["IRLimitTLow"]))
		else:
			lobj["IRAnalog"] = lobj["IRLimitTLow"]
			lobj["IRDir"] = 1
			
	if lobj["IRDir"] == 1:
		if lobj["IRAnalog"] < lobj["IRLimitHigh"]:
			lobj["IRAnalog"] = lobj["IRAnalog"]+random.randint(1, abs(lobj["IRLimitHigh"]-lobj["IRAnalog"]))
		else:
			lobj["IRAnalog"] = lobj["IRLimitHigh"]
			lobj["IRDir"] = 0

def UpdateIRState(lobj):
	if lobj["IRAnalog"] >= lobj["IRLimitHigh"]:
		lobj["IRState"] = 1
	elif lobj["IRAnalog"] <= lobj["IRLimitLow"]:
		lobj["IRState"] = 2
	else:
		lobj["IRState"] = 0

ReadLimits()
ReadMailData()

sensorTime+=random.randint(10, 1200)
RandomizeIRData(horiIR)
RandomizeIRData(vertIR)
DoorState=random.randint(0, 1)
UpdateIRState(horiIR)
UpdateIRState(vertIR)


WriteMailData()

print "{\
	\"Current\":{\
		\"error\": "+str(error)+",\
		\"time\": "+str(sensorTime)+",\
		\"timestr\": null,\
		\"HorizontalDir\": "+str(horiIR["IRDir"])+",\
		\"VerictalDir\": "+str(vertIR["IRDir"])+",\
		\"HorizontalIRAnalog\": "+str(horiIR["IRAnalog"])+",\
		\"VerticalIRAnalog\": "+str(vertIR["IRAnalog"])+",\
		\"HorizontalIRState\": "+str(horiIR["IRState"])+",\
		\"VerticalIRState\": "+str(vertIR["IRState"])+",\
		\"DoorState\": "+str(DoorState)+"\
	},\
	\"Changes\": [{\
		\"time\": 1126,\
		\"timestr\": null,\
		\"HorizontalIRAnalog\": 4095,\
		\"VerticalIRAnalog\": 4095,\
		\"HorizontalIRState\": 0,\
		\"VeriticalIRState\": 0,\
		\"DoorState\": 0\
	}]\
}"