import sys
from gpiozero import Servo
servo = Servo(18)
print(sys.argv(1))
servo.value = sys.argv[1]