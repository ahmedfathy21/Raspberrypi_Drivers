#include <boost/asio.hpp>       // sudo apt install libboost-all-dev
#include <iostream>
#include <sstream>
#include <string>

// in your browser write: <raspberry_ip_address>:<port_number>
//                        http://10.42.0.120:5555

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;
using std::string;

#define PORT_NUMBER 5555

// Global variable to hold button state: 1 for ON, 0 for OFF.
int button_pressed = 0;  // initial value (off)

string read_(tcp::socket &socket)
{
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, "\r\n");
    string data = boost::asio::buffer_cast<const char *>(buf.data());
    return data;
}

void send_(tcp::socket &socket, const string &message)
{
    boost::asio::write(socket, boost::asio::buffer(message));
}

int main()
{
    try
    {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), PORT_NUMBER));

        cout << "Server running on port " << PORT_NUMBER << "...\n";

        while (true)
        {
            tcp::socket socket_(io_service);
            acceptor_.accept(socket_);

            // Read the first line of the HTTP request (e.g., "GET / HTTP/1.1")
            string request_line = read_(socket_);
            cout << "Client Request: " << request_line << endl;

            // Parse the request line to get the path.
            std::istringstream request_stream(request_line);
            string method, path, protocol;
            request_stream >> method >> path >> protocol;

            // If the request is for the command (/send?message=on or /send?message=off)
            if (path.find("/send?message=") == 0)
            {
                // Extract the message value.
                string msg_value = path.substr(string("/send?message=").length());
                if (msg_value == "on")
                {
                    button_pressed = 1;
                }
                else if (msg_value == "off")
                {
                    button_pressed = 0;
                }
                // Prepare a plain text response indicating the value of button_pressed.
                string response_body = "button_pressed=" + std::to_string(button_pressed);
                string response = "HTTP/1.1 200 OK\r\n"
                                  "Content-Type: text/plain\r\n"
                                  "Content-Length: " + std::to_string(response_body.size()) + "\r\n"
                                  "Connection: close\r\n\r\n" +
                                  response_body;
                send_(socket_, response);
            }
            else
            {
                // Otherwise, serve the HTML page with the ON/OFF buttons.
                string html_response = R"HTML(HTTP/1.1 200 OK
                    Content-Type: text/html
                    Connection: close

                    <!DOCTYPE html>
                    <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <meta name="viewport" content="width=device-width, initial-scale=1.0">
                        <title>Raspberry Pi Control</title>
                        <style>
                            body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f4; margin: 0; padding: 0; }
                            .container { width: 50%; margin: auto; background: white; padding: 20px; margin-top: 50px; box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1); border-radius: 8px; }
                            h1 { color: #333; }
                            p { font-size: 18px; color: #555; }
                            button { padding: 10px 20px; font-size: 18px; color: white; border: none; border-radius: 5px; cursor: pointer; margin: 10px; }
                            .on { background-color: #28a745; }
                            .off { background-color: #dc3545; }
                            button:hover { opacity: 0.8; }
                        </style>
                    </head>
                    <body>
                        <div class="container">
                            <h1>Raspberry Pi</h1>
                            <p>Control LED</p>
                            <button class="on" onclick="sendMessage('on')">ON</button>
                            <button class="off" onclick="sendMessage('off')">OFF</button>
                            <p id="response"></p>
                        </div>
                        <script>
                            function sendMessage(value) {
                                fetch("/send?message=" + value)
                                .then(response => response.text())
                                .then(data => {
                                    document.getElementById("response").innerText = "Server Response: " + data;
                                })
                                .catch(error => console.error("Error:", error));
                            }
                        </script>
                    </body>
                    </html>
                    )HTML";
                send_(socket_, html_response);
            }
            cout << "Response sent. Current button_pressed = " << button_pressed << endl;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
    return 0;
}