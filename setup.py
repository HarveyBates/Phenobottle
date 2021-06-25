from setuptools import setup, find_packages

with open('requirements.txt') as file:
    required = file.read().splitlines()

setup(
        name="Phenobottle",
        version="0.2",
        install_requires=required,
        packages=find_packages(),
        description="Phenobottle - An open-source photobioreactor for environmental simulation",
        author="Harvey Bates",
)
