#!/usr/bin/env python
import os
import time
import atexit
import json
import cgi
import cgitb; cgitb.enable()  # for troubleshooting


form = cgi.FieldStorage()

with open("..\\limts.txt", "w") as file:
	file.write(form.getvalue('hAnalogIRLimitHigh')+'\n')
	file.write(form.getvalue('hAnalogIRLimitLow')+'\n')
	file.write(form.getvalue('vAnalogIRLimitHigh')+'\n')
	file.write(form.getvalue('vAnalogIRLimitLow')+'\n')

print "Content-type: application/json\n\n"
print "{"
print "\"hAnalogIRLimitHigh\": "+str(form.getvalue('hAnalogIRLimitHigh'))+","
print "\"hAnalogIRLimitLow\": "+str(form.getvalue('hAnalogIRLimitLow'))+","
print "\"vAnalogIRLimitHigh\": "+str(form.getvalue('vAnalogIRLimitHigh'))+","
print "\"vAnalogIRLimitLow\": "+str(form.getvalue('vAnalogIRLimitLow'))
print "}"