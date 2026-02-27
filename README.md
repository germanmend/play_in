# 🏆 LoL Stats IoT Display (ESP32 + e-Paper)

A dedicated hardware dashboard for League of Legends players. This project allows you and your friends to securely link your Riot accounts and display your real-time Rank, LP, and Level on a low-power e-Paper screen.



## ✨ Key Features
- **Riot Sign-On (RSO):** Secure OAuth 2.0 integration. We never see your password.
- **Privacy First:** No email collection. No account linking to personal Google/Gmail accounts.
- **Hardware:** ESP32 + GDEY0154D67 1.54" e-Paper Display.
- **Multi-User:** A web-based login allows multiple friends to "register" their profiles to the device.

## 🛠️ Installation

### Backend (Python Gateway)
The backend acts as a secure bridge between Riot's API and the ESP32.
1. Install dependencies: `pip install flask requests`
2. Run the server: `python app.py`
3. Expose via ngrok: `ngrok http --url=your-domain.ngrok-free.app 5000`

### Firmware (ESP32)
1. Open `lol_display.ino` in Arduino IDE.
2. Install `GxEPD2`, `Adafruit_GFX`, and `ArduinoJson` via Library Manager.
3. Update `ssid` and `serverUrl` in the code.
4. Upload to your ESP32.

## 🔌 Connection Diagram
- **VCC** -> 3.3V
- **GND** -> GND
- **BUSY** -> GPIO 4
- **RST** -> GPIO 0
- **DC** -> GPIO 1
- **CS** -> GPIO 3
- **CLK** -> GPIO 6 (SCK)
- **DIN** -> GPIO 7 (MOSI)

## 📡 API Usage
This app utilizes the following Riot Games API endpoints:
- `summoner-v4`: To get level and account ID.
- `league-v4`: To get ranked tier, division, and league points.
- `RSO (Auth)`: For secure user authentication.
