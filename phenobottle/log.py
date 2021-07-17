import threading
from phenobottle import microcontroller
import time


def start_log():
    with open("log/log.txt", "r") as file:
        while True:
            requests = file.readlines()
            for request in requests:
                microcontroller.send_command(request.rstrip().encode("utf-8"))
                # TODO get a response so we know the command was recieved and done
            clear_log() # Fix this

readLog = threading.Thread(target=start_log)
readLog.start()


def write_to_log(input):
    with open("log/log.txt", "a") as file:
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

