# <Phenobottle Raspberry Pi Script. Operates "The Phenobottle" which is an Open-source Photobioreactor>
    # Copyright (C) <2020>  <Harvey Bates>

    # This program is free software: you can redistribute it and/or modify
    # it under the terms of the GNU General Public License as published by
    # the Free Software Foundation, either version 3 of the License, or
    # (at your option) any later version.

    # This program is distributed in the hope that it will be useful,
    # but WITHOUT ANY WARRANTY; without even the implied warranty of
    # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    # GNU General Public License for more details.

    # You should have received a copy of the GNU General Public License
    # along with this program.  If not, see <https://www.gnu.org/licenses/>
    
    # For more information contact: harvey.bates@student.uts.edu.au

import pymysql.cursors
import datetime
import atexit
import serial
import time
import csv
import math
from Adafruit_MotorHAT import Adafruit_MotorHAT
from twilio.rest import Client
from time import strftime

# -------------------------------------------- Setup Required Below -------------------------------------------- #

PHENOBOTTLE_NUMBER = 3

INITIAL_LIGHT_INTENSITY = 500
INITIAL_OPTICAL_DENSITY = 888

ARDUINO_FLUORESCENCE_REFERENCE = 3.3

MIXING_SPEED = 32
BUBBLING_INTENSITY = 60
LIGHT_INTENSITY = 220
PERISTALTIC_SPEED = 160
EXPERIMENT_START_TIME = "07:00:00"

ser = serial.Serial('/dev/ttyACM0', 115200)

# ------------------------------------------------ End of Setup ------------------------------------------------ #
# --------------------------------------------- Motor Control Setup ------------------------------------------------ #

MOTOR_INDEX = Adafruit_MotorHAT(addr=0x60)
MIXING_MOTOR = MOTOR_INDEX.getMotor(1)
PERISTALTIC_MOTOR = MOTOR_INDEX.getMotor(2)
BUBBLING_MOTOR = MOTOR_INDEX.getMotor(3)
LIGHT_CONTROL = MOTOR_INDEX.getMotor(4)

# --------------------------------------------- Experimental Setup ------------------------------------------------ #

NOW = datetime.datetime.now()
experiment_datetime = datetime.datetime.strptime(EXPERIMENT_START_TIME, "%H:%M:%S")
experiment_datetime = NOW.replace(hour=experiment_datetime.time().hour, minute=experiment_datetime.time().minute,
                                  second=experiment_datetime.time().second, microsecond=0)


# ------------------------------------------------- Database Setup ------------------------------------------------- #

connection = pymysql.connect(host='IP_ADDRESS',
                             port=port_number,
                             user='Username',
                             password='Password',
                             db='Phenobottle No.%s' %PHENOBOTTLE_NUMBER,
                             charset='utf8mb4',
                             cursorclass=pymysql.cursors.DictCursor)

# --------- SMS Messaging Setup --------- #

acc_sid = "Account SSID"
auth_token = "Account Token"

client = Client(acc_sid, auth_token)

from_number = "Twilio Phone Number"
to_number = "Mobile Phone Number"

message_sent = False

# --------- Initialise with some fake values --------- #

time_now = str(datetime.datetime.now().strftime("%Y/%m/%d %H:%M:%S"))
day_night = "N/a"
optical_density, transmittance, od_raw, temperature = 0, 0, 0, 0
fo, f_300us, fj, fi, fm, variable_fluorescence, quantum_yield = 0, 0, 0, 0, 0, 0, 0
vj, fm_qa, mo, performance_index = 0, 0, 0, 0
fj_fo, fi_fo, fi_fj, fm_fi = 0, 0, 0, 0
fo_od, fj_od, fi_od, fm_od = 0, 0, 0, 0
variable_fluorescence_od = 0
time_ojip, value_ojip, norm_ojip = [0, 0, 0], [0, 0, 0], [0, 0, 0]


class MotorsAndLights:
    @staticmethod
    def peristaltic_motor_on():
        PERISTALTIC_MOTOR.run(Adafruit_MotorHAT.FORWARD)
        PERISTALTIC_MOTOR.setSpeed(PERISTALTIC_SPEED)

    @staticmethod
    def peristaltic_motor_off():
        PERISTALTIC_MOTOR.run(Adafruit_MotorHAT.RELEASE)

    @staticmethod
    def bubbling_motor_on():
        BUBBLING_MOTOR.run(Adafruit_MotorHAT.BACKWARD)
        BUBBLING_MOTOR.setSpeed(BUBBLING_INTENSITY)

    @staticmethod
    def bubbling_motor_off():
        BUBBLING_MOTOR.run(Adafruit_MotorHAT.RELEASE)

    @staticmethod
    def mixing_motor_on():
        MIXING_MOTOR.run(Adafruit_MotorHAT.BACKWARD)
        MIXING_MOTOR.setSpeed(MIXING_SPEED)

    @staticmethod
    def mixing_motor_off():
        MIXING_MOTOR.run(Adafruit_MotorHAT.RELEASE)

    @staticmethod
    def light_on():
        LIGHT_CONTROL.run(Adafruit_MotorHAT.BACKWARD)
        LIGHT_CONTROL.setSpeed(int(LIGHT_INTENSITY))
        ser.flush()
        str_red = bytes(str(LIGHT_RED), "utf-8")
        str_green = bytes(str(LIGHT_GREEN), "utf-8")
        str_blue = bytes(str(LIGHT_BLUE), "utf-8")
        ser.write(b'LED_light_ON;%s;%s;%s;%s;%s;%s' %("36".encode("utf-8"), 
        	str_red, "37".encode("utf-8"), str_green, "38".encode("utf-8"), str_blue))

    @staticmethod
    def light_off(color=None):
        LIGHT_CONTROL.run(Adafruit_MotorHAT.RELEASE)
        ser.flush()
        if color == "Red":
            ser.write(b'LED_light_OFF;%s' %"36".encode("utf-8"))
        if color == "Green":
            ser.write(b'LED_light_OFF;%s' %"37".encode("utf-8"))
        if color == "Blue":
            ser.write(b'LED_light_OFF;%s' %"38".encode("utf-8"))
        if color is None:
            pass


class Sensors:
    @staticmethod
    def measure_and_adjust_light_intensity():
        global LIGHT_INTENSITY
        global light_intensity_a
        ser.flush()
        time.sleep(1)
        ser.write(b'MeasureLightIntensity')
        ser_bytes = ser.readline(20)
        decoded_bytes = str(ser_bytes[0:len(ser_bytes) - 2].decode("utf-8"))
        print("Light Intensity = ", decoded_bytes)
        data_split = [float(s) for s in decoded_bytes.split(" , ")]
        light_intensity_a = data_split[0]
        light_max = light_intensity_a + 5
        light_min = light_intensity_a - 5
        if INITIAL_LIGHT_INTENSITY > light_max:
            print("Light Too bright")
            LIGHT_INTENSITY = LIGHT_INTENSITY - 1
            return
        if INITIAL_LIGHT_INTENSITY < light_min:
            print("Light too dim")
            LIGHT_INTENSITY = LIGHT_INTENSITY + 1
            return
        else:
            print("Light Intensity Optimal")
            return

    @staticmethod
    def measure_light_intensity():
        global light_intensity_a
        ser.flush()
        time.sleep(1)
        ser.write(b'MeasureLightIntensity')
        print("Waiting for response...")
        ser_bytes = ser.readline(20)
        light_intensity_a = str(ser_bytes[0:len(ser_bytes) - 2].decode("utf-8"))
        print("Light Intensity = ", light_intensity_a)


    @staticmethod
    def measure_optical_density():
        global od_raw, transmittance, optical_density
        ser.flush()
        time.sleep(1)
        ser.write(b'MeasureOpticalDensity')
        optical_density_bytes = ser.readline(20)
        decoded_optical_density_bytes = str(optical_density_bytes[0:len(optical_density_bytes) - 2].decode("utf-8"))
        try:
            data_split = [float(s) for s in decoded_optical_density_bytes.split(",")]
            od_raw = data_split[0]
            initial_transmittance = od_raw / INITIAL_OPTICAL_DENSITY
            transmittance = initial_transmittance * 100
            transmittance = round(transmittance, 2)
            try:
                calc_optical_density = (-math.log10(initial_transmittance))
            except:
                calc_optical_density = 0
            optical_density = round(calc_optical_density, 3)
        except ZeroDivisionError: 
            od_raw = 0
            transmittance = 0
            optical_density = 0

        print("Bit Count OD = ", od_raw)
        print("Transmittance (%) = ", transmittance)
        print("Optical Density = ", optical_density)

    @staticmethod
    def measure_temperature():
        global temperature
        ser.flush()
        time.sleep(1)
        ser.write(b'MeasureTemperature')
        temperature_bytes = ser.readline(20)
        decoded_temperature_bytes = str(temperature_bytes[0:len(temperature_bytes) - 2].decode("utf-8"))
        data_split = [float(s) for s in decoded_temperature_bytes.split(",")]
        temperature = data_split[0]
        print("Temperature = ", temperature)

    @staticmethod
    def measure_fluorescence():
        global fo, f_300us, fj, fi, fm, variable_fluorescence, quantum_yield, vj, fm_qa, mo, performance_index, fj_fo, fi_fo, fi_fj, fm_fi, fo_od, fj_od, fi_od, fm_od, variable_fluorescence_od, time_ojip, value_ojip, norm_ojip
        ser.flush()
        time.sleep(1)
        ser.write(b'MeasureFluorescence')
        time_ojip = []
        value_ojip = []
        norm_ojip = []
        for _ in range(2000):
            fluorescence_bytes = ser.readline()
            decoded_fluorescence_bytes = str(fluorescence_bytes[0:len(fluorescence_bytes) - 2].decode("utf-8"))
            data_split = [float(s) for s in decoded_fluorescence_bytes.split("\t")]
            x_data = data_split[0]
            y_data = float(data_split[1])
            y_data = ((y_data * FLUORESCENCE_REFERENCE) / 4096)
            time_ojip.append(x_data)
            value_ojip.append(round(y_data, 3))

# -------- Calculate Basic Fluorescence Parameters -------- #
        fo = value_ojip[4]
        f_300us = value_ojip[36]
        fj = value_ojip[248]
        fi = value_ojip[1023]
        fm = max(value_ojip[10:]) # Ignores the first few values as there can be a peak at 1us due to the amplifier
        fm_fi = (fm - fi)
        fi_fj = (fi - fj)
        fj_fo = (fj - fo)
        fm_fj = (fm - fj)
        fi_fo = (fi - fo)
        variable_fluorescence = (fm - fo)

# -------- Calculate vj -------- #
        try:
            vj = (fj - fo) / variable_fluorescence
            vj = round(vj, 2)
        except ZeroDivisionError:
            vj = 0

# -------- Calculate Quantum Yield -------- #
        try:
            quantum_yield = variable_fluorescence / fm
            quantum_yield = round(quantum_yield, 2)
        except ZeroDivisionError:
            quantum_yield = 0

# -------- Calculate Mo -------- #
        try:
            mo = 4 * ((f_300us - fo) / variable_fluorescence)
            mo = round(mo, 3)
        except ZeroDivisionError:
            mo = 0

# -------- Calculate Perfomance Index -------- #
        try:
            performance_index = ((1 - (fo / fm)) / (mo / vj)) * ((fm - fo) / fo) * ((1 - vj) / vj)
            performance_index = round(performance_index, 3)
        except ZeroDivisionError:
            performance_index = 0

# -------- Calculate FmQa -------- #
        try:
            fm_qa = (fj - fo) / fj
            fm_qa = round(fm_qa, 3)
        except ZeroDivisionError:
            fm_qa = 0

# -------- Create Normalised OJIP Array -------- #
        try:
            for e in range(len(value_ojip)):
                rvf = ((value_ojip[e] - fo) / (fm - fo))
                norm_ojip.append(round(rvf, 3))
        except ZeroDivisionError:
            norm_ojip = 0

# -------- Calculate Optical Density Normalised Values -------- #
        try: 
            fo_od = round((fo / optical_density), 3)
        except ZeroDivisionError:
            fo_od = 0

        try:
            fj_od = round((fj / optical_density), 3)
        except ZeroDivisionError:
            fj_od = 0

        try:
            fi_od = round((fi / optical_density), 3)
        except ZeroDivisionError:
            fi_od = 0

        try:
            fm_od = round((fm / optical_density), 3)
        except ZeroDivisionError:
            fm_od = 0

        try:
            variable_fluorescence_od = round((variable_fluorescence / optical_density), 3)
        except:
            variable_fluorescence_od = 0

# -------- Test if fluorometer is saturating -------- #
        Readings.test_saturation()


class Test:
    @staticmethod
    def motors_and_lights():
        MotorsAndLights.mixing_motor_on()
        time.sleep(30)
        MotorsAndLights.mixing_motor_off()
        MotorsAndLights.bubbling_motor_on()
        time.sleep(10)
        MotorsAndLights.bubbling_motor_off()
        MotorsAndLights.peristaltic_motor_on()
        time.sleep(10)
        MotorsAndLights.peristaltic_motor_off()

    @staticmethod
    def sensors():
        Sensors.measure_fluorescence()
        Sensors.measure_optical_density()
        Sensors.measure_temperature()
        Sensors.measure_light_intensity()

    @staticmethod
    def calibrate_od():
        MotorsAndLights.light_on(None)
        MotorsAndLights.mixing_motor_on()
        MotorsAndLights.peristaltic_motor_on()
        time.sleep(10)
        MotorsAndLights.mixing_motor_off()
        MotorsAndLights.peristaltic_motor_off()
        MotorsAndLights.light_off()
        time.sleep(2)
        Sensors.measure_optical_density()
        time.sleep(1)

    @staticmethod
    def prelim_od():
        bottle_in_slot = input("Is a culture flask with media in Phenobottle slot? (y or n)")
        if bottle_in_slot == "y":
            print("Measuring Intial Transmittance (3 Times) Please Wait...")
            print(1)
            Sensors.measure_optical_density()
            time.sleep(1)
            print(2)
            Sensors.measure_optical_density()
            time.sleep(1)
            print(3)
            Sensors.measure_optical_density()
            time.sleep(1)
        else:
            print("Exiting...")


class Database:
    @staticmethod
    def upload():
        global time_ojip, value_ojip, norm_ojip
        time_ojip = ", ".join(str(x) for x in time_ojip)
        value_ojip = ", ".join(str(x) for x in value_ojip)
        norm_ojip = ", ".join(str(x) for x in norm_ojip)
        try:
            with connection.cursor() as cursor:
                sql_time = str(datetime.datetime.now().strftime("%Y/%m/%d %H:%M:%S"))
                sql = "INSERT INTO `Advanced Parameters` (`Time Now`, `Day Night`, `Optical Density RAW`, `Transmittance`, `Optical Density`, `Temperature`, `Light Intensity`, `Fo`, `F_300us`, `Fj`, `Fi`, `Fm`, `Fv`, `Fv/Fm`, `Vj`, `FmQa`, `Mo`, `PIabs`, `Fj - Fo`, `Fi - Fo`, `Fi - Fj`, `Fm - Fi`, `Fo / OD`, `Fj / OD`, `Fi / OD`, `Fm / OD`, `Fv / OD`) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)"
                cursor.execute(sql, (sql_time, day_night, od_raw, transmittance, optical_density,  temperature, LIGHT_INTENSITY, fo, f_300us, fj, fi, fm, 
                    variable_fluorescence, quantum_yield, vj, fm_qa, mo, performance_index, fj_fo, fi_fo, fi_fj, fm_fi, fo_od, fj_od, fi_od, fm_od, 
                    variable_fluorescence_od))
                sql = "INSERT INTO `OJIP Curves` (`Time Now`, `Day Night`, `Time OJIP`, `Raw OJIP`, `Normalised OJIP`) VALUES (%s, %s, %s, %s, %s)"
                cursor.execute(sql, (sql_time, day_night, time_ojip, value_ojip, norm_ojip))
                
                print("Sent to SQLdb server")
            connection.commit()

        except:
            print("SQLdb not avaliable... check db server... writing to csv instead")

    def close_database(self):
        connection.close()


class Excel:
    @staticmethod
    def upload():
        global day_night
        worksheet_name = "Phenobottle No.%s.csv" % PHENOBOTTLE_NUMBER
        with open(worksheet_name, 'a') as f:
            try:
                writer = csv.writer(f)
                spreadsheet_time = str(datetime.datetime.now().strftime("%Y/%m/%d %H:%M:%S"))
                writer.writerow([spreadsheet_time, day_night, od_raw, transmittance, optical_density,  temperature,
                LIGHT_INTENSITY, fo, f_300us, fj, fi, fm, variable_fluorescence, quantum_yield, vj, fm_qa, mo,
                performance_index, fj_fo, fi_fo, fi_fj, fm_fi, fo_od, fj_od, fi_od, fm_od, variable_fluorescence_od,
                time_ojip, value_ojip, norm_ojip])
                print("Written to local csv")

            except:
                print("Failed to write to local csv file...")
        f.close()


class Readings:
    def send_message():
        global message_sent
        if not message_sent:
            try:
                client.messages.create(body='Phenobottle No.%s is approching saturation. Please change the reference voltage.' %PHENOBOTTLE_NUMBER,
                                            from_=from_number,
                                            to=to_number)
                print("Message sent")
                message_sent = True
            except:
                print("Fm at saturation, but there was an error...")
        else:
            pass

    @staticmethod
    def test_saturation():
        if fm >= (FLUORESCENCE_REFERENCE * 0.9):
            Readings.send_message()
        else:
            pass


class Experiment:
    @staticmethod
    def day_routine():
        global day_night
        day_night = "Day"
        MotorsAndLights.light_on(None)
        ##MotorsAndLights.light_on('red')
        ##MotorsAndLights.light_on('blue')
        MotorsAndLights.mixing_motor_on()
        time.sleep(30)

        MotorsAndLights.mixing_motor_off()
        MotorsAndLights.light_off()
        Sensors.measure_optical_density()
        Sensors.measure_fluorescence()

        MotorsAndLights.light_on(None)
        ##MotorsAndLights.light_on('red')
        ##MotorsAndLights.light_on('blue')
        MotorsAndLights.peristaltic_motor_on()
        Database.upload()
        Excel.upload()
        time.sleep(20)

        MotorsAndLights.peristaltic_motor_off()
        MotorsAndLights.bubbling_motor_on()
        MotorsAndLights.mixing_motor_on()
        time.sleep(30)

        MotorsAndLights.bubbling_motor_off()
        MotorsAndLights.mixing_motor_off()

    @staticmethod
    def night_routine():
        global day_night
        day_night = "Night"
        MotorsAndLights.light_off()
        MotorsAndLights.mixing_motor_on()
        time.sleep(30)
        MotorsAndLights.mixing_motor_off()

        Sensors.measure_optical_density()
        Sensors.measure_fluorescence()

        MotorsAndLights.peristaltic_motor_on()
        Database.upload()
        Excel.upload()
        time.sleep(20)

        MotorsAndLights.peristaltic_motor_off()
        MotorsAndLights.bubbling_motor_on()
        MotorsAndLights.mixing_motor_on()
        time.sleep(30)

        MotorsAndLights.bubbling_motor_off()
        MotorsAndLights.mixing_motor_off()

    @staticmethod
    def experiment_schedule(morning_time, night_time):
        global current_time
        current_time = datetime.datetime.now().time()
        if morning_time < night_time:
            return morning_time <= current_time <= night_time
        else:
            return current_time >= morning_time or current_time <= night_time

atexit.register(Database.close_database) #Closes database if python quits

# -------------------------------- Dilute Culture to Starting OD --------------------------------------- #

# Test.prelim_od()

# while True:
#     Test.calibrate_od()

# -------------------------------- Waits for Experiment Start Time ------------------------------------- #

while experiment_datetime > NOW:
    NOW = datetime.datetime.now()
    print("Current time is: ", NOW)
    print("Experiment starting at: ", EXPERIMENT_START_TIME)
    time.sleep(1)

# ---------------------------------------- Starts Experiment ------------------------------------------- #
#For RGB Lights
MotorsAndLights.light_on('blue')
MotorsAndLights.light_on('red')

#For Normal Lights
MotorsAndLights.light_on(None)

##LIGHT_INTENSITY = 0
if __name__ == "__main__":
    while True:
        if datetime.datetime.now().minute % 10 == 0:
            if Experiment.experiment_schedule(datetime.time(8, 00), datetime.time(20, 00)):
                NOW = datetime.datetime.now()
                # --- Slow Rise of Light Intensity --- #
                if Experiment.experiment_schedule(datetime.time(8, 00), datetime.time(10, 00)) and LIGHT_INTENSITY < 220: #Change for RGB
                    LIGHT_INTENSITY += 20 #Change for RGB
                    print(LIGHT_INTENSITY)
                # --- Slow Fall of Light Intensity --- #
                if Experiment.experiment_schedule(datetime.time(18, 00), datetime.time(20, 00)):
                    LIGHT_INTENSITY -= 20 #Change for RGB
                    print(LIGHT_INTENSITY)
                # --- Constant Daytime Light Intensity --- #
                else:
                    pass
                time.sleep(2)
                Experiment.day_routine()
 
            else:
                NOW = datetime.datetime.now()
                Experiment.night_routine()
                LIGHT_INTENSITY = 0
        time.sleep(1)
