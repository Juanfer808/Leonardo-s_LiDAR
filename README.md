# Leonardo-s_LiDAR
A functional LiDAR, made with a VL53L0X ToF sensor, and in-app screening of the readings through bluetooth, showing a drawing of the room
Created during 2021

This project is currently on hold as I have moved on to other projects.

# Features
- 360º Scanning
- Bluetooth App

# Hardware
- Arduino Nano
- 3D printed parts: custom gears and stand
- Bluetooth: HC-05
- VL53L0X

# Software & Dependencies
- Arduino IDE
- SoftwareSerial.h
- Adafruit_VL53L0X.h
- App made with AppInventor

# Context
In the making of a model tank based on the design of Leonardo da Vinci, the usage of VL53L0X sensors were indispensable. After trying to use some clever design where the cannon's barrels were the sensors, I found out that using a system of Master-Slave and a bubble wasn't worth it, being a bit delicate. Also the own design was limiting the range of the sensor, that when I opted to use a LiDAR system similar to that of roomba

# Notes
The last thing I managed to do was a 3D printed stand and gear that moved the sensor in 360º, making those a map of the room. The length of the sensor was pretty limited though so we deemed a better idea to buy a commercial-grade LiDAR and then program it ourselves.
