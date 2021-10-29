# Design of wearable motion sensors for a wireless audio headset (gyroscope)
The project has been developed as an individual capstone project in PolyU EIE department under supervision of Dr. Lun Pak-Kong, Daniel.  
Due to the pandemic, the device prototype was built in home conditions using the most basic eletronic components, hence can be developed with small effort.
## Description
As the everyday reliance on portable mobile devices is increasing, new diseases and health issues are arising. One of the key emerging physiological problems is neck and back pain due to wrong head posture while using smartphones. The design of a wearable headset with the sensor is presented. The headset is meant to help prevent Text Neck syndrome by monitoring a person's head position and alarming in wrongness with vibration. 
## Electornic design 
Below is the list of instructions on building this project prototype. 
<p> 
  <img src = "https://github.com/aidinatt8/FYP_Gyroscope_with_headset/blob/main/Images/device_mount.jpg" width=200 />
  <img src = "https://github.com/aidinatt8/FYP_Gyroscope_with_headset/blob/main/Images/device_mount2.jpg" width=200 />
  </p>
### Necessary components: 
- Arduino Nano (not original, works on CH340G) 
- Gyroscope GY-521 based on MPU6050 
- Micro vibration motor of size 10x3mm
- Transistor NPN BC337
- Resistor 47ohm
The circuit is built on a breadboard using Arduino Nano and MPU6050 gyroscope. The programming has been completed in Arduino IDE using few libraries. The way how data is collected using gyroscope DMP is explained in this report with testing done using Serial Plotter in Arduino IDE. 
