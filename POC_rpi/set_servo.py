import sys
import warnings
warnings.simplefilter('ignore')
from gpiozero import Servo
servo = Servo(18)
while 1 :
    servo_cmd = input()
    print(servo_cmd)
    servo.value = float(servo_cmd)
