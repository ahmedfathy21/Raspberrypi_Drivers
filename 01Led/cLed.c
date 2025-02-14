// Compile & link & generate output file
//   -> gcc -o <output file name> <file name> -lgpiod
//   -> gcc -o cLed.out cLed.c -lgpiod
// Run
//   -> ./<output file name>
//   -> ./cLed.out

#include <gpiod.h>          
#include <stdio.h>          
#include <unistd.h>         // system <sleep, exit>
#include <signal.h>         // for catching interrupt signal of (ctrl+c)

#define LED_PIN        22        
#define CONSUMER_NAME  "CBlinkLed"      // Will be disaplyed to show, who using the line <gpioinfo>

// creating object of gpiod chip & line
struct gpiod_chip *chip;
struct gpiod_line *line;

// Cleanup when receiving (ctrl+c)
void cleanup(int sig)
{
    // Reset the GPIO pin to LOW (optional)
    gpiod_line_set_value(line, 0);
    // Release the line and close the chip
    gpiod_line_release(line);
    gpiod_chip_close(chip);

    printf("\nGPIO cleanup done. Exiting safely.\n");
    _exit(0);  // Exit the program
}

int main()
{
    int ret;

    // Set up signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, cleanup);

    // Open the GPIO chip
    chip = gpiod_chip_open_by_name("gpiochip0");
    if (!chip) {
        perror("Failed to open gpiochip0");
        return -1;
    }

    // Get the GPIO line for the LED pin -> line LED_PIN
    line = gpiod_chip_get_line(chip, LED_PIN);
    if (!line) {
        perror("Failed to get GPIO line");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Request the line as output
    ret = gpiod_line_request_output(line, CONSUMER_NAME, 0);
    if (ret < 0) {
        perror("Failed to request GPIO line as output");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Blink the LED
    while (1) {
        // Set GPIO high
        gpiod_line_set_value(line, 1);
        sleep(1);

        // Set GPIO low
        gpiod_line_set_value(line, 0);
        sleep(1);
    }

    return EXIT_SUCCESS;
}