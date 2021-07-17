import json
from phenobottle import microcontroller, log 

def parse_message(msg):
    msg = json.loads(msg)
    #print(json.dumps(msg, indent=4))
    if msg["metadata"]["type"] == "request":
        parse_request(msg)

def parse_request(msg):
    if msg["metadata"]["device"] == "temperature":
        microcontroller.send_command(command=msg["content"]["message"].encode("utf-8"),
                waitResponse=msg["metadata"]["response"])
    elif msg["metadata"]["device"] == "lights":
        if msg["content"]["switch"] == "off":
            microcontroller.send_command(command="O".encode("utf-8"))
            return
        else:
            red = "LR&I:{}&F:{}".format(msg["content"]["intensity"]["red"], msg["content"]["frequency"]["red"]) 
            green = "LG&I:{}&F:{}".format(msg["content"]["intensity"]["green"],
                    msg["content"]["frequency"]["green"]) 
            blue = "LB&I:{}&F:{}".format(msg["content"]["intensity"]["blue"], 
                    msg["content"]["frequency"]["blue"]) 
            log.write_to_log(red)
            log.write_to_log(green)
            log.write_to_log(blue)
            #microcontroller.send_command(command=red)
            #microcontroller.send_command(command=green)
            #microcontroller.send_command(command=blue)

