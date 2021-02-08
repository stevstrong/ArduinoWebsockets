/*
  Teensy41 Websockets Server (using NativeEthernet)

  This sketch:
  1. Connects to a ethernet network
  2. Starts a websocket server on port 80
  3. Waits for connections
  4. Once a client connects, it wait for a message from the client
  5. Echoes the message to the client
  6. Closes the connection and goes back to step 3

  Note:
  Make sure you share your computer's internet connection with the Teensy
  via ethernet.

  Libraries:
  To use this sketch install
  * TeensyID library (https://github.com/sstaub/TeensyID)
  * NativeEthernet (https://github.com/vjmuzik/NativeEthernet)

  Hardware:
  For this sketch you need a Teensy 4.1 board and the Teensy 4.1 Ethernet Kit
  (https://www.pjrc.com/store/ethernet_kit.html).
*/

#include <Ethernet.h>
#include <ArduinoWebsockets.h>

using namespace websockets;
WebsocketsServer server;

// We will set the MAC address at the beginning of `setup()`
byte mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC};

// Enter websockets server port.
const uint16_t port = 80;

void setup() {
  // Set the MAC address.
  //teensyMAC(mac);

  // Start Serial and wait until it is ready.
  Serial.begin(9600);
  while (!Serial) {}

  // Connect to ethernet.
  if (Ethernet.begin(mac)) {
    Serial.println("Ethernet connected");
  } else {
    Serial.println("Ethernet failed");
  }

  // Start websockets server.
  server.listen(port);
  if (server.available()) {
    Serial.print("Server available at ws://");
    Serial.print(Ethernet.localIP());
    // Also log port.
    Serial.print(":"); Serial.println(port);
  } else {
    Serial.println("Server not available!");
  }
}

void loop() {
  WebsocketsClient client = server.accept();
  
  if (client.available()) {
    Serial.println("Client connected");

    // Read message from client and log it.
    WebsocketsMessage msg = client.readBlocking();
    Serial.print("Got Message: ");
    Serial.println(msg.data());

    // Echo the message.
    client.send("Echo: " + msg.data());

    // Close the connection.
    client.close();
    Serial.println("Client closed");
  }
}