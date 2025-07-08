🔋 Project Overview: ATtiny85 6h ON / 18h OFF LED Timer
🧰 Hardware Requirements
Component	Description
ATtiny85	8-pin microcontroller
2x AAA Batteries	3V total power
2x LEDs	Small low-current (e.g. 2mA)
2x Resistors	100–220Ω (for LEDs)
Capacitor	0.1µF (decoupling, optional)
Programmer	USBASP, Arduino as ISP, or Tiny AVR Programmer
Optional Switch	To reset/start the timer manually

          +---\/---+
  PB5 (5) |1     8| VCC
  PB3 (3) |2     7| PB2 (2)
  PB4 (4) |3     6| PB1 (1) → LED 2
   GND    |4     5| PB0 (0) → LED 1
          +-------+

| ATtiny85 Pin | Function    | Connects To            |
| ------------ | ----------- | ---------------------- |
| Pin 4        | GND         | Battery Negative (-)   |
| Pin 8        | VCC         | Battery Positive (+3V) |
| Pin 5 (PB0)  | LED1 Output | Resistor → LED → GND   |
| Pin 6 (PB1)  | LED2 Output | Resistor → LED → GND   |
| Optional     | Pin 7 (PB2) | For ISP (SCK)          |
| Optional     | Pin 1 (PB5) | For ISP Reset          |

PB0 (Pin 5) → R1 (220Ω) → LED1 (+) → LED1 (−) → GND

PB1 (Pin 6) → R2 (220Ω) → LED2 (+) → LED2 (−) → GND

🧾 Fuse Settings (for 1 MHz + low power)
When using the Spence Konde ATtinyCore, set:

Clock: 1 MHz (internal)

BOD: Disabled

Fuse values typically:

Low: 0x62

High: 0xDF

Extended: 0xFF

Set these via "Burn Bootloader" in the Arduino IDE after selecting 1 MHz internal and BOD disabled.

⚡ Component Power Consumption per Day
🟢 LED Current (when ON)
2 LEDs × 2 mA = 4 mA

6 hours ON per 24h

4 mA × 6 h = 24 mAh per day

🔵 ATtiny85 Current
Active Time (waking):

Let's say total CPU awake time is 30 seconds per 24 hours.

Active current ~3 mA

3 mA × (30 / 3600) h = ~0.025 mAh

Sleep Time (~99.9% of day):

WDT sleep current ~6 µA

24 hours × 6 µA = ~0.144 mAh

Total ATtiny draw = ~0.17 mAh per day

🔋 Total System Consumption
sql
Copy
Edit
LEDs:      ~24.0 mAh/day  
ATtiny85:  ~0.17 mAh/day  
-------------------------
TOTAL:     ~24.17 mAh/day
📅 Estimated Battery Life
With 1000 mAh available from the two AAA batteries:

yaml
Copy
Edit
1000 mAh / 24.17 mAh ≈ ~41.4 days
✅ Expected Runtime: ~40–45 days

If using PWM (Pulse Width Modulation):

🧠 How PWM Works on ATtiny85
The ATtiny85 supports hardware PWM on the following pins:

Pin	Arduino ID	Hardware PWM
5	PB0 (0)	✅ Timer 0
6	PB1 (1)	✅ Timer 0
7	PB2 (2)	✅ Timer 1


Current Features List:

6Hr On/18Hr Off - Configurable
Sleep Mode / Watchdog system
PWM for LEDs
Fade In/Fade Out effect

