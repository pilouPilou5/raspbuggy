import sys
from time import sleep
from gpiozero import Servo
from gpiozero.pins.pigpio import PiGPIOFactory
factory = PiGPIOFactory()
servo = Servo(13)
while True:
    input_str = input()
    if len(input_str) > 0 :
        servo.value = float(input_str)
