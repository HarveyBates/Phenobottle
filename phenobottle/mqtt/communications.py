import paho.mqtt.client as mqtt
from phenobottle.config import get_configuration
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
            print("-----")
        elif rc == 1:
            print("Connection to MQTT-Broker was refused - Incorrect protocol version.")
            print("-----")
        elif rc == 2:
            print("Connection to MQTT-Broker was refused - Invalid client identifier.")
            print("-----")
        elif rc == 3:
            print("Connection to MQTT-Broker was refused - Server was unavailable.")
            print("-----")
        elif rc == 4:
            print("Connection to MQTT-Broker was refused - Bad username and/or password.")
            print("-----")
        else:
            print("Connection to MQTT-Broker was refused - Not authorised.")
            print("-----")

    def on_message(client, userdata, msg): 
        print("Message recieved:")
        print("Topic: " + msg.topic + " Payload: " + str(msg.payload, 'utf-8')) 
        print("-----")

    def on_disconnect(client, userdata, rc):
        if rc != 0:
            print("Client unexpectedly disconnected from MQTT-Broker")
            print("-----")
        else:
            print("Client disconnected from MQTT-Boker safely.")
            print("-----")

    config = get_configuration("../config.yaml")

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_disconnect = on_disconnect

    client.username_pw_set(os.getenv("BROKER_UN"), os.getenv("BROKER_PSWD"))
    client.tls_set(ca_certs=config["broker"]["certs"])
    
    client.connect(config["broker"]["address"], config["broker"]["port"], keepalive) 
    print("Attempting to connect to MQTT broker at: \nHostname: {} \nPort: {}".format(config["broker"]["address"], config["broker"]["port"]))
    print("-----")

    client.loop_start()
    return client
        

def subscribe(client, topic):
    """
    Append config to topic and subscribe. Check for errors.
    Topic should be formatted as e.g. "motor/mixing"
    Cant use # or + in topic name. TODO: Add this validation.
    """

    config = get_configuration("../config.yaml")
    # Subscribe to a topic in relation to the user and their treatment 
    topic = config["user"]["id"] + "/" + config["phenobottle"]["treatment"] + "/" + topic 
    sub = client.subscribe(topic)
    # Check error code to ensure topic has been subscribed successfully
    if(sub[0] == 0):
        print("Subscribed to topic: {}".format(topic))
        print("-----")
    else:
        print("Failed to subscribe to topic: {} ErrorCode: {}".format(topic, sub[0]))
        print("-----")

    
def publish(client, topic, payload, single=True):
    """
    Open config and append to topic.
    Check if using a single topic or and array (single=False)
    TODO: Add check for use of # or +
    Check for returned errors.
    """
    config = get_configuration("../config.yaml")
    topic = config["user"]["id"] + "/" + config["phenobottle"]["treatment"] + "/" + topic 
    pub = client.publish(topic, payload)
    if(pub[0] == 0):
        print("Pubished \"{}\" to topic: {} sucessfully.".format(payload, topic))
        print("-----")
    else:
        print("Failed to publish to topic: {} ErrorCode: {}".format(topic, pub[0]))
        print("-----")

if __name__ == "__main__":
    client = new_client()
    subscribe(client, "test")
    publish(client, "test", "this is a test")
    
    while True:
        pass


