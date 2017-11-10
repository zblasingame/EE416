# EE416
Collection of code used for EE416

## Line Scan
Code is found in the files `line_scan/metric.h` & `line_scan/metric.` All algorithms returns the difference between the measured centre and ideal centre (64). There are currently three algorithms implemented to find the centre of the black line given the data array:

* Default Approach:
Uses the notation of the centre of mass of a signal to find the centre. The function prototype is
```c
int16_t metric(uint16_t* data);
```
where `data` is the line scan data array. Returns a value in [-64, 63].

* FIR Low Pass Filter:
Uses a FIR low pass filter to preprocess data before using the notation of the centre of mass of a signal to find the centre. The function prototype is
```c
int16_t metric_lpf(uint16_t* data);
```
where `data` is the line scan data array. Returns a value in [-64, 63].

* Machine Learning Approach:
Uses the linear least sqaures algorithm to learn a mapping from array data to centre. The function prototype is
```c
int16_t metric_ml(uint16_t* data);
```
where `data` is the line scan data array. Returns a value in [-64, 63].

## Pixy Camera
Code for the pixy parser is found in the files `pixy/pixy_parser.c` & `pixy/pixy_parser.h`. There are two version of the pixy parser available: one for byte streams and another for 16-bit word streams.

* Byte Stream:
The function prototype is
```c
int8_t parse_bytes(enum command* command, uint8_t* significance, double* dist, uint8_t* bytes, uint16_t size);
```
where `command` is the location of where the command (FORWARD, LEFT, RIGHT, BACKWARD) found from the markers will be stored; `significance` is the location of the significance determined via the size of the markers will be stored; `dist` is the location where the distance between the camera and the other vehilce will be stored; `bytes` is the byte array; and `size` is the size of the array.

* Word Stream:
The function prototype is
```c
int8_t parse_words(enum command* command, uint8_t* significance, double* dist, uint8_t* words, uint16_t size);
```
where `command` is the location of where the command (FORWARD, LEFT, RIGHT, BACKWARD) found from the markers will be stored; `significance` is the location of the significance determined via the size of the markers will be stored; `dist` is the location where the distance between the camera and the other vehilce will be stored; `words` is the 16-bit word array; and `size` is the size of the array.
