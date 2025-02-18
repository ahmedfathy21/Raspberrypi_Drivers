import tkinter as tk
import RPi.GPIO as GPIO

# GPIO setup
LED_PIN = 18
GPIO.setmode(GPIO.BCM)  # Use BCM pin numbering
GPIO.setup(LED_PIN, GPIO.OUT)
GPIO.output(LED_PIN, GPIO.LOW)  # Initialize LED to OFF

# Function to turn LED ON
def turn_on():
    GPIO.output(LED_PIN, GPIO.HIGH)
    status_label.config(text="LED is ON", fg="green")

# Function to turn LED OFF
def turn_off():
    GPIO.output(LED_PIN, GPIO.LOW)
    status_label.config(text="LED is OFF", fg="red")

# Function to clean up GPIO on exit
def on_closing():
    GPIO.cleanup()
    window.destroy()

# Create the main window
window = tk.Tk()
window.title("LED Control")
window.geometry("300x200")

# Create and place the ON button
on_button = tk.Button(window, text="Turn ON", command=turn_on, bg="green", fg="white", font=("Arial", 14))
on_button.pack(pady=20)

# Create and place the OFF button
off_button = tk.Button(window, text="Turn OFF", command=turn_off, bg="red", fg="white", font=("Arial", 14))
off_button.pack(pady=20)

# Create and place the status label
status_label = tk.Label(window, text="LED is OFF", fg="red", font=("Arial", 16))
status_label.pack(pady=20)

# Handle window close event
window.protocol("WM_DELETE_WINDOW", on_closing)

# Start the Tkinter event loop
window.mainloop()