#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Nader";
const char* password = "Nader2233";

WebServer server(80);

// HTML content
const char* htmlContent = R"rawliteral(
<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>ESP32</title>
    <style>
        body {
            background-color: #1e1e1e;
            font-family: "JetBrains Mono", monospace;
            color: white;
            margin: 0;
            padding: 20px;
        }
        .container {
            display: flex;
            align-items: center;
            justify-content: center;
            margin-bottom: 20px;
        }
        .title {
            color: cornflowerblue;
            margin-right: 20px;
        }
        .Sensors {
            display: flex;
            flex-direction: column;
            align-items: center;
            margin-top: 20px;
        }
        .sensor-item {
            font-size: medium;
            border: 2px solid;
            border-radius: 8px;
            padding: 15px;
            margin: 10px 0;
            width: 300px;
            text-align: center;
            transition: transform 0.2s, box-shadow 0.2s;
        }
        .sensor-item:hover {
            transform: scale(1.05);
            box-shadow: 0 4px 12px rgba(0, 0, 0, 0.3);
        }
        .flame-sensor {
            color: #ed6464;
            background: rgba(237, 100, 100, 0.2);
            border-color: #ed6464;
        }
        .gas-sensor {
            color: #f0e68c;
            background: rgba(240, 230, 140, 0.2);
            border-color: #f0e68c;
        }
        .earthquake-sensor {
            color: #ffcc00;
            background: rgba(255, 204, 0, 0.2);
            border-color: #ffcc00;
        }
        .door-sensor {
            color: #66b3ff;
            background: rgba(102, 179, 255, 0.2);
            border-color: #66b3ff;
        }
        .input-sensor {
            background: rgba(255, 255, 255, 0.1);
            border: 2px solid #66b3ff;
            color: #fff;
            padding: 10px;
            width: 320px;
            text-align: center;
        }
        .input-sensor::placeholder {
            color: #b0c4de;
        }
        #Device {
            font-size: medium;
            border: 3px solid transparent;
            border-radius: 50px;
            background: linear-gradient(aliceblue, aliceblue) padding-box,
                        linear-gradient(to right, cornflowerblue, deepskyblue) border-box;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
            padding: 10px 20px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1 class="title">Smart Home Monitor</h1>
        <h1 class="title" id="Device">Device: ESP32</h1>
    </div>
    <hr>
    <div class="Sensors">
        <h1 class="sensor-item flame-sensor">Flame Sensor Signal = FlameRead 🔥</h1>
        <h1 class="sensor-item gas-sensor">Gas Sensor Signal = Warning Message</h1>
        <h1 class="sensor-item earthquake-sensor">Earthquake = MpuMessage</h1>
        <h1 class="sensor-item door-sensor">Guest On The Door = UltraSonic</h1>
        <input class="sensor-item input-sensor" type="text" placeholder="Type a Message to the Guest">
    </div>
</body>
</html>
)rawliteral";

void setup() {
    Serial.begin(115200);
    
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());

    // Serve the HTML page
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", htmlContent);
    });

    // Start the server
    server.begin();
}

void loop() {
    server.handleClient(); // Handle incoming clients
}
