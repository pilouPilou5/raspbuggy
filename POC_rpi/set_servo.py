import sys
from time import sleep
import warnings
warnings.simplefilter('ignore')
from gpiozero import Servo
import os
servo = Servo(18)
pipe = open('/tmp/pipe', 'r')
while True:
    input_str = pipe.readline()[:-1]
    if len(input_str) > 0 :
        servo.value = float(input_str)
