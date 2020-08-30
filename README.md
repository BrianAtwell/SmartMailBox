# SmartMailBox
By Brian Atwell

This is a project to make a smart mail box. Basicly just had a spare ESP32-WROOM sitting around and wanting to make some IoT project. Although I try limit it to just a local network.

This project is a work in progress and right now it is still in the design phase.

## Hardware
* MCU: ESP32-WROOM
* IR LED - IR receiver	analog ambient light + full IR LED/ Blocked by mail ambient light. Light can also flucuate between mailbox door open and close. The Light can also change based on the time of day. There are two pairs, one for the vertical and one for the horizontal.
* Limit Switch			digital on/off connected to the mailbox door. Connected when the door is closed and disconnected when the door is open.

## Hardware to MCU Pinout
* Vertical IR LED GPIO13
* Horizontal IR LED GPIO16
* Vertical IR detector RTC_GPIO0, ADC1_CH0, GPIO36, pin14
* Horizontal IR detector RTC_GPIO4, ADC1_CH6, GPIO34, pin12
* limit switch GPIO17


## Software Requirements

### Minimum
1. Send/Transmit the following events
   1. Mail box door open (limit switch "connected")
   1. Mail box door close (limit switch "open")
   1. Vertical IR Blocked (Based on a user settable Vertical Blocked Trigger value. This is a lower value in the analog range but a local max value that indicates a blockage. When the analog IR value drops below the Vertical Blocked Trigger then it will send the Vertical IR Blocked event.
   1. Vertical IR Connected (Based on user settable Vertical Connect Trigger value. This is a high value in the analog range but a local min value that indicates light from IR LED. When the analog IR value raises above the Vertical Connect Trigger value then it will send the Vertical IR Connected event.
   1. Horizontal IR Blocked (Based on a user settable Horizontal Blocked Trigger value. This is a lower value in the analog range but a local max value that indicates a blockage. When the analog IR value drops below the Horizontal Blocked Trigger then it will send the Horizontal IR Blocked event.
	 1. Horizontal IR Connected (Based on user settable Horizontal Connect Trigger value. This is a high value in the analog range but a local min value that indicates light from IR LED. When the analog IR value raises above the Horizontal Connect Trigger value then it will send the Horizontal IR Connected event.

1. The ESP32 will host an IP Server application
	 1. A client (PC) will connect, send its IP address and port, then disconnect. This will be called Server Registration.
	 1. It will support up to 10 clients
	 1. When the an event occurs the ESP32 will connect to the "client". Now the "client" will host a server application at the IP and port.
	 1. The data will probably be sent in a format similiar to aREST https://www.home-assistant.io/integrations/arest/
   
1. Server application to receive the SmartMailBox events. This will probably a separate project that will use a raspberry pi. Following are some ideas.
   1. The Pi will control an old CD drive that will popup a prop letter that says you got mail.
   1. Use a servo to swing to upright position to indicate mail arrived.
   
1. Server Host URL will be a max of 512 bytes.
	
### Optional
1. The ESP32 will register SmartHomeDevice mDNS service.
	 1. A Server (PC) will listen for the SmartHomeDevice mDNS service.
	 1. The Server will connect to the ESP32 using the Server Registration listed above.
