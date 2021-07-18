from yaml import Loader
import yaml
from dotenv import load_dotenv
load_dotenv()


def get_configuration(path="config/config.yaml"):
    """
    Open yaml configuration file.

    @returns yaml config file
    """
    with open(path, 'r') as conf:
        config = yaml.load(conf, Loader=Loader)
    return config

