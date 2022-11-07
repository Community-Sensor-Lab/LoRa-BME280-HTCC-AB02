# LoRa-BME280-HTCC-AB02 

Heltec HTCC-AB02 LoRaWAN with two BME280 for Rooftop Material Analysis with Architecture 

Welcome. 

Make it more precise. Good reference if you are going to redo the project in a couple years. 
## Content: 

The BME 280 is sensors capable of measuring **TEMPERATURE**, **HUMIDITY**, and **BAROMETRIC PRESSURE**. 

Connected to the **HTCC-AB02**, built by Heltec Automation, part of their CubeCell Series. They are specifically built to be used with the Low-Power Wide-Area Network (LoRaWan). 

Data measured from the BME280 sensors are sent to The Things Stack using LoRaWan. The *Payload Formatters* in The Things Stacks is edited to reformat the information sent into readable and useful data. 
Inclusion of a webhook in The Things Network to connect and send information to a spreadsheet. 
Creation of the LoRaWan Spreadsheet. The spreadsheet was programmed using AppsScript code.gs. Addition of the Google Spreadsheet Identification (GSS_ID). Addition of table that shows the change of the **temperature**, **pressure**, **humidity**, and **battery voltage** throughout the time. It updates itself with the new information in real time. 

Creation of the bretboard, schematic, and PCB board using Fritzing software. 
## Hardware Parts: 
| Part | Part Image | Use | 
|----------------|-------------------------------|----------|
|BME 280 Sensor |--------------------------------|Sensor that measures the **Temperature (T)**, **Humidity (hPa)**, and **Pressure (P)**| 
|HTCC-AB02|--------------------------------------|Board that connects sensor, radio, and battery| 
|Battery|----------------------------------------|One of the ways that we will use to power the board and sensor| 
|Solar Panel|------------------------------------|Another way to give board and sensors power| 
|Antennae|---------------------------------------|Helps send the information gathered to The Things Network| 

![BME280Images ](https://user-images.githubusercontent.com/19189152/200361392-ebb52e23-9a29-423e-adf1-9858b4552812.jpeg)
**Above**: Image of the BME280 Sensor. 
![IMG_5879 (1)](https://user-images.githubusercontent.com/19189152/200358666-001eae0a-c64b-4593-bc46-24464a87c1a0.jpeg)
**Above**: Image of the HTCC-AB02, with the BME280, battery, and antennae connected. 


## Diagram: 
This is the diagram of the HTCC-AB02 connected to the BME280 sensor. Diagram was built using Fritzing software: 
![BME280Fritzing](https://user-images.githubusercontent.com/19189152/200355749-81b0526c-638a-4737-a1f7-40e12526b8f6.jpg) 

[HTCC-AB02_PinoutDiagram.pdf](https://github.com/Community-Sensor-Lab/LoRa-BME280-HTCC-AB02/files/9953972/HTCC-AB02_PinoutDiagram.pdf)
