# Signal Analysis: Aliasing, Filtering, and Windowing

This project explores the fundamental challenges of digital signal processing (DSP), specifically focusing on the relationship between sampling frequency and signal reconstruction accuracy. The work is divided into two primary experiments involving hardware-in-the-loop (HIL) testing using Arduino and MATLAB.

## Experiment 1: Aliasing & The Nyquist Theorem
The objective was to observe the aliasing phenomenon by generating a **23 Hz** sinusoidal signal and sampling it at various frequencies ($f_s$): 12, 20, 26, 46, 92, and 120 Hz.


### Key Findings:
* **Nyquist Criterion:** As predicted, aliasing was observed for all sampling frequencies below 46 Hz (2x the signal frequency).
* **Reconstruction Accuracy:** While $f_s = 46$ Hz satisfied the Nyquist rate to prevent frequency aliasing, higher sampling rates (92 Hz and 120 Hz) were required to accurately capture the signal's amplitude.
* **Filter Design:** An RC low-pass filter was implemented with a cutoff frequency ($f_c$) of 100 Hz to mitigate high-frequency noise.

## Experiment 2: Multi-Frequency Signals & Hanning Windowing
[cite_start]This experiment analyzed a composite signal containing two frequencies: **4 Hz** and **61 Hz**.

### System Design:
* **Low-Pass Filtering:** A filter with $f_c \approx 7.23$ Hz was designed to isolate the 4 Hz component and attenuate the 61 Hz "noise".
* **Signal Conditioning:** The FFT analysis was performed both with and without a **Hanning window** to evaluate its effect on spectral leakage.
* **Attenuation Analysis:** Theoretical vs. experimental attenuation (dB) was calculated to validate the RC filter performance.


## Hardware & Software Used
* **Microcontroller:** Arduino Ethernet Shield (Rev3).
* **Circuitry:** RC Low-Pass Filters (1µF/1591Ω and 100µF/220Ω configurations).
* **Analysis:** MATLAB for FFT (Fast Fourier Transform), `lsim` simulations, and Hanning window implementation.

