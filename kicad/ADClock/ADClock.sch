EESchema Schematic File Version 4
LIBS:ADClock-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:Crystal Y1
U 1 1 5CD78B8B
P 1500 1600
F 0 "Y1" V 1546 1469 50  0000 R CNN
F 1 "Crystal" V 1455 1469 50  0000 R CNN
F 2 "Crystal:Crystal_HC49-4H_Vertical" H 1500 1600 50  0001 C CNN
F 3 "~" H 1500 1600 50  0001 C CNN
	1    1500 1600
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C1
U 1 1 5CD7B1CB
P 1800 1150
F 0 "C1" H 1915 1196 50  0000 L CNN
F 1 "C" H 1915 1105 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 1838 1000 50  0001 C CNN
F 3 "~" H 1800 1150 50  0001 C CNN
	1    1800 1150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5CD7D2AA
P 2000 2100
F 0 "C2" H 2115 2146 50  0000 L CNN
F 1 "C" H 2115 2055 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 2038 1950 50  0001 C CNN
F 3 "~" H 2000 2100 50  0001 C CNN
	1    2000 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1550 1800 1450
Wire Wire Line
	1800 1450 1500 1450
Wire Wire Line
	1500 1750 2000 1750
Wire Wire Line
	2000 1950 2000 1750
Connection ~ 2000 1750
Wire Wire Line
	2000 1750 2450 1750
Wire Wire Line
	1800 1450 1800 1300
Connection ~ 1800 1450
Wire Wire Line
	1800 1000 950  1000
Wire Wire Line
	950  1000 950  3650
Wire Wire Line
	1600 3450 1600 3900
$Comp
L Connector:Screw_Terminal_01x03 VCC(5v)1
U 1 1 5CE03BE8
P 1800 4500
F 0 "VCC(5v)1" H 1880 4542 50  0000 L CNN
F 1 "Screw_VCC" H 1880 4451 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-3_P5.08mm" H 1800 4500 50  0001 C CNN
F 3 "~" H 1800 4500 50  0001 C CNN
	1    1800 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 4500 1600 4600
Wire Wire Line
	2450 4500 2450 4600
Wire Wire Line
	1600 4500 1600 4400
Connection ~ 1600 4500
Connection ~ 1600 4400
Wire Wire Line
	4950 1000 4950 1250
Wire Wire Line
	5600 1250 4950 1250
Wire Wire Line
	2300 1000 4950 1000
Wire Wire Line
	5000 950  5000 1350
$Comp
L Connector:Conn_01x06_Male Stepper1
U 1 1 5CD8A699
P 5400 1450
F 0 "Stepper1" H 5508 1831 50  0000 C CNN
F 1 "Stepper_1" H 5508 1740 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 5400 1450 50  0001 C CNN
F 3 "~" H 5400 1450 50  0001 C CNN
	1    5400 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2050 5250 2050
Wire Wire Line
	5300 2150 4450 2150
$Comp
L Connector:Conn_01x06_Male Stepper2
U 1 1 5CD8C93F
P 9350 2750
F 0 "Stepper2" H 9458 3131 50  0000 C CNN
F 1 "Stepper_2" H 9458 3040 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 9350 2750 50  0001 C CNN
F 3 "~" H 9350 2750 50  0001 C CNN
	1    9350 2750
	1    0    0    -1  
$EndComp
Connection ~ 2450 4500
Wire Wire Line
	5000 1350 5600 1350
Wire Wire Line
	2200 950  5000 950 
$Comp
L Connector:Conn_01x03_Female HallSensor1
U 1 1 5CD99EB0
P 5650 3050
F 0 "HallSensor1" H 5678 3076 50  0000 L CNN
F 1 "Hall_Sensor_1" H 5678 2985 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 5650 3050 50  0001 C CNN
F 3 "~" H 5650 3050 50  0001 C CNN
	1    5650 3050
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Female HallSensor2
U 1 1 5CD9C0E9
P 5650 3450
F 0 "HallSensor2" H 5678 3476 50  0000 L CNN
F 1 "Hall_Sensor_1" H 5678 3385 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 5650 3450 50  0001 C CNN
F 3 "~" H 5650 3450 50  0001 C CNN
	1    5650 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 2950 5150 2950
Wire Wire Line
	5150 2950 5150 3350
Wire Wire Line
	5150 3350 5450 3350
Wire Wire Line
	5450 3050 5200 3050
Wire Wire Line
	5200 3050 5200 3450
Wire Wire Line
	5200 3450 5450 3450
Wire Wire Line
	5250 2050 5250 3150
Wire Wire Line
	5250 3150 5450 3150
Wire Wire Line
	5450 3550 5300 3550
Wire Wire Line
	5300 3550 5300 2150
Wire Wire Line
	5200 3450 5200 4050
Connection ~ 5200 3450
Wire Wire Line
	5150 3350 5150 4000
Wire Wire Line
	5150 4000 2300 4000
Connection ~ 5150 3350
Wire Wire Line
	5200 4050 2200 4050
Wire Wire Line
	2300 3450 2300 1000
Connection ~ 2450 4400
Wire Wire Line
	2450 4500 2450 4400
$Comp
L Connector:Screw_Terminal_01x03 GND1
U 1 1 5CE04652
P 2650 4500
F 0 "GND1" H 2730 4542 50  0000 L CNN
F 1 "Screw_GND" H 2730 4451 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-3_P5.08mm" H 2650 4500 50  0001 C CNN
F 3 "~" H 2650 4500 50  0001 C CNN
	1    2650 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 4150 2450 4400
Wire Wire Line
	2200 3100 2200 950 
Connection ~ 2200 3100
Wire Wire Line
	2150 3100 2150 3050
Connection ~ 2150 3100
Wire Wire Line
	2150 3100 2200 3100
Connection ~ 2150 3250
Wire Wire Line
	2150 3400 2150 3250
Wire Wire Line
	2150 3250 2150 3100
$Comp
L Device:C C3
U 1 1 5CDD9B23
P 2300 3050
F 0 "C3" V 2048 3050 50  0000 C CNN
F 1 "C" V 2139 3050 50  0000 C CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 2338 2900 50  0001 C CNN
F 3 "~" H 2300 3050 50  0001 C CNN
	1    2300 3050
	0    1    1    0   
$EndComp
Wire Wire Line
	2450 3400 2450 3450
Connection ~ 2450 3400
$Comp
L Device:C C4
U 1 1 5CDD1844
P 2300 3250
F 0 "C4" V 2048 3250 50  0000 C CNN
F 1 "C" V 2139 3250 50  0000 C CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 2338 3100 50  0001 C CNN
F 3 "~" H 2300 3250 50  0001 C CNN
	1    2300 3250
	0    1    1    0   
$EndComp
$Comp
L Device:C C5
U 1 1 5CDCD79B
P 2300 3400
F 0 "C5" V 2552 3400 50  0000 C CNN
F 1 "C" V 2461 3400 50  0000 C CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 2338 3250 50  0001 C CNN
F 3 "~" H 2300 3400 50  0001 C CNN
	1    2300 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2450 3250 2450 3400
Wire Wire Line
	2000 4150 2000 3800
Wire Wire Line
	2450 4150 2000 4150
Connection ~ 2300 3450
Wire Wire Line
	2300 4000 2300 3450
Wire Wire Line
	2200 4050 2200 3650
Wire Wire Line
	2200 3650 2450 3650
Connection ~ 2200 3650
Wire Wire Line
	2200 3650 2200 3100
Wire Wire Line
	2450 3450 2300 3450
Wire Wire Line
	2000 3650 2200 3650
Connection ~ 2000 3650
Wire Wire Line
	950  3650 2000 3650
Connection ~ 2450 3450
Connection ~ 2450 3250
Wire Wire Line
	1600 3450 2300 3450
Wire Wire Line
	2450 1550 1800 1550
Wire Wire Line
	2000 2250 2000 3650
$Comp
L ATMEGA328-PU:ATMEGA328-PU IC1
U 1 1 5CD82419
P 3450 2450
F 0 "IC1" H 3450 3817 50  0000 C CNN
F 1 "ATMEGA328-PU" H 3450 3726 50  0000 C CNN
F 2 "DIL28" H 3450 2450 50  0001 L BNN
F 3 "" H 3450 2450 50  0001 L BNN
F 4 "None" H 3450 2450 50  0001 L BNN "Field4"
F 5 "ATmega Series 20 MHz 32 KB Flash 2 KB SRAM 8-Bit Microcontroller - DIP-28" H 3450 2450 50  0001 L BNN "Field5"
F 6 "Unavailable" H 3450 2450 50  0001 L BNN "Field6"
F 7 "ATMEGA328-PU" H 3450 2450 50  0001 L BNN "Field7"
F 8 "PDIP-28 Microchip" H 3450 2450 50  0001 L BNN "Field8"
	1    3450 2450
	1    0    0    -1  
$EndComp
$Comp
L ATMEGA328-PU:ATMEGA328-PU IC2
U 1 1 5CDD2DBB
P 7900 2600
F 0 "IC2" H 7900 3967 50  0000 C CNN
F 1 "ATMEGA328-PU" H 7900 3876 50  0000 C CNN
F 2 "DIL28" H 7900 2600 50  0001 L BNN
F 3 "" H 7900 2600 50  0001 L BNN
F 4 "None" H 7900 2600 50  0001 L BNN "Field4"
F 5 "ATmega Series 20 MHz 32 KB Flash 2 KB SRAM 8-Bit Microcontroller - DIP-28" H 7900 2600 50  0001 L BNN "Field5"
F 6 "Unavailable" H 7900 2600 50  0001 L BNN "Field6"
F 7 "ATMEGA328-PU" H 7900 2600 50  0001 L BNN "Field7"
F 8 "PDIP-28 Microchip" H 7900 2600 50  0001 L BNN "Field8"
	1    7900 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 3400 6900 3500
Wire Wire Line
	6900 3800 6800 3800
Connection ~ 2000 3800
Wire Wire Line
	2000 3800 2000 3650
Wire Wire Line
	4700 3900 1600 3900
Wire Wire Line
	4700 3500 6600 3500
Connection ~ 6900 3500
Wire Wire Line
	6900 3500 6900 3600
Connection ~ 1600 3900
Wire Wire Line
	1600 3900 1600 4400
Wire Wire Line
	9550 2550 9550 750 
Wire Wire Line
	9550 750  5600 750 
Wire Wire Line
	5600 750  5600 1250
Connection ~ 5600 1250
Wire Wire Line
	9550 2650 9600 2650
Wire Wire Line
	9600 2650 9600 800 
Wire Wire Line
	9600 800  6700 800 
Wire Wire Line
	5650 800  5650 1350
Wire Wire Line
	5650 1350 5600 1350
Connection ~ 5600 1350
$Comp
L Connector:Conn_01x06_Male ExtDigital1
U 1 1 5CDA59E4
P 6200 1450
F 0 "ExtDigital1" H 6308 1831 50  0000 C CNN
F 1 "Conn_Ext_digital" H 6308 1740 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 6200 1450 50  0001 C CNN
F 3 "~" H 6200 1450 50  0001 C CNN
	1    6200 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 1450 5600 1450
Wire Wire Line
	4450 1550 5600 1550
Wire Wire Line
	4450 1650 5600 1650
Wire Wire Line
	4450 3250 5000 3250
Wire Wire Line
	5000 3250 5000 1750
Wire Wire Line
	5000 1750 5600 1750
Wire Wire Line
	8900 1600 9300 1600
Wire Wire Line
	9300 1600 9300 2750
Wire Wire Line
	9300 2750 9550 2750
Wire Wire Line
	8900 1700 9250 1700
Wire Wire Line
	9250 1700 9250 2850
Wire Wire Line
	9250 2850 9550 2850
Wire Wire Line
	8900 1800 9200 1800
Wire Wire Line
	9200 1800 9200 2950
Wire Wire Line
	9200 2950 9550 2950
Wire Wire Line
	8900 3400 9550 3400
Wire Wire Line
	9550 3400 9550 3050
Wire Wire Line
	4450 2950 4650 2950
Wire Wire Line
	4650 2950 4650 4550
Wire Wire Line
	4650 4550 9100 4550
Wire Wire Line
	9100 4550 9100 3100
Wire Wire Line
	9100 3100 8900 3100
Wire Wire Line
	4700 3500 4700 3900
Wire Wire Line
	8900 3200 9050 3200
Wire Wire Line
	9050 3200 9050 4450
Wire Wire Line
	9050 4450 4750 4450
Wire Wire Line
	4750 4450 4750 3050
Wire Wire Line
	4750 3050 4450 3050
Wire Wire Line
	4450 3150 4850 3150
Wire Wire Line
	4850 3150 4850 4400
Wire Wire Line
	4850 4400 9000 4400
Wire Wire Line
	9000 4400 9000 3300
Wire Wire Line
	9000 3300 8900 3300
Wire Wire Line
	4450 1350 4900 1350
Wire Wire Line
	4900 1350 4900 1800
Wire Wire Line
	4900 1800 5800 1800
Wire Wire Line
	5800 1800 5800 1250
Wire Wire Line
	5800 1250 6400 1250
Wire Wire Line
	4450 1750 4850 1750
Wire Wire Line
	4850 1750 4850 1850
Wire Wire Line
	4850 1850 5900 1850
Wire Wire Line
	5900 1850 5900 1350
Wire Wire Line
	5900 1350 6400 1350
Wire Wire Line
	4450 1850 4750 1850
Wire Wire Line
	4750 1850 4750 1950
Wire Wire Line
	4750 1950 6000 1950
Wire Wire Line
	6000 1950 6000 1450
Wire Wire Line
	6000 1450 6400 1450
Wire Wire Line
	4450 3350 5050 3350
Wire Wire Line
	5050 3350 5050 2200
Wire Wire Line
	5050 2200 6050 2200
Wire Wire Line
	6050 2200 6050 1550
Wire Wire Line
	6050 1550 6400 1550
Wire Wire Line
	4450 3450 5100 3450
Wire Wire Line
	5100 3450 5100 2250
Wire Wire Line
	5100 2250 6100 2250
Wire Wire Line
	6100 2250 6100 1650
Wire Wire Line
	6100 1650 6400 1650
Wire Wire Line
	4450 3550 5250 3550
Wire Wire Line
	5250 3550 5250 3650
Wire Wire Line
	5250 3650 5950 3650
Wire Wire Line
	5950 3650 5950 1750
Wire Wire Line
	5950 1750 6400 1750
$Comp
L Device:C C10
U 1 1 5CE48E49
P 6800 3650
F 0 "C10" V 6548 3650 50  0000 C CNN
F 1 "C" V 6639 3650 50  0000 C CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 6838 3500 50  0001 C CNN
F 3 "~" H 6800 3650 50  0001 C CNN
	1    6800 3650
	-1   0    0    1   
$EndComp
Connection ~ 6800 3800
Wire Wire Line
	6800 3800 6600 3800
Connection ~ 6800 3500
Wire Wire Line
	6800 3500 6900 3500
$Comp
L Device:C C6
U 1 1 5CE4B755
P 6600 3650
F 0 "C6" V 6348 3650 50  0000 C CNN
F 1 "C" V 6439 3650 50  0000 C CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 6638 3500 50  0001 C CNN
F 3 "~" H 6600 3650 50  0001 C CNN
	1    6600 3650
	-1   0    0    1   
$EndComp
Connection ~ 6600 3800
Wire Wire Line
	6600 3800 6300 3800
Connection ~ 6600 3500
Wire Wire Line
	6600 3500 6800 3500
$Comp
L Device:C C9
U 1 1 5CE4C2DF
P 6700 3200
F 0 "C9" V 6448 3200 50  0000 C CNN
F 1 "C" V 6539 3200 50  0000 C CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 6738 3050 50  0001 C CNN
F 3 "~" H 6700 3200 50  0001 C CNN
	1    6700 3200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6900 3200 6850 3200
Wire Wire Line
	6550 3200 6300 3200
Wire Wire Line
	6300 3200 6300 3800
Connection ~ 6300 3800
Wire Wire Line
	6300 3800 2000 3800
$Comp
L Device:Crystal Y2
U 1 1 5CE574FB
P 6650 1800
F 0 "Y2" V 6696 1669 50  0000 R CNN
F 1 "Crystal" V 6605 1669 50  0000 R CNN
F 2 "Crystal:Crystal_HC49-4H_Vertical" H 6650 1800 50  0001 C CNN
F 3 "~" H 6650 1800 50  0001 C CNN
	1    6650 1800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6900 1700 6700 1700
Wire Wire Line
	6650 1700 6650 1650
Wire Wire Line
	6900 1900 6650 1900
Wire Wire Line
	6650 1900 6650 1950
$Comp
L Device:C C8
U 1 1 5CE637A5
P 6700 1400
F 0 "C8" H 6815 1446 50  0000 L CNN
F 1 "C" H 6815 1355 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 6738 1250 50  0001 C CNN
F 3 "~" H 6700 1400 50  0001 C CNN
	1    6700 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 1250 6700 1400
Wire Wire Line
	6700 1550 6700 1700
Connection ~ 6700 1700
Wire Wire Line
	6700 1700 6650 1700
Wire Wire Line
	6700 1250 6700 800 
Connection ~ 6700 1250
Connection ~ 6700 800 
Wire Wire Line
	6700 800  6500 800 
$Comp
L Device:C C7
U 1 1 5CE7581E
P 6650 2100
F 0 "C7" H 6765 2146 50  0000 L CNN
F 1 "C" H 6765 2055 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 6688 1950 50  0001 C CNN
F 3 "~" H 6650 2100 50  0001 C CNN
	1    6650 2100
	1    0    0    -1  
$EndComp
Connection ~ 6650 1950
Wire Wire Line
	6650 2250 6500 2250
Wire Wire Line
	6500 2250 6500 800 
Connection ~ 6500 800 
Wire Wire Line
	6500 800  5650 800 
$Comp
L Connector:Conn_01x06_Male ExtDigital2
U 1 1 5CEADFA0
P 9850 1700
F 0 "ExtDigital2" H 9958 2081 50  0000 C CNN
F 1 "Conn_Ext_digital_2" H 9958 1990 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 9850 1700 50  0001 C CNN
F 3 "~" H 9850 1700 50  0001 C CNN
	1    9850 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 2000 9750 2000
Wire Wire Line
	9750 2000 9750 1900
Wire Wire Line
	9750 1900 10050 1900
Wire Wire Line
	8900 1900 9700 1900
Wire Wire Line
	9700 1900 9700 1800
Wire Wire Line
	9700 1800 10050 1800
Wire Wire Line
	8900 3500 10050 3500
Wire Wire Line
	10050 3500 10050 2000
Wire Wire Line
	8900 3600 10100 3600
Wire Wire Line
	10100 3600 10100 1700
Wire Wire Line
	10100 1700 10050 1700
Wire Wire Line
	10050 1600 10150 1600
Wire Wire Line
	10150 1600 10150 3700
Wire Wire Line
	10150 3700 8900 3700
Wire Wire Line
	8900 3800 10200 3800
Wire Wire Line
	10200 3800 10200 1500
Wire Wire Line
	10200 1500 10050 1500
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 5CF3FE1E
P 1650 5000
F 0 "H1" H 1750 5049 50  0000 L CNN
F 1 "MountingHole_Pad" H 1750 4958 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 1650 5000 50  0001 C CNN
F 3 "~" H 1650 5000 50  0001 C CNN
	1    1650 5000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 5CF4761B
P 1950 5000
F 0 "H2" H 2050 5049 50  0000 L CNN
F 1 "MountingHole_Pad" H 2050 4958 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 1950 5000 50  0001 C CNN
F 3 "~" H 1950 5000 50  0001 C CNN
	1    1950 5000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 5CF47CC2
P 2300 5000
F 0 "H3" H 2400 5049 50  0000 L CNN
F 1 "MountingHole_Pad" H 2400 4958 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 2300 5000 50  0001 C CNN
F 3 "~" H 2300 5000 50  0001 C CNN
	1    2300 5000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 5CF47E5C
P 2700 5000
F 0 "H4" H 2800 5049 50  0000 L CNN
F 1 "MountingHole_Pad" H 2800 4958 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 2700 5000 50  0001 C CNN
F 3 "~" H 2700 5000 50  0001 C CNN
	1    2700 5000
	1    0    0    -1  
$EndComp
Connection ~ 1950 5100
Wire Wire Line
	2300 5100 2450 5100
Connection ~ 2300 5100
Wire Wire Line
	2450 4600 2450 5100
Connection ~ 2450 4600
Connection ~ 2450 5100
Wire Wire Line
	2450 5100 2700 5100
Wire Wire Line
	1950 5100 2300 5100
Wire Wire Line
	1650 5100 1950 5100
$EndSCHEMATC
