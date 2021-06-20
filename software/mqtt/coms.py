import paho.mqtt.client as mqtt
from yaml import Loader
import yaml
import os
from dotenv import load_dotenv
load_dotenv()


def get_configuration(path="../config.yaml"):
    """
    Open yaml configuration file.

    @returns yaml config file
    """
    with open(path, 'r') as conf:
        config = yaml.load(conf, Loader=Loader)
    return config


def new_client(clean_session=False, keepalive=60):
    """
    Create new MQTT(s) client with appropriate credentials.

    @returns client 
    """
    connected = False

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
        print(msg.topic + " " + str(msg.payload)) 

    def on_disconnect(client, userdata, rc):
        if rc != 0:
            print("Client unexpectedly disconnected from MQTT-Broker")
        else:
            print("Client disconnected from MQTT-Boker safely.")

    config = get_configuration()

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_disconnect = on_disconnect

    client.username_pw_set(os.getenv("BROKER_UN"), os.getenv("BROKER_PSWD"))
    client.tls_set(ca_certs=config["broker"]["certs"])
    
    client.connect(config["broker"]["address"], config["broker"]["port"], keepalive) 
    print("Attempting to connect to MQTT broker at: \nHostname: {} \nPort: {}".format(config["broker"]["address"], config["broker"]["port"]))
    client.loop_start()
    return client
        

if __name__ == "__main__":
    new_client()

    


