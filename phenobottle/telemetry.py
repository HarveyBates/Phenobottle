import paho.mqtt.client as mqtt
from phenobottle import log, microcontroller, config, parse
import os


def new_client(clean_session=False, keepalive=60):
    """
    Create new MQTT(s) client with appropriate credentials.
    @returns client 
    """

    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            connected = True
            print("Connected to MQTT-Broker.")
        elif rc == 1:
            print("Connection to MQTT-Broker was refused - Incorrect protocol version.")
        elif rc == 2:
            print("Connection to MQTT-Broker was refused - Invalid client identifier.")
        elif rc == 3:
            print("Connection to MQTT-Broker was refused - Server was unavailable.")
        elif rc == 4:
            print("Connection to MQTT-Broker was refused - Bad username and/or password.")
        else:
            print("Connection to MQTT-Broker was refused - Not authorised.")

    def on_message(client, userdata, msg): 
        parse.parse_message(msg.payload)

    def on_disconnect(client, userdata, rc):
        if rc != 0:
            print("Client unexpectedly disconnected from MQTT-Broker")
        else:
            print("Client disconnected from MQTT-Boker safely.")

    settings = config.get_configuration()

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_disconnect = on_disconnect

    client.username_pw_set(os.getenv("BROKER_UN"), os.getenv("BROKER_PSWD"))
    client.tls_set(ca_certs=settings["broker"]["certs"])
    
    client.connect(settings["broker"]["address"], settings["broker"]["port"], keepalive) 
    print("Attempting to connect to MQTT broker at: \nHostname: {} \nPort: {}".format(settings["broker"]["address"], settings["broker"]["port"]))

    client.loop_start()
    return client
        

def subscribe(client, topic):
    """
    Append config to topic and subscribe. Check for errors.
    Topic should be formatted as e.g. "motor/mixing"
    Cant use # or + in topic name. TODO: Add this validation.
    """

    settings = config.get_configuration()
    # Subscribe to a topic in relation to the user and their treatment 
    topic = settings["user"]["id"] + "/" + settings["phenobottle"]["treatment"] + "/" + topic 
    sub = client.subscribe(topic)
    # Check error code to ensure topic has been subscribed successfully
    if(sub[0] == 0):
        print("Subscribed to topic: {}".format(topic))
    else:
        print("Failed to subscribe to topic: {} ErrorCode: {}".format(topic, sub[0]))

    
def publish(client, topic, payload):
    """
    Open config and append to topic.
    Check if using a single topic or and array (single=False)
    TODO: Add check for use of # or +
    Check for returned errors.
    """
    settings = config.get_configuration()
    topic = settings["user"]["id"] + "/" + settings["phenobottle"]["treatment"] + "/" + topic 
    pub = client.publish(topic, payload)
    if(pub[0] == 0):
        print("Pubished \"{}\" to topic: {} sucessfully.".format(payload, topic))
    else:
        print("Failed to publish to topic: {} ErrorCode: {}".format(topic, pub[0]))


if __name__ == "__main__":
    client = new_client()
    subscribe(client, "test")
    #publish(client, "test", '{ "metadata": { "time": 1626508388, "type": "request", "device": "lights", "response": false }, "content": { "switch": "off" } }')
    publish(client, "test",  '{ "metadata": { "time": 1626508388, "type": "request", "device": "lights", "response": false }, "content": { "switch": "on", "frequency": { "red": 20000, "green": 20000, "blue": 20000 }, "intensity": { "red": 50, "green": 50, "blue": 10 } } }')
    
    while True:
        pass


