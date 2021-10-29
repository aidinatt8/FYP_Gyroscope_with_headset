# Design of wearable motion sensors for a wireless audio headset (gyroscope)
The project has been developed as an individual capstone project in PolyU EIE department under supervision of Dr. Lun Pak-Kong, Daniel.  
Due to the pandemic, the device prototype was built in home conditions using the most basic eletronic components, hence can be developed with small effort.

## Description
As the everyday reliance on portable mobile devices is increasing, new diseases and health issues are arising. One of the key emerging physiological problems is neck and back pain due to wrong head posture while using smartphones. The design of a wearable headset with the sensor is presented. The headset is meant to help prevent Text Neck syndrome by monitoring a person's head position and alarming in wrongness with vibration. 
<p> 
  <img src = "https://github.com/aidinatt8/FYP_Gyroscope_with_headset/blob/main/Images/Picture%202.jpg" width=150 />
  <img src = "https://github.com/aidinatt8/FYP_Gyroscope_with_headset/blob/main/Images/Picture%201.jpg" width = 300 />
  <br> Illustration of Text Neck syndrome 
  </p>
  
## Program flow 
- Tilt of head is explored by using only one axes as reference point;  
- The vibration motor is turned on when there is forward tilt on Y axes of gyroscope for more than 30 degrees; 
- The vibration motor waits for 5 seconds before turning on, working only during prolonged wrong head position;
- The posture state such as sleeping does affect the functionality. When there is forward head inclination at horizontal position, the vibration motor doesn't work because the Text Neck syndrome happens during inclination at vertical position and gravity affecting on it;
- There is a calibration button that helps to reset desired initial position for gyroscope using EEPROM memory;
<p> 
  Program flowchart
  <img src = "https://github.com/aidinatt8/FYP_Gyroscope_with_headset/blob/main/Images/program_flowchart.jpg"/>
  </p>
  
# Electornic design 
Below is the list of instructions on building this project prototype. 
<p> 
  <img src = "https://github.com/aidinatt8/FYP_Gyroscope_with_headset/blob/main/Images/device_mount.jpg" width=300 />
  <img src = "https://github.com/aidinatt8/FYP_Gyroscope_with_headset/blob/main/Images/device_mount2.jpg" width=300 />
  </p>
 
## Necessary components: 
- Arduino Nano (not original, works on CH340G) 
- Gyroscope GY-521 based on MPU6050 
- Micro vibration motor of size 10x3mm
- Transistor NPN BC337
- Resistor 47ohm
- Push button switch, breadboard, USB cable 

<p> 
  <img src = "https://github.com/aidinatt8/FYP_Gyroscope_with_headset/blob/main/Images/circuit%20design.png" />
  Circuit design diagram 
  </p>
