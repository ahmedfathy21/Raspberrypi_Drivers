import smbus2
import RPi.GPIO as GPIO
import time

# Suppress GPIO warnings
GPIO.setwarnings(False)

# LED Setup
LED_PIN = 17  # GPIO17
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)
GPIO.output(LED_PIN, GPIO.LOW)

# I2C Setup
I2C_ADDRESS = 0x08  # Match ESP8266's address
bus = smbus2.SMBus(1)  # Use I2C bus 1 (GPIO 2/3)

try:
    while True:
        try:
            # Read 3 bytes from the ESP8266
            command_bytes = bus.read_i2c_block_data(I2C_ADDRESS, 0, 3)
            command = bytes(command_bytes).decode("utf-8").strip()

            # Validate and control the LED
            if command in ["ON", "OFF"]:
                if command == "ON":
                    GPIO.output(LED_PIN, GPIO.HIGH)
                else:
                    GPIO.output(LED_PIN, GPIO.LOW)
                print(f"Received: {command}")
            else:
                print(f"Invalid command: {command}")

        except Exception as e:
            print(f"I2C Error: {e}")

        time.sleep(0.5)  # Poll every 500ms

except KeyboardInterrupt:
    GPIO.cleanup()