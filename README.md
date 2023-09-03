![Phenobottle](media/Logo.png)

![Phenobottle](media/PhenobottleRender.png)

---

### Description

An open-source Raspberry-Pi based photobioreactor for cultivating and assessing physiology (photosynthesis & productivity) in microscopic algae. The device can be used to cultivate both green algae and cyanobacteria while continuiously (and non-invasively) probing photosynthetic and growth phenotypes.

#### Publication

The device was published in [Algal Research in 2020](https://www.sciencedirect.com/science/article/abs/pii/S2211926420309735):

*The Phenobottle, an open-source photobioreactor platform for environmental simulation - Bates et al. 2020*

[![DOI](https://zenodo.org/badge/248412211.svg)](https://zenodo.org/badge/latestdoi/248412211)

#### Future of The Phenobottle

The Phenobottle is currently being updated to version 0.2. This will feature several environmental controls that were not featured the original version (v0.1) of the device. Additonal environmental sensors will also feature in version 0.2 along with a graphical user interface (GUI). 

If you would like to learn more about the progress of this version please email: [harvey_bates@hotmail.com](harvey_bates@hotmail.com)

---

### [Live](https://youtu.be/7FyAfFrLizw) Preview

[![PhenobottleGIF](https://j.gifs.com/mO31ZE.gif)](https://youtu.be/7FyAfFrLizw)

___

# Operating Manual

## Specifications

| Feature                                                      | Value / Description                                          |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| Growth light quality                                         | Red, green and blue (Adjustable). 8-bit individual adjustment for each channel. |
| Growth light intensity                                       | Up to 200 &mu;E (can be increased if needed).                |
| Mixing                                                       | Vertical mixing using magnetic stirrer (adjustable intensity) |
| Bubbling                                                     | Included bubbling motor (adjustable) or solenoid control for external supply of CO<sub>2</sub> |
| Main board                                                   | Raspberry Pi 3B+ (can accomidate the new 4 - 4B)             |
| Microcontroller                                              | Teensy 3.5/3.6 or Teensy 4.1 (3.6 Recommended)               |
| Actinic LED colors                                           | **Blue** 455 – 485 nm (466 nm), **Green** 517 – 555 nm (532 nm), **Orange** 583 – 600 nm (593 nm), **Red** 617 – 635 nm (626 nm). |
| Resolution of **optical density** and **chlorophyll fluorescence** measurments | 10 to 16-bit (adjustable)                                    |
| Sampling rate of fluorometer                                 | 8 &mu;s at 12-bits (can be overclocked)                      |
| Operating voltage                                            | 15 V 1.2 A (Barrel plug), 12 V 2 A (Barrel Plug) and a 5 V 2 A (MicroUSB) power supply |
| Data collection                                              | SQL Server and local .csv                                    |

## Downloading files from this repository

As the Phenobottle is being updated constantly, please download a released version of the device from the releases tab (located on the right hand side of the screen). This contains all the files needed to 3D-print and control Phenobottles as well as some information about the required materials and tools. 

If you wish to clone the main repository you may; however, as there are new versions of the Phenobottle in the works, the files may not work as planned.

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
This requires a Raspberry Pi to be configured as a dedicated SQL server. Information on how to do that can be found [here](https://pimylifeup.com/raspberry-pi-mysql/). It is recomended that you use a dedicated Raspberry Pi for the SQL server; however, you can use one of the Phenobottle's Raspberry Pi's as a server if you are limited.

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
```c++
void setup() {
  Serial.begin(115200); // Initalise serial communications at specific baud rate
  analogReadResolution(12); // Set the resolution of the microcontroller in bits
  set_reference_voltage(refVoltage); // Only applicable with a Teensy 3.6 (disable if using other microcontroller)
  ...
}
```
## Teensy Slave Setup
The Teensy is constantly checking if there is requests in its serial port from the Raspberry Pi. It does this via the main loop:
```c++
void loop(){
  if(Serial.available()){   
    command = Serial.readStringUntil('\n');
    if(command.equals("MeasureOpticalDensity") || command.equals("MOD")){
      measure_optical_density();
    }
    else if(command.equals("MeasureFluorescence") || command.equals("MF")){
      measure_fluorescence();
    }
    ...
}
```
For example, when a string containing ```"MeasureOpticalDensity"``` into the Teensy's serial port, the resulting function ```measure_optical_density()``` is carried out. This simple design makes it easy to add new requests from the Raspberry Pi [.py](Phenobottle_RaspberryPi.py) file and the Teensy [.ino](Phenobottle_Teensy.ino) file by adding a new `else if` statement to this loop. 

## Fluorescence Measurment
### Setup
The Teensy in its current configuration takes a one second measurement of chlorophyll fluorescence, this however, can be adjusted as described below. The first part is to create arrays of different sizes corresponding to the time between measurements of fluorescence. The one second measurement is comprised of over 2000 data points which are comprised of fast (microsecond timescales) and slow (millisecond timescales) measurments:
2. Micro - Comprised of microsecond measurments (~8 &mu;s between measurments) of a set amount
3. Milli - Comprised of millisecond measurements (~1 ms between measurements) of a set amount
```c++
// Setup arrays that hold fluorescence data
int microRead [1000];
int milliRead[1000];

// Setup arrays that hold timestamps that correspond to fluorescence values
float microTime[1000]; 
float milliTime[1000];
String command;

int microLength = 1000, milliLength = 1000; // Change these to match the size of the above arrays
```
### Read
The measurment procedure is as follows:
```c++
void measure_fluorescence() {
  set_reference_voltage(refVoltage); 
  digitalWrite(actinicPin, HIGH); // Turn on actinic LED
  long timer = micros(); // Start timer 
  ...
```
The measurment is carried out in two seperate parts (1) `microRead` and (2) `milliRead`. The `analogRead` values are stored in their respective arrays with corresponding timestamps in their own array.
```c++
  // Read microsecond fluorescence values and corresponding timestamps
  for (unsigned int i = 0; i < sizeof(microRead) / sizeof(int); i++) 
  {
    microRead[i] = analogRead(fluorescenceReadPin);
    microTime[i] = micros() - timer;
  }

  // Read millisecond fluorescence values and corresponding timestamps
  for (unsigned int i = 0; i < sizeof(milliRead) / sizeof(int); i++) 
  {
    milliRead[i] = analogRead(fluorescenceReadPin);
    milliTime[i] = micros() - timer;
    delay(1); // Delay ~ 1ms between aqusisitions
  }
  ...
```
Any processing (converting timestamps from `micros` to milliseconds) of the arrays after this measurment is carried out after the actinic LED is switched off. This speeds up the `analogRead` function as we only need to store array values while measuring. 

## Optical Density
Measurment of optical density is really simple. 
```c++
void measure_optical_density(){
  set_reference_voltage(refVoltage);
  digitalWrite(odEmitPin, HIGH); // Turn on emitting LED
  delay(500); // Slight delay to ensure emitting LED is stable
  int odMeasure = analogRead(odReadPin); // Read optical density
  Serial.println(odMeasure);
  digitalWrite(odEmitPin, LOW); // Turn off emitting LED
}
```

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

<a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/3.0/88x31.png" /></a>

Hardware (3D Models, Electronic Schematics) in this work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/">Creative Commons Attribution-ShareAlike 3.0 Unported License</a>.
