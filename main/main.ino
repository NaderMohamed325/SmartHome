#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Nader";
const char* password = "Nader2233";

WebServer server(80);

const int analogPin = 36; // D15 pin for analog reading

// HTML content
String htmlContent = R"rawliteral(
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
        .analog-sensor {
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
        .send-button {
            background-color: cornflowerblue;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 10px 15px;
            cursor: pointer;
            margin-top: 10px;
        }
        .send-button:hover {
            background-color: deepskyblue;
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
        .authors {
            margin-top: 20px;
            text-align: center;
            color: #b0c4de;
        }
        .author-names {
            font-size: larger;
            color: white;
            transition: transform 0.2s, box-shadow 0.2s;
            display: inline-block;
        }
        .author-names:hover {
            transform: scale(1.05);
            box-shadow: 0 4px 12px rgba(0, 0, 0, 0.5);
        }
    </style>
</head>
<body>
<div class="container">
    <h1 class="title">Smart Home Monitor</h1>
    <h1 class="title" id="Device">Device: ESP32</h1>
</div>
<div class="Sensors">
    <h1 class="sensor-item analog-sensor" id="analogSensorReading">Analog Sensor Signal = Waiting...</h1>
    <h1 class="sensor-item gas-sensor">Gas Sensor Signal = Warning Message</h1>
    <h1 class="sensor-item earthquake-sensor">Earthquake = MpuMessage</h1>
    <h1 class="sensor-item door-sensor">Guest On The Door = UltraSonic</h1>
    <label>
        <input id="guestMessage" class="sensor-item input-sensor" type="text" placeholder="Type a Message to the Guest">
    </label>
    <button id="sendButton" class="send-button">Send Message</button>
</div>

<div class="authors">
    <h2>Authors:</h2>
    <p class="author-names">Nader || Bassem || Mahmoud</p>
</div>

<script>
    document.getElementById('sendButton').addEventListener('click', function() {
        const message = document.getElementById('guestMessage').value;
        fetch('/sendMessage', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ message: message })
        }).then(response => {
            if (response.ok) {
                console.log('Message sent to LCD');
                document.getElementById('guestMessage').value = ''; // Clear the input
            }
        });
    });

    function updateAnalogSensorReading() {
        fetch('/analogSensor')
            .then(response => response.json())
            .then(data => {
                document.getElementById('analogSensorReading').innerText = 'Analog Sensor Signal = ' + data.value;
            });
    }

    setInterval(updateAnalogSensorReading, 1000); // Update every second
</script>
</body>
</html>
)rawliteral";

int readAnalogSensor() {
    return analogRead(analogPin);
}

void setup() {
    Serial.begin(115200);
    pinMode(36,  INPUT);
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

    // Endpoint to get the analog sensor reading
    server.on("/analogSensor", HTTP_GET, []() {
        int analogValue = readAnalogSensor();
        String json = "{\"value\": " + String(analogValue) + "}";
        server.send(200, "application/json", json);
    });

    // Start the server
    server.begin();
}

void loop() {
    server.handleClient(); // Handle incoming clients
    
    // Read and print analog value to Serial Monitor
    int analogValue = readAnalogSensor();
    Serial.print("Analog Sensor Value: ");
    Serial.println(analogValue);
    
    delay(1000); // Delay for 1 second between readings
}