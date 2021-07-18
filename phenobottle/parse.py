import json
from phenobottle import microcontroller, log 


def parse_message(msg):
    msg = json.loads(msg)
    if msg["metadata"]["type"] == "request":
        parse_request(msg)

def parse_request(msg):
    if msg["metadata"]["device"] == "temperature":
        temperature(msg)
    elif msg["metadata"]["device"] == "lights":
        light(msg)


def temperature(msg):
    microcontroller.send_command(command=msg["content"]["message"].encode("utf-8"),
            response=msg["metadata"]["response"])


def light(msg):
    if msg["content"]["switch"] == "off":
        microcontroller.send_command(command="O".encode("utf-8"))
        return
    else:
        red = "LR&I:{}&F:{}".format(msg["content"]["intensity"]["red"], msg["content"]["frequency"]["red"]) 
        green = "LG&I:{}&F:{}".format(msg["content"]["intensity"]["green"],
                msg["content"]["frequency"]["green"]) 
        blue = "LB&I:{}&F:{}".format(msg["content"]["intensity"]["blue"], 
                msg["content"]["frequency"]["blue"]) 
        log.write_to_log([red, green, blue])

