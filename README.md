# What Is This?
Hola. This is a UD capstone design repo for a robot spider. AKA Project Venom.

# Directories
- spider_server: this is a python web server that acts as the user's controller. It is hosted on c9.io.
- spider_raspberry_pi: this is the python code for a WiFi-connected Raspberry Pi that continuously polls spider_server for the latest user commands while simultaneously transmitting these commands over a serial line to the Arduino.
- spider_arduino: this is the Arduino code that continually listens for new commands from the serial input line from the Raspberry Pi and then translates those commands into servo signals that move the robot spider's legs.
- spider_body: this is STL files for 3D-printing the spider

# Summary
spider_server -> spider_raspberry_pi -> spider_arduino -> leg servos

            (WiFi)              (Serial USB)  (PWM signals over wires)
