# Phenobottle
The Phenobottle is an open-source Raspberry Pi based tissue culture flask photobioreactor used for assessing growth and photophysiology in microalgae. 

![Phenobottle](https://github.com/Phenobottle/Phenobottle/blob/master/Phenobottle%20Image.png)

The first version of the Phenobottle (v0.1) was developed as a way of conducting high-time series physiological experiments with microalgae. Version 0.1 of the device features a range of environmental controls (light (quality/quantity), air(CO2) and mixing) and phyiological sensors (optical density (growth), chlorophyll fluorescence (photosynthesis)).

### Click [Here](https://youtu.be/7FyAfFrLizw) The Phenobottle in Action

[![PhenobottleGIF](https://j.gifs.com/mO31ZE.gif)](https://youtu.be/7FyAfFrLizw)

The architecture of the Phenobottle from both a software and hardware perspective can be adjusted to accommodate new sensors, novel operational routines, graphing procedures etc.

In this repository users can find:
1.	3D models for 3D printing a Phenobottle
2.	Electronic schematics for the custom Phenobottle circuit board
3.	Scripts for running the Phenobottles
4.	Additional information such as a bill of materials etc.

## Downloading files from this repository
As the Phenobottle is being updated constantly, please download the released versions of the device from the releases tab (located on the right hand side of the screen). This contains all the files needed to 3D-print and control Phenobottles as well as some information about the required equipment. 

If you wish to clone the main repository you may; however, as there are new versions of the Phenobottle in the works, the files may not work as planned.

[![DOI](https://zenodo.org/badge/248412211.svg)](https://zenodo.org/badge/latestdoi/248412211)

# Raspberry Pi Commands
## Python dependencies
A number of packages are included in the [Phenobottle_RaspberryPi.py](Phenobottle_RaspberryPi.py). Some of which are already installed on a Raspberry Pi and as such wont be discussed. Others need to be installed using the Raspberry Pi terminal before compiling, these include: 
### PyMySQL - For SQL connection
#### Terminal
```bash
pip3 install PyMySQL
```
#### Python
```python
import pymysql.cursors
```
This requires a Raspberry Pi to be configured as a dedicated SQL server. Information on how to do that can be found [here](https://pimylifeup.com/raspberry-pi-mysql/). It is recomended that you use a dedicated Raspberry Pi for the SQL server; however, if you are on a budget one of the Phenobottle's Raspberry Pi's can be used as a server.

### Adafruit_MotorHAT - For I2C communication with the [Adafruit-MotorHAT](https://www.adafruit.com/product/2348)
#### Terminal
```bash
git clone https://github.com/adafruit/Adafruit-Motor-HAT-Python-Library.git
cd Adafruit-Motor-HAT-Python-Library
python3 setup.py install
```
#### Python
```python
from Adafruit_MotorHAT import Adafruit_MotorHAT
```
Unfortunatly this package is depreciated; however, its still fine to use for the Phenobottle.

### Twilio - SMS messaging
#### Terminal
```bash
pip3 install twilio
```
#### Python
```python
from twilio.rest import Client
```
This package is not required if you do not want to recieve SMS updates. Twilio is a paid service that you must sign up for [here](https://www.twilio.com/). However, this is non-essential and the Phenobottle runs fine without this addition. This can be handy if you are away from The Phenobottle and need to know how the experiment is running. 

### pySerial - Serial (USB) communication with the microcontroller
#### Terminal
```bash
python3 -m pip install pyserial
```
#### Python
```python
import serial
```
Required for communication with the Teensy 3.6 microcontroller. Documentation on this package can be found [here](https://pyserial.readthedocs.io/en/latest/index.html). 

## Required setup in [Phenobottle_RaspberryPi.py](Phenobottle_RaspberryPi.py)
### Basic Setup
```python
PHENOBOTTLE_NUMBER = 3 # Arbitary number to differentiate between Phenobottles


INITIAL_OPTICAL_DENSITY = 888 # 12-bit value obtained by calibrating the optical density sensor

ARDUINO_FLUORESCENCE_REFERENCE = 3.3 # Teensy microntrollers have 2 analog reference values 1.1 and 3.3 

MIXING_SPEED = 32 # Mixing speed of the mixing motor (0 - 255)
BUBBLING_INTENSITY = 60 # Bubbling intensity of the bubbling motor (0 - 255)
LIGHT_INTENSITY = 220 # Overall light intensity of the grow lights (0 - 255) 
PERISTALTIC_SPEED = 160 # Speed of the peristaltic pump (0 - 255)
EXPERIMENT_START_TIME = "07:00:00" # Specify when the experiment should start 

ser = serial.Serial('/dev/ttyACM0', 115200) # Serial port and baud rate of Teensy 3.6 microcontroller
```
The `INITAL_OPTICAL_DENSITY` value is obtained by reading the raw value of optical density BEFORE adding microalgae to the flask. This can be acheived by writing "MeasureOpticalDensity" in the serial input of the Teensy 3.6 microcontroller (in the ArduinoIDE) or calling `Test.prelim_od()` in the python [file](Phenobottle_RaspberryPi.py). This is required to obtain a 100% transmission reading for which optical density measurments can be formulated from in the code. 

### SQL Setup
```python
connection = pymysql.connect(host='IP_ADDRESS', # IP address of Raspberry Pi SQL server
                             port=port_number, # Usually 3306
                             user='Username', # Username of phpmyadmin 
                             password='Password', # Password of phpmyadmin
                             db='Phenobottle No.%s' %PHENOBOTTLE_NUMBER, # Database name
                             charset='utf8mb4',
                             cursorclass=pymysql.cursors.DictCursor)
```
For a SQL server connection this command must be compiled. See [here](https://pypi.org/project/PyMySQL/) for some documentation related to PyMySQL. For the database name you can change this to whichever name you choose; however, here we have premaded databases in out SQL server named "Phenobottle No.1", "Phenobottle No.2" ..., so that the `PHENOBOTTLE_NUMBER` corresponds with the correct database. 

### Messaging Setup
```python
acc_sid = "Account SSID" # Accound SSID from Twilio
auth_token = "Account Token" # Accound token from Twilio

client = Client(acc_sid, auth_token)

from_number = "Twilio Phone Number" # Designated twilio phone number
to_number = "Mobile Phone Number" # Your phone number
```
As mentioned above, this part isn't really nessessary. However, information regarding the setup of SMS messaging can be found [here](https://www.twilio.com/).

## Experimental procedure
### Measurment Intervals
The Phenobottle is capable of measuring/controling environmental variables at described intervals. This is done through this set of commands:
```python
""" Here we can check if the current time in minutes is devisable by our required measuring interval in this case 10 minutes. This means every 10 minutes a routine (day or night) will be run."""
if datetime.datetime.now().minute % 10 == 0: 
    """ Here we can setup or morning (8:00) and night times (20:00)"""
    if Experiment.experiment_schedule(datetime.time(8, 00), datetime.time(20, 00)): 
        NOW = datetime.datetime.now()
        Experiment.day_routine() # Runs the day routine
    else:
        NOW = datetime.datetime.now() 
        Experiment.night_routine() # Runs the night routine
```
### Routines
The day and the night routine are idential except for the fact that during the night the LED grow lights are constantly switched OFF. 
```python
MotorsAndLights.light_on(None) # Turn on lights (daytime only) with desired color
MotorsAndLights.mixing_motor_on() # Mixing on at set speed
time.sleep(30) # Wait 30 seconds for mixing 

MotorsAndLights.mixing_motor_off() # Mixing off
MotorsAndLights.light_off() # Lights off to ensure no noise from PWM interferes with measurments
Sensors.measure_optical_density() # Measures optical density
Sensors.measure_fluorescence() # Measures fluorescence

MotorsAndLights.light_on(None) # Turn on lights (daytime only) with desired color
MotorsAndLights.peristaltic_motor_on() # Pump on to clear cuvette
Database.upload() # Upload to SQL server
Excel.upload() # Upload to local .csv file
time.sleep(20) # Wait for pump to clear cuvette 

MotorsAndLights.peristaltic_motor_off() # Stop pump
MotorsAndLights.bubbling_motor_on() # Bubble main culture flask on
MotorsAndLights.mixing_motor_on() # Mixing on to homogenate culture
time.sleep(30) # Wait 30 seconds for bubbling and mixing 

MotorsAndLights.bubbling_motor_off() # Bubbling off
MotorsAndLights.mixing_motor_off() # Mixing off
```
This procedure is completely customisable, i.e. methods can be swapped around and the length of certain processes such as the bubbling time can be increased easily. 

# [Teensy 3.6](https://www.pjrc.com/store/teensy36.html) Microcontroller 
Analog functions are carried out by a Teensy 3.6 microcontroller which is connected to the Rasberry Pi via a serial (USB) connection. These analog functions include the measurement of optical density (growth), temperture and chlorophyll fluorescence (photosynthesis). 

The Teensy microcontroller can be programed with the [Arduino IDE](https://www.arduino.cc/en/Main/Software) on the Raspberry Pi if the additional software provided by [PJRC](https://www.pjrc.com/) called [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html) is installed. 

The Teensy [code](Phenobottle_Teensy.ino) is flashed to the Teensy board and doesn't need modifying as it acts as a slave to the Raspberry Pi. 

## Teensy Setup
```c
void setup() {
  Serial.begin(115200); // Baud rate must match the baud rate in the .py script
  analogReadResolution(12); // Set the resolution of the analog measurments (can be up-to 16-bit but this sacrifices speed)
  analogReference(DEFAULT); // Default (3.3) or 1.1 V reference voltage for the analog measurements
  ...
}
```
## Teensy Slave Setup
The Teensy is constantly checking if there is requests in its serial port from the Raspberry Pi. It does this by the main loop:
```c
void loop(){
  if(Serial.available()){ // Check serial port
    command = Serial.readStringUntil('\n'); // Read request
    if(command.equals("MeasureOpticalDensity")){ // Request to measure optical density
      measureOpticalDensity(); // Measure
    }
    else if(command.equals("MeasureFluorescence")){ // Request to measure fluorescence
      measureFluoro(); // Measure
    }
    ...
}
```
This simple design makes it easy to add new requests from the Raspberry Pi [.py](Phenobottle_RaspberryPi.py) file and the Teensy [.ino](Phenobottle_Teensy.ino) file by adding a new `else if` statement to this loop. 

## Fluorescence Measurment
### Setup
The Teensy in its current configuration takes a one second measurement of chlorophyll fluorescence, this however, can be adjusted as described below. The first part is to create arrays of different sizes corresponding to the time between measurements of fluorescence. The one second measurement is comprised of over 2000 data points which are comprised of baseline, fast (microsecond timescales) and slow (millisecond timescales) measurments:
1. Baseline - Reads fluorescence intensity while the actinic LED is switched off (mostly used for debugging purposes)
2. Micro - Comprised of microsecond measurments (~8 us between measurments) of a set amount
3. Milli - Comprised of millisecond measurements (~1 ms between measurements) of a set amount
```c
int baseRead[5]; // Create a baseline array with 5 values
int microRead [1000]; // Create a microread array with 1000 values
int milliRead[1000]; // Create a milliread array with 1000 values

int baseCycles = 5; // Set the size of the array (can just use sizeof(baseRead))
int microCycles = 1000; // Set the size of the array (can just use sizeof(microRead))
int milliCycles = 1000; // Set the size of the array (can just use sizeof(milliRead))

float h[5]; // Create an identially sized array of baseRead for the timepoints
float t[1000]; // Create an identially sized array of microRead for the timepoints
float p[1000]; // Create an identially sized array of milliRead for the timepoints
```
### Read
The measurment procedure is as follows:
```c
void measureFluoro() {
  analogReference(DEFAULT); // Set the required analog reference voltage 
  delay(10); // Pause
  analogRead(readPin); // Single read to initiate the reference voltage
  delay (1000); // Pause
  digitalWrite(actinicPin, HIGH); // Turn on actinic LED
  long timer = micros(); // Start the timer
  ...
```
The measurment is carried out in two seperate parts (1) `microRead` and (2) `milliRead` (`baseRead` is not used for measuring). The `analogRead` values are stored in their respective arrays with corresponding timestamps in their own array.
```c
for (int i = 0; i < microCycles; i++) // Micro reads for desired length
  {
    microRead[i] = analogRead(readPin); // Store analog values
    t[i] = micros() - timer; // Stores timestamps
    // No delay needed here as we want to measure FAST
  }
for (int o = 0; o < milliCycles; o++) // Milli reads for desired length
  {
    milliRead[o] = analogRead(readPin); // Store analog values
    p[o] = micros() - timer; // Stores timestamps
    delay(1); // Millisecond delay between measurments
  }
  ...
```
Any processing (converting timestamps from `micros` to milliseconds) of the arrays after this measurment is carried out after the actinic LED is switched off. This speeds up the `analogRead` function as we only need to store array values while measuring. 

## Optical Density
Measurment of optical density is really simple. 
```c
void measureOpticalDensity(){
  digitalWrite(ODemitPower, HIGH); // Set the emitting IR-LED ON
  analogReference(DEFAULT); // Set the analog reference voltage
  delay(1000);
  analogRead(odRead); // Read to initalise the reference voltage
  delay(100);
  ODMeasure = analogRead(odRead); // Read optical density
  Serial.println(ODMeasure); // Print out OD for the Raspberry Pi to read
  delay(10); 
  digitalWrite(ODemitPower, LOW); // Turn off the IR-LED
  delay(100);
}
```

# Future of The Phenobottle
The Phenobottle is currently being updated to version 0.2. This will feature several environmental controls that were not featured the original version (v0.1) of the device. Additonal environmental sensors will also feature in version 0.2 along with a graphical user interface. 

If you would like to learn more about the progress of this version please email: [harvey_bates@hotmail.com](harvey_bates@hotmail.com) or [harvey.bates@student.uts.edu.au](harvey.bates@student.uts.edu.au)

# Open-Source Licensing

The software in this work is licensed under GNU AFFERO GENERAL PUBLIC LICENSE
  Copyright (C) (2020)  (Harvey Bates)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>

  For more information contact: harvey.bates@student.uts.edu.au

<a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/3.0/88x31.png" /></a><br />The hardware (3D Models, Electronic Schematics) in this work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/">Creative Commons Attribution-ShareAlike 3.0 Unported License</a>.
