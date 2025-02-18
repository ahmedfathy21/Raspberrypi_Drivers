import RPi.GPIO as GPIO

# GPIO setup
LED_PIN = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)
GPIO.output(LED_PIN, GPIO.LOW)

def turn_on():
    GPIO.output(LED_PIN, GPIO.HIGH)
    print("LED is ON")

def turn_off():
    GPIO.output(LED_PIN, GPIO.LOW)
    print("LED is OFF")

try:
    while True:
        command = input("Enter 'on' to turn LED ON, 'off' to turn LED OFF, or 'exit' to quit: ").strip().lower()
        if command == "on":
            turn_on()
        elif command == "off":
            turn_off()
        elif command == "exit":
            break
        else:
            print("Invalid command. Please try again.")
finally:
    GPIO.cleanup()
    print("GPIO cleaned up. Exiting.")