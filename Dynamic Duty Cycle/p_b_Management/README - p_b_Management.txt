p_b_Management by Jacob Sharkey
Code uploaded to the Arduino Microcontroller that uses the LiFuelGauge and Wire Arduino libraries as well as I2C communication to communicate with an I2C Mux and furthermore 3 different Fuel Gauges to read the state of charge (SOC) of 3 lithium polymer batteries.
Turns on the 1st MOSFET, delays for a variable boot up time and then prints the SOCs serially to a connected Raspberry Pi; delays again for another variable time and then turns off 1st MOSFET.
Turns on the 2nd MOSFET delays for a variable boot up time and then for another variable time before turning off the 2nd MOSFET.
SOCs are then read a second time and a time off variable is then set based on the total mAh hours left in the combined batteries (more mAh meaning a shorter time off).
