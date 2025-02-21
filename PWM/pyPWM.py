from time import sleep
import RPi.GPIO as GPIO

GPIO.setwarnings(False)

PWM_PIN = 12

GPIO.setmode(GPIO.BCM)
GPIO.setup(PWM_PIN, GPIO.OUT)

pwm = GPIO.PWM(PWM_PIN, 1000)        # Frequency -> 1000Hz
pwm.start(0)

while True:
    for duty in range(0,101,1):
        pwm.ChangeDutyCycle(duty)
        sleep(0.01)
    for duty in range(100,-1,-1):
        pwm.ChangeDutyCycle(duty)
        sleep(0.01)