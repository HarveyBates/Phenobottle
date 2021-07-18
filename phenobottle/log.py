import threading
from phenobottle import microcontroller
import time


def start_log():
    time.sleep(2) # Slight startup delay
    while True:
        time.sleep(0.1) # Small delay to prevent 99% cpu
        with open("log/log.txt", "r") as file:
            lines = file.readlines()
        with open("log/log.txt", "w") as output:
            for line in lines:
                microcontroller.send_command(line.rstrip().encode("utf-8"), response=True)

readLog = threading.Thread(target=start_log)
readLog.start()


def write_to_log(input):
    with open("log/log.txt", "a") as file:
        if isinstance(input, list):
            for item in input:
                file.write(item + "\n")
        else:
            file.write(input + "\n")
    file.close()


def read_log():
    with open("log/log.txt", "r+") as file:
        log = file.readlines()
    file.close()
    if not log:
        return False
    else:
        return [line.rstrip() for line in log]


def clear_log():
    open("log/log.txt", "w").close()


if __name__ == "__main__":
    write_to_log("Testing1")
    print(read_log())
    clear_log()

