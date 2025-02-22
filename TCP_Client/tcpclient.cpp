#include <boost/asio.hpp>       // sudo apt install libboost-all-dev
#include <iostream>
                            
// in your terminal run: nc -lvp 5555  "To listen for port 5555"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::cin;
using std::endl;

#define PORT_NUMBER         5555
#define RASPBERRY_PI_IP     "10.42.0.120"  // Your Raspberry IP Address

int main() {
    try {
        boost::asio::io_service io_service;

        // Create a socket
        tcp::socket socket(io_service);

        // Connect to Raspberry Pi (Change IP to your Pi's actual address)
        string raspberry_ip = RASPBERRY_PI_IP;  // Replace with your Pi's actual IP
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(raspberry_ip), PORT_NUMBER));

        cout << "Connected to Raspberry Pi at " << raspberry_ip << endl;

        while (true) {
            string msg;
            cout << "Enter message to send: ";
            std::getline(cin, msg);

            if (msg == "exit") {
                cout << "Exiting...\n";
                break;
            }

            // Send message to Raspberry Pi
            boost::system::error_code error;
            boost::asio::write(socket, boost::asio::buffer(msg + "\n"), error);

            if (error) {
                cout << "Send failed: " << error.message() << endl;
                break;
            }

            // Receive response from Raspberry Pi
            boost::asio::streambuf receive_buffer;
            boost::asio::read_until(socket, receive_buffer, "\n", error);

            if (error && error != boost::asio::error::eof) {
                cout << "Receive failed: " << error.message() << endl;
                break;
            }

            string response(boost::asio::buffers_begin(receive_buffer.data()), boost::asio::buffers_end(receive_buffer.data()));
            cout << "Received: " << response << endl;
        }

    } catch (std::exception &e) {
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}