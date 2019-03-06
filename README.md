# sailbot1819NEW

Steps for setting up and running NMEA CAN bus on arduino:

1. Install configuration for using linux virtualbox for the arduino: http://joequery.me/guides/arduino-ubuntu-virtualbox-windows-host/
	- For a Mac, just go into the Arduino IDE and see what port path your Arduino is connected to, such as: 
	     /dev/cu.usbmodem1411
	- When you connect the Arduino to the port in the settings, you will not be able to turn on your VM unless the Arduino is plugged in. If you're having issues, or you get an error message, just unplug the Arduino, reset the VM, and plug the Arduino back in. This should solve the issues.


2. Install required libraries for the NMEA stuff by going through all installation directions: https://github.com/ttlappalainen/NMEA2000/blob/master/Documents/NMEA2000_library_reference.pdf
   
3. Download OpenSkipper to be able to read data from testing: http://openskipper.org/openskipperwordpress/?page_id=6. This reads the NMEA2000 CAN signals being sent out from the Arduino and shows it in a visual manner.

4.  - (For Mac) Install Mono for your ubuntu environment (16.04) and run through the steps at the end of step 3 to check your installation (https://www.mono-project.com/download/stable) This is to help you run the windows applications in linux

    - (For PC) You should be able to just download based on the directions here: http://openskipper.org/openskipperwordpress/?page_id=6

