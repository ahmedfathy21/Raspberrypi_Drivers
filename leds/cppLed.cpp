#include <gpiod.h>
#include <iostream>
#include <csignal>      // For catching interrupt signal (Ctrl+C)
#include <thread>       // For std::this_thread::sleep_for
#include <chrono>       // For std::chrono::seconds
#include <memory>       // For smart pointers

#define LED_PIN          22
#define CONSUMER_NAME    "CBlinkLed"

class LedController
{
private:
    int pin_;
    std::string consumer_name_;
    std::unique_ptr<gpiod_chip, decltype(&gpiod_chip_close)> chip_;
    std::unique_ptr<gpiod_line, decltype(&gpiod_line_release)> line_;
    bool cleaned_up_ = false;

    void getLine() {
        line_ = std::unique_ptr<gpiod_line, decltype(&gpiod_line_release)>(
            gpiod_chip_get_line(chip_.get(), pin_), gpiod_line_release);
        if (!line_) {
            throw std::runtime_error("Failed to get GPIO line");
        }
    }

    void requestLine() {
        if (gpiod_line_request_output(line_.get(), consumer_name_.c_str(), 0) < 0) {
            throw std::runtime_error("Failed to request GPIO line as output");
        }
    }

    void setLineValue(int value) {
        if (gpiod_line_set_value(line_.get(), value) < 0) {
            throw std::runtime_error("Failed to set GPIO line value");
        }
    }

public:
    LedController(int pin, const std::string& consumer_name)
        : pin_(pin), consumer_name_(consumer_name),
          chip_(gpiod_chip_open_by_name("gpiochip0"), gpiod_chip_close),
          line_(nullptr, gpiod_line_release) {
        if (!chip_) {
            throw std::runtime_error("Failed to open gpiochip0");
        }
        getLine();
        requestLine();
    }

    ~LedController() {
        if(!cleaned_up_){
            cleanup();
        }
    }

    void blink(int interval_seconds) {
        while (true) {
            setLineValue(1);
            std::this_thread::sleep_for(std::chrono::seconds(interval_seconds));
            setLineValue(0);
            std::this_thread::sleep_for(std::chrono::seconds(interval_seconds));
        }
    }

    void cleanup() {
        if (!cleaned_up_) {
            if (line_) {
                setLineValue(0);
                gpiod_line_release(line_.get());
            }
            if (chip_) {
                gpiod_chip_close(chip_.get());
            }
            std::cout << "\nGPIO cleanup done. Exiting safely." << std::endl;
            cleaned_up_ = true;
        }
    }
};

std::unique_ptr<LedController> ledController;

void signalHandler(int sig) {
    if (ledController) {
        ledController->cleanup();
    }
    std::exit(0);
}

int main()
{
    try {
        std::signal(SIGINT, signalHandler);
        ledController = std::make_unique<LedController>(LED_PIN, CONSUMER_NAME);
        ledController->blink(1);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}