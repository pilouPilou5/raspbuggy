from gpiozero import Servo
from time import sleep

# Define the GPIO pin for the servo (use the BCM pin number)
servo = Servo(18)

while True:
    # Move the servo to -1 (min position, typically 0 degrees)
    servo.min()
    sleep(1)
    
    # Move the servo to 0 (mid position, typically 90 degrees)
    servo.mid()
    sleep(1)
    
    # Move the servo to 1 (max position, typically 180 degrees)
    servo.max()
    sleep(1)