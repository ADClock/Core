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
$Comp
L Connector:Conn_01x06_Male Stepper1
U 1 1 5CD8A699
P 6600 900
F 0 "Stepper1" H 6708 1281 50  0000 C CNN
F 1 "Stepper_1" H 6708 1190 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 6600 900 50  0001 C CNN
F 3 "~" H 6600 900 50  0001 C CNN
	1    6600 900 
	1    0    0    -1  
$EndComp
Connection ~ 2450 4500
$Comp
L Connector:Conn_01x03_Female HallSensor1
U 1 1 5CD99EB0
P 6850 3300
F 0 "HallSensor1" H 6878 3326 50  0000 L CNN
F 1 "Hall_Sensor_1" H 6878 3235 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6850 3300 50  0001 C CNN
F 3 "~" H 6850 3300 50  0001 C CNN
	1    6850 3300
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Female HallSensor2
U 1 1 5CD9C0E9
P 6850 3800
F 0 "HallSensor2" H 6878 3826 50  0000 L CNN
F 1 "Hall_Sensor_1" H 6878 3735 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6850 3800 50  0001 C CNN
F 3 "~" H 6850 3800 50  0001 C CNN
	1    6850 3800
	1    0    0    -1  
$EndComp
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
	2450 4150 2000 4150
Wire Wire Line
	2200 3650 2450 3650
Connection ~ 2200 3650
Wire Wire Line
	2200 3650 2200 3100
Wire Wire Line
	2000 3650 2200 3650
Connection ~ 2000 3650
Wire Wire Line
	950  3650 2000 3650
Wire Wire Line
	2450 1550 1800 1550
Wire Wire Line
	2000 2250 2000 3650
$Comp
L Connector:Conn_01x06_Male ExtDigital1
U 1 1 5CDA59E4
P 6600 2600
F 0 "ExtDigital1" H 6708 2981 50  0000 C CNN
F 1 "Conn_Ext_digital" H 6708 2890 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 6600 2600 50  0001 C CNN
F 3 "~" H 6600 2600 50  0001 C CNN
	1    6600 2600
	1    0    0    -1  
$EndComp
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
Wire Wire Line
	2000 3650 2000 3950
Wire Wire Line
	1600 3450 1600 3850
Connection ~ 2450 3250
Connection ~ 2450 3450
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
Wire Wire Line
	1600 3450 2050 3450
$Comp
L Device:R R1
U 1 1 5CE55E6C
P 2200 1350
F 0 "R1" V 1993 1350 50  0000 C CNN
F 1 "R" V 2084 1350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 2130 1350 50  0001 C CNN
F 3 "~" H 2200 1350 50  0001 C CNN
	1    2200 1350
	0    1    1    0   
$EndComp
Wire Wire Line
	2450 1350 2400 1350
Wire Wire Line
	2050 1350 2050 3450
Connection ~ 2050 3450
Wire Wire Line
	2050 3450 2450 3450
$Comp
L Connector:Conn_01x06_Male Stepper2
U 1 1 5CE5EA17
P 6600 1750
F 0 "Stepper2" H 6708 2131 50  0000 C CNN
F 1 "Stepper_2" H 6708 2040 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 6600 1750 50  0001 C CNN
F 3 "~" H 6600 1750 50  0001 C CNN
	1    6600 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 850  2400 850 
Wire Wire Line
	2400 850  2400 1350
Connection ~ 2400 1350
Wire Wire Line
	2400 1350 2350 1350
Wire Wire Line
	3100 850  3100 2950
Wire Wire Line
	3100 2950 4450 2950
Wire Wire Line
	4450 3050 3200 3050
Wire Wire Line
	3200 3050 3200 850 
Wire Wire Line
	4450 2950 5450 2950
Wire Wire Line
	5450 2950 5450 2400
Wire Wire Line
	5450 2400 6800 2400
Connection ~ 4450 2950
Wire Wire Line
	4450 3050 5500 3050
Wire Wire Line
	5500 3050 5500 2500
Wire Wire Line
	5500 2500 6800 2500
Connection ~ 4450 3050
Wire Wire Line
	4450 3150 5550 3150
Wire Wire Line
	5550 3150 5550 2600
Wire Wire Line
	5550 2600 6800 2600
Wire Wire Line
	6800 2700 5600 2700
Wire Wire Line
	5600 2700 5600 3250
Wire Wire Line
	5600 3250 4450 3250
Wire Wire Line
	4450 3350 5650 3350
Wire Wire Line
	5650 3350 5650 2800
Wire Wire Line
	5650 2800 6800 2800
Wire Wire Line
	4450 3450 5700 3450
Wire Wire Line
	5700 3450 5700 2900
Wire Wire Line
	5700 2900 6800 2900
Wire Wire Line
	6800 1550 6950 1550
Wire Wire Line
	6950 1550 6950 700 
Wire Wire Line
	6950 700  6800 700 
Wire Wire Line
	6800 1650 7050 1650
Wire Wire Line
	7050 1650 7050 800 
Wire Wire Line
	7050 800  6800 800 
Wire Wire Line
	6950 1550 6950 3150
Wire Wire Line
	6950 3200 6650 3200
Connection ~ 6950 1550
Wire Wire Line
	6950 3200 6950 3650
Wire Wire Line
	6950 3700 6650 3700
Connection ~ 6950 3200
Wire Wire Line
	7050 1650 7050 3300
Wire Wire Line
	7050 3300 6650 3300
Connection ~ 7050 1650
Wire Wire Line
	7050 3300 7050 3800
Wire Wire Line
	7050 3800 6650 3800
Connection ~ 7050 3300
Wire Wire Line
	6650 3400 4650 3400
Wire Wire Line
	4650 3400 4650 2050
Wire Wire Line
	4650 2050 4450 2050
Wire Wire Line
	6650 3900 6400 3900
Wire Wire Line
	6400 3900 6400 3300
Wire Wire Line
	6400 3300 4600 3300
Wire Wire Line
	4600 3300 4600 2150
Wire Wire Line
	4600 2150 4450 2150
$Comp
L Device:R R3
U 1 1 5CE9AB24
P 7850 3800
F 0 "R3" H 7920 3846 50  0000 L CNN
F 1 "R" H 7920 3755 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7780 3800 50  0001 C CNN
F 3 "~" H 7850 3800 50  0001 C CNN
	1    7850 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5CE9D05E
P 7850 3300
F 0 "R2" H 7920 3346 50  0000 L CNN
F 1 "R" H 7920 3255 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7780 3300 50  0001 C CNN
F 3 "~" H 7850 3300 50  0001 C CNN
	1    7850 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 3150 6950 3150
Connection ~ 6950 3150
Wire Wire Line
	6950 3150 6950 3200
Wire Wire Line
	7850 3650 6950 3650
Connection ~ 6950 3650
Wire Wire Line
	6950 3650 6950 3700
Wire Wire Line
	7850 3450 6650 3450
Wire Wire Line
	6650 3450 6650 3400
Connection ~ 6650 3400
Wire Wire Line
	7850 3950 6650 3950
Wire Wire Line
	6650 3950 6650 3900
Connection ~ 6650 3900
Wire Wire Line
	4450 3550 5750 3550
Wire Wire Line
	5750 3550 5750 2050
Wire Wire Line
	5750 2050 6800 2050
Wire Wire Line
	4450 3650 5800 3650
Wire Wire Line
	5800 3650 5800 1950
Wire Wire Line
	5800 1950 6800 1950
Wire Wire Line
	4450 1350 5800 1350
Wire Wire Line
	5800 1350 5800 1850
Wire Wire Line
	5800 1850 6800 1850
Wire Wire Line
	4450 1450 5850 1450
Wire Wire Line
	5850 1450 5850 1750
Wire Wire Line
	5850 1750 6800 1750
Wire Wire Line
	4450 1550 4750 1550
Wire Wire Line
	4750 1550 4750 900 
Wire Wire Line
	4750 900  6800 900 
Wire Wire Line
	6800 1000 4800 1000
Wire Wire Line
	4800 1000 4800 1650
Wire Wire Line
	4800 1650 4450 1650
Wire Wire Line
	4450 1750 4850 1750
Wire Wire Line
	4850 1750 4850 1100
Wire Wire Line
	4850 1100 6800 1100
Wire Wire Line
	4450 1850 4900 1850
Wire Wire Line
	4900 1850 4900 1200
Wire Wire Line
	4900 1200 6800 1200
Connection ~ 6800 700 
$Comp
L Connector:Conn_01x05_Female J1
U 1 1 5CF13408
P 3200 650
F 0 "J1" V 3138 362 50  0000 R CNN
F 1 "Conn_Programmer" V 3047 362 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 3200 650 50  0001 C CNN
F 3 "~" H 3200 650 50  0001 C CNN
	1    3200 650 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4400 700  4400 850 
Wire Wire Line
	4400 850  3400 850 
Wire Wire Line
	6800 800  4450 800 
Wire Wire Line
	4450 800  4450 900 
Wire Wire Line
	4450 900  3450 900 
Wire Wire Line
	3300 900  3300 850 
Connection ~ 6800 800 
Wire Wire Line
	3400 850  3400 3850
Wire Wire Line
	3400 3850 1600 3850
Connection ~ 3400 850 
Connection ~ 1600 3850
Wire Wire Line
	1600 3850 1600 4400
Wire Wire Line
	2000 3950 3450 3950
Wire Wire Line
	3450 3950 3450 900 
Connection ~ 2000 3950
Wire Wire Line
	2000 3950 2000 4150
Connection ~ 3450 900 
Wire Wire Line
	3450 900  3300 900 
Wire Wire Line
	4400 700  6800 700 
$EndSCHEMATC
