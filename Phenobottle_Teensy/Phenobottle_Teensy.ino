/* <Phenobottle Teensy Script. Controls "The Phenobottles" sensors>
     Copyright (C) <2020>  <Harvey Bates>

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
     
     For more information contact: harvey.bates@student.uts.edu.au or see
     https://github.com/HarveyBates/Open-JIP 
*/ 

// Analog Pins
#define fluorescenceReadPin 33
#define odReadPin 34
#define temperatureReadPin 35

// Digital Pins
#define actinicPin 21
#define odEmitPin 23
#define odDetectPin 22
#define temperaturePower 20

//RGB Control 
#define redPin 36
#define greenPin 37
#define bluePin 38

// Setup arrays that hold fluorescence data
int microRead [1000];
int milliRead[1000];

// Setup arrays that hold timestamps that correspond to fluorescence values
float microTime[1000]; 
float milliTime[1000];
String command;

int microLength = 1000, milliLength = 1000; // Change these to match the size of the above arrays

float refVoltage = 3.3; // Set the reference voltage (only applicable with Teensy 3.6)

void setup() {
  Serial.begin(115200); // Initalise serial communications at specific baud rate
  
  analogReadResolution(12); // Set the resolution of the microcontroller in bits
  set_reference_voltage(refVoltage); // Only applicable with a Teensy 3.6 (disable if using other microcontroller)
  
  pinMode(temperaturePower, OUTPUT);
  pinMode(odEmitPin, OUTPUT);
  pinMode(odDetectPin, OUTPUT);
  pinMode(actinicPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
 
  digitalWrite(actinicPin, LOW);
  digitalWrite(odEmitPin, LOW);
  digitalWrite(odDetectPin, HIGH);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

void LED_light_on(int red, int redFreq, int green, int greenFreq, int blue, int blueFreq){
  analogWrite(red, redFreq);
  analogWrite(green, greenFreq);
  analogWrite(blue, blueFreq);
  if (redFreq == 0){
    digitalWrite(red, LOW);
  }
  if (greenFreq == 0){
    digitalWrite(green, LOW);
  }
  if (blueFreq == 0){
    digitalWrite(blue, LOW);
  }
}

void LED_light_off(int color){
  digitalWrite(color, LOW);
}

void rgb_lights_off(){
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

String getValue(String data, char separator, int index){
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void measure_optical_density(){
  digitalWrite(odEmitPin, HIGH);
  set_reference_voltage(refVoltage);
  delay(1000);
  analogRead(odReadPin);
  delay(100);
  int odMeasure = analogRead(odReadPin);
  Serial.println(odMeasure);
  delay(10);
  digitalWrite(odEmitPin, LOW);
  delay(100);
}

void set_reference_voltage(float voltage){
  // Sets and initalises the required reference voltage for measurments
  if(voltage == 3.3){
    analogReference(DEFAULT); // Set to 3.3 V
  }
  else if(voltage == 1.1){
    analogReference(INTERNAL1V1); // Set to 1.1 V
  }
  else{
    analogReference(DEFAULT); // Set to default (3.3 V) if unknown value is found
  }
  analogRead(fluorescenceReadPin); // Initalise reference voltage
}

void measure_fluorescence() {
  set_reference_voltage(refVoltage); 
  
  digitalWrite(actinicPin, HIGH); // Turn on actinic LED

  long timer = micros(); // Start timer 

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
    delay(1);
  }
  
  digitalWrite(actinicPin, LOW); // Turn off actinic LED
  delay(10);

  // Convert micros() to milliseconds (ms) for microsecond values and convert bits to voltage
  for (unsigned int i = 0; i < sizeof(microRead) / sizeof(int); i++)
  {
   float milliReal = microTime[i]/1000; // Convert micros() to ms
   Serial.print(milliReal, 3); 
   Serial.print("\t");
   Serial.println(microRead[i]);
   delay(1);
  }

  // Convert micros() to milliseconds for millsecond values and convert bits to voltage
  for (unsigned int i = 0; i < sizeof(milliRead) / sizeof(int); i++) 
  {
   float milliReal = milliTime[i]/1000; // Convert micros() to ms
   Serial.print(milliReal, 3); 
   Serial.print("\t");
   Serial.println(milliRead[i]);
   delay(1);
  }
}

void measure_light(){
  digitalWrite(actinicPin, HIGH);
  delay(3000);
  digitalWrite(actinicPin, LOW);
  delay(20);
}

void loop(){
  if(Serial.available()){   
    command = Serial.readStringUntil('\n');
    if(command.equals("MeasureOpticalDensity") || command.equals("MOD")){
      measure_optical_density();
    }
    else if(command.equals("MeasureFluorescence") || command.equals("MF")){
      measure_fluorescence();
    }
    else if(command.equals("MeasureLight") || command.equals("ML")){
      measure_light();
    }
    else if(command.equals("RGBOFF")){
      rgb_lights_off();
    }
    else if(command.startsWith("LED_light_ON")){
      String red_col = getValue(command, ';', 1);
      String red_freq = getValue(command, ';', 2); 
      String green_col = getValue(command, ';', 3);
      String green_freq = getValue(command, ';', 4); 
      String blue_col = getValue(command, ';', 5);
      String blue_freq = getValue(command, ';', 6);      
      LED_light_on(red_col.toInt(), red_freq.toInt(), green_col.toInt(), 
      green_freq.toInt(), blue_col.toInt(), blue_freq.toInt());
    }
    else if(command.startsWith("LED_light_OFF")){
      String col = getValue(command, ';', 1);
      LED_light_off(col.toInt());
    }
    else{
      delay(100);
    }
  }
}
