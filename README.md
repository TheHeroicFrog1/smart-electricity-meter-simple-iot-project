# ⚡ IoT Smart Electricity Meter (ESP8266 + PZEM-004T)

> **I made this project in my 2nd year and found the files for it today. As it is my 1st real engineering project I want to archive it. Also, if anyone wants to make something similar and needs help, I have added steps (I used AI to format it because I didn't want to write a lot).**

It measures real-time AC voltage, current, power, and energy consumption, and pushes it all to a live Blynk dashboard on your phone.

## 📐 System Architecture
Here is the exact wiring setup to make this work safely.

![Circuit Diagram](/circuit-diagram.svg)

## 🛠️ Hardware You Need
* **ESP8266 (NodeMCU)** - The brain and Wi-Fi chip.
* **PZEM-004T (V3.0)** - The AC energy monitoring module.
* **Split-Core Current Transformer (CT Coil)** - Usually comes in the box with the PZEM.
* **An Extension Board / Socket** - To safely plug in your appliances.
* **Jumper Wires & a 5V Micro-USB Cable.**

---

## ⚠️ Problems I had to figure out

When you look up tutorials for this online, the diagrams are often super confusing or flat-out wrong. Here is how it actually works in the real world:

### 1. The CT Coil Trick
The CT (Current Transformer) is that little plastic clamp. **DO NOT pass a whole appliance cable through this ring.** If you pass both the Live and Neutral wires through it at the same time, their magnetic fields cancel each other out, and your current reading will be exactly zero.
* **The Fix:** You need to strip the outer rubber of the main wire feeding *into* your extension board and clamp the CT around **ONLY the Live wire** (usually red or brown). 

### 2. The Power Supply Illusion
The PZEM-004T connects to the scary 230V AC mains voltage, but it **does not** power your microcontroller. 
* Your ESP8266 needs its own safe 5V power source. Just plug a standard Micro-USB cable into your laptop or a phone charger. 
* The ESP8266 will then share that 5V logic power to wake up the PZEM sensor via the `Vin` and `GND` pins.

### 3. RX/TX Cross-over
Serial communication wires always cross over. 
* ESP `D5` (RX) connects to PZEM `TX`
* ESP `D6` (TX) connects to PZEM `RX`

---

## 💻 Software Setup
1. Get the code from releases.
2. Install the required libraries via the Library Manager: `Blynk` and `PZEM004Tv30`.
3. Swap out the Wi-Fi credentials (`ssid` and `pass`) for your own network.
4. Set up a free project on the [Blynk IoT Cloud](https://blynk.cloud/), grab your `BLYNK_AUTH_TOKEN`, and paste it into the code.
5. Flash the ESP8266!

## 📊 The Dashboard Setup
In your Blynk app, set up these Virtual Pins to catch the telemetry data:
* `V1` = Voltage (V)
* `V2` = Current (A)
* `V3` = Power (W)
* `V4` = Energy (Wh)
* `V5` = Frequency (Hz)
* `V6` = Power Factor
* `V7` = Estimated Bill (₹) *(Note: I hardcoded the rate to ₹6.62/kWh, but you can change the `UNIT_RATE` variable in the code).*