# EMS_with_SmokeTempGSM
This project is a Smoke and Temperature Monitoring System using Arduino.
# Arduino Smoke and Temperature Monitoring System

This Arduino sketch is designed to monitor environmental conditions using a DHT sensor (temperature and humidity), an MQ2 smoke sensor, and an LCD display. Additionally, it integrates a GSM module (SIM800L) to send SMS alerts when predefined thresholds for smoke or temperature are exceeded.
This program is useful for **real-time environmental monitoring and safety applications**. It can be used in homes, offices, or industries to:

1. **Detect Smoke or Gas Leaks**: Alerts when smoke levels exceed a safe threshold, helping to prevent fire hazards.
2. **Monitor Temperature and Humidity**: Tracks environmental conditions and alerts if temperatures become too high, reducing the risk of overheating or heat-related incidents.
3. **Send SMS Alerts**: Notifies users remotely via SMS, ensuring quick action in emergencies even when they are away.
4. **Local Display**: Provides live data on an LCD for on-site monitoring.

This system is ideal for improving safety, automation, and environmental awareness in various settings.

---

## Hardware Requirements

1. **Arduino Board** (e.g., Uno)
2. **DHT Sensor** (e.g., DHT11)
3. **MQ2 Smoke Sensor**
4. **SIM800L GSM Module**
5. **16x2 LCD Display**
6. **Connection Wires and Breadboard**
### Dependencies

- `dht.h`: For DHT sensor readings.
- `LiquidCrystal.h`: For LCD handling.
- `SoftwareSerial.h`: For GSM communication.


---

## Code Features

### 1. **Setup**

- Initializes the Serial communication at 9600 baud for debugging.
- Configures the GSM module for SMS functionality.
- Initializes the LCD for displaying system status and sensor readings.
```
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  // Initialize GSM module
  Serial.println("Initializing GSM...");
  mySerial.println("AT"); // Test GSM connection
  mySerial.println("AT+CMGF=1"); // Set GSM to text mode
  mySerial.println("AT+CMGS=\"+916382954917\""); // Set SMS recipient
  
  // Display initialization message
  lcd.begin(16, 2);
  lcd.print("Initializing");
  lcd.setCursor(0, 1);
  lcd.print("    :)    ");
  
  // Print initial messages
  Serial.println("....Temperature and Humidity Sensor....");
  Serial.println("....Smoke Sensor is Ready...");
}

```

---
### 2. **Main Functionality**

#### Sensor Monitoring and LCD Display

- **DHT Sensor**: Reads temperature and humidity values.
- **MQ2 Sensor**: Reads smoke levels.
- Updates the LCD with real-time humidity and temperature readings.

#### GSM Commands

Command    	Action
status	            Sends current temperature and humidity via SMS.

- Listens for SMS commands like "Status" to provide updates on environmental conditions.
- Sends alerts via SMS when:
    - Smoke levels exceed a threshold.
    - Temperature surpasses 30°C.
```
    void loop() {
  // Read sensors
  Smokelvl = analogRead(mq2_pin);
  DHT.read11(dht_apin);
  
  // Display readings on LCD
  lcd.setCursor(0, 0);
  lcd.print("Humidity:");
  lcd.print(DHT.humidity);
  lcd.setCursor(0, 1);
  lcd.print("Temperature:");
  lcd.print(DHT.temperature);

  // Check thresholds and send alerts
  if (Smokelvl > 300) smklvl(); // Smoke alert
  else if (DHT.temperature > 30) templvl(); // Temperature alert
}

```

---
### 3. **Alert Functions**

#### Smoke Alert (`smklvl()`)

- Detects high smoke levels.
- Displays a warning on the LCD and sends an SMS alert.

#### Temperature Alert (`templvl()`)

- Detects high temperature.
- Displays a warning on the LCD and sends an SMS alert.
```
void smklvl() {
  lcd.print("Smoke Detected");
  lcd.print("Open Windows");
  mySerial.print("Smoke Level is Above The Limit."); // Send SMS
}

void templvl() {
  lcd.print("Temperature");
  lcd.print("Exceeded");
  mySerial.print("Temperature level is Too High"); // Send SMS
}

```

---
### 4. **Command Handling**

The code listens for commands sent via SMS or Serial Monitor. It processes specific commands like:

- `"status"`: Triggers the `Right()` function to send current sensor readings via SMS.
- Invalid commands: Triggers the `intrupW()` function to display an error message.
```
void Right() {
  lcd.print("Request Received");
  lcd.print("Sending SMS");
  String Output = "Humidity: " + String(DHT.humidity) + "%\n" +
                  "Temperature: " + String(DHT.temperature) + "°C";
  mySerial.print(Output); // Send SMS with readings
}

```

---
### 5. **Serial Communication (`updateSerial()`)**

Synchronizes data between the Serial Monitor and the GSM module.
```
void updateSerial() {
  while (Serial.available()) {
    mySerial.write(Serial.read());
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

```

---
## Functionality Overview

1. Initializes sensors, LCD, and GSM module.
2. Continuously monitors:
    - Temperature and humidity via DHT sensor.
    - Smoke levels via MQ2 sensor.
3. Updates the LCD with live readings.
4. Sends SMS alerts when thresholds are exceeded.
5. Processes SMS or Serial commands to provide real-time status updates.
---
