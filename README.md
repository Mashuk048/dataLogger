# ESP8266 IoT Environmental Monitoring

## Overview

This project utilizes an ESP8266 microcontroller to monitor and report environmental data. It connects to a specified WiFi network and sends HTTP POST requests to a server, which can be used to log or display sensor data. The system retrieves data every 20 seconds and stores it in a CSV format.

## Features

- Connects to a WiFi network.
- Sends HTTP requests to a specified server to log environmental data.
- Supports reading and sending data as CSV files.
- Utilizes ArduinoJson for JSON parsing.

## Hardware Requirements

- ESP8266 module (e.g., NodeMCU, Wemos D1 Mini)
- Access to a WiFi network
- A server that can accept incoming HTTP requests (e.g., a local server or cloud service)
- Jumper wires (if using a breadboard setup)

## Software Requirements

- Arduino IDE with ESP8266 board support
- Required libraries:
  - `ESP8266WiFi`
  - `ESP8266HTTPClient`
  - `ArduinoJson`
  - `FS` (for SPIFFS file system)
  - `Ticker`

## Setup Instructions

1. **Install Arduino IDE**: If you haven't already, download and install the Arduino IDE.

2. **Install ESP8266 Board Support**:
   - Open the Arduino IDE.
   - Go to `File` -> `Preferences`.
   - Add the following URL to the `Additional Board Manager URLs`:
     ```
     http://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
   - Go to `Tools` -> `Board` -> `Board Manager`, search for `ESP8266`, and install the package.

3. **Install Required Libraries**:
   - Use the Library Manager (`Sketch` -> `Include Library` -> `Manage Libraries`) to install the `ArduinoJson` library.

4. **Upload the Code**:
   - Connect your ESP8266 to your computer.
   - Select the appropriate board and port in the Arduino IDE.
   - Copy the provided code into a new Arduino sketch.
   - Modify the `ssid` and `password` variables to match your WiFi credentials.
   - Update the `server` and `serverName` variables with the appropriate IP address and port for your server.
   - Upload the code to the ESP8266.

## Code Explanation

### Key Functions

- `setup()`: Initializes the serial communication, connects to WiFi, and mounts the SPIFFS filesystem. It also sets up a timer to trigger data reading.

- `loop()`: Checks if it's time to send a new HTTP request. It retrieves data from the server and logs it. If the WiFi is disconnected, it reports the status.

- `sendData(String data)`: Sends a string of data to the server via an HTTP POST request.

- `sendCSV()`: Sends the payload as a CSV format to the server.

- `sendFile()`: Sends a log file to the server as a multipart/form-data POST request.

### Debugging

The code includes a debug mode that can be toggled with the `DEBUG` macro. If `DEBUG` is set to `1`, the program will print debugging information to the Serial Monitor.

### Data Flow

1. The ESP8266 connects to the specified WiFi network.
2. Every 20 seconds, it sends an HTTP GET request to retrieve environmental data from the server.
3. The data is processed, and specific values (like timestamp and power) are logged.
4. The ESP8266 sends the logged data to the server in CSV format.

## Usage

1. After uploading the code, open the Serial Monitor in the Arduino IDE.
2. Observe the connection status and any debug output.
3. Ensure that the server is running and can receive the data being sent.
4. Check the server logs or database to verify that the data is being received correctly.

## Troubleshooting

- Ensure that the ESP8266 is properly connected to your WiFi.
- Check the server IP address and port; they must be correct for data to be sent successfully.
- Look for any error codes printed to the Serial Monitor, which can indicate issues with HTTP requests.

## Contributing

Contributions are welcome! If you have suggestions for improvements or features, feel free to submit an issue or pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
