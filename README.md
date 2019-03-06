# sailbot1819NEW

Steps for setting up and running NMEA CAN bus on arduino:

1. install configuration for using linux virtualbox for the arduino: http://joequery.me/guides/arduino-ubuntu-virtualbox-windows-host/
	a. for a mac, just go into your arduino ide and see what port path your arduino is connected to, such as: 
	     /dev/cu.usbmodem1411
	b. also just try unplugging it and plugging it back in before turning the vm back on

2. install required libraries for the NMEA stuff by going through all installation directions: https://github.com/ttlappalainen/NMEA2000/blob/master/Documents/NMEA2000_library_reference.pdf

3. 
  - (For Mac) Install Mono for your ubuntu environment (16.04) and run through the steps at the end of step 3 to check your installation (https://www.mono-project.com/download/stable) This is to help you run the windows applications in linux

  - (For PC) You should be able to just download based on the directions here: http://openskipper.org/openskipperwordpress/?page_id=6
