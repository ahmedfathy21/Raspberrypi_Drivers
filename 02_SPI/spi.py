import spidev
import serial
import time

spi_bus = 0
spi_device = 0

# Open SPI connection with mode 0
spi = spidev.SpiDev()
spi.open(spi_bus, spi_device)

spi.max_speed_hz = 50000
spi.mode = 0

# Open serial connection
ser = serial.Serial('/dev/ttyUSB0', 115000)  # Adjust the serial port as needed

try:
    while True:
        if ser.in_waiting > 0:
            command = ser.readline().decode('utf-8').strip()
            if command.lower() == 'on':
                spi.xfer2([1])  # Send signal to turn on the LED
                print("LED turned ON")
            elif command.lower() == 'off':
                spi.xfer2([0])  # Send signal to turn off the LED
                print("LED turned OFF")
finally:
    spi.close()
    ser.close()