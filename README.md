# ESP32 Auto-Ranging Ohmmeter

This project implements an **auto-ranging ohmmeter using only an ESP32, resistors, and diodes**, without external ADC, op-amp, or multiplexer ICs.

The ESP32 automatically switches between multiple resistor dividers to keep the measured voltage within an optimal ADC range, allowing resistance measurements from approximately:

500 Ω to 1 MΩ


---

## How it works

The ESP32 creates multiple voltage divider bridges using fixed resistors:

820 Ω  
3.3 kΩ  
12 kΩ  
47 kΩ  
200 kΩ  
666 kΩ  

Only one bridge is active at a time.

The unknown resistor forms a voltage divider:

Vout = Vref × Rx / (Rfixed + Rx)

The ESP32 reads the divider voltage using its internal ADC and calculates:

Rx = Rfixed × Vout / (Vref − Vout)


---

## Auto-ranging logic

The firmware keeps the measured voltage between:

0.90 V and 2.30 V

If voltage is too low → switch to smaller reference resistor  
If voltage is too high → switch to larger reference resistor  

This maximizes ADC resolution.


---

## ADC noise reduction

Multiple samples are averaged:

70 samples during range adjustment  
100 samples for final measurement  

This reduces ESP32 ADC noise significantly.


---

## Hardware requirements

ESP32  
6 resistors  
Schottky or germanium diodes  
no op-amp  
no external ADC  
no multiplexer IC  


This project intentionally avoids external ICs to demonstrate a minimal implementation.


---

## Known limitations

### ESP32 GPIO voltage is not precise

ESP32 HIGH output is:

~3.0 V to 3.3 V  
depends on load current  

Schottky/germanium diodes introduce ~0.2–0.3 V drop.

Actual divider reference voltage is typically:

~3.0 V


This affects absolute accuracy.


---

### ESP32 ADC accuracy is limited

ESP32 ADC is:

non-linear  
noisy  
low effective resolution  

Error can reach:

±5% to ±10%

Averaging improves stability but not absolute accuracy.


---

### GPIO current capability is limited

ESP32 GPIO cannot provide stable reference voltage under load.


---

## Measurement range achieved

Approximate usable range:

500 Ω to 1 MΩ

Accuracy improves in mid-ranges.


---

## Why no multiplexer IC was used

This project intentionally uses only ESP32 GPIO pins and discrete components.

Using a proper analog multiplexer such as:

CD4051  
74HC4051  

or MOSFET switching would dramatically improve performance:

stable voltage source  
lower impedance  
better accuracy  
more ranges possible  


This project demonstrates the simplest possible implementation.


---

## Future improvements

External ADC (ADS1115)

16-bit resolution  
much lower noise  

MOSFET switching instead of direct GPIO

stable reference voltage  

Op-amp buffer

prevents ADC loading  

Programmable gain amplifier

true precision autoranging ohmmeter  


Digital potentiometer for automatic gain control


---

## Example serial output

Bridge: 2 | Voltage: 1.532 | Resistance: 14832.25


---

## File

esp32_auto_ohmmeter.ino


---

## Author

First hardware measurement project using ESP32.
