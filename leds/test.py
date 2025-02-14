import RPi.GPIO as gpio
import time
gpio.setmode(gpio.BCM)  # 15
gpio.setup(2, gpio.OUT)
while(True):
    gpio.output(2, 1)
    time.sleep(1)
    gpio.output(2, 0)
    time.sleep(1)
