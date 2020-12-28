#pragma once

/*
 * Names of chars to their respective representation
  'R' = Red
  'G' = Green
  'B' = Blue
  'M' = Mixing
  'A' = Air (Bubbling)
  'F' = Fan(s)
  'I' = Peristaltic Input (Media)
  'O' = Peristaltic Output (Algae)
  'C' = Peristaltic Cuvette (Fluorometer & Optical Density)
  'T' = Temperature probe
*/

// Intialisers for PCA9865
int IN1 = 0;
int IN2 = 0;
int PWM = 0;

// Defaults
int RFreq = 120;
int GFreq = 120;
int BFreq = 120;
int MFreq = 1600;
