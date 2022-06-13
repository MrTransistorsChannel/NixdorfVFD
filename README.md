# NixdorfVFD
An Arduino library for interfacing to Nixdorf BA63 VFD customer displays.

Currently has support for English and Russian code pages with native conversion from utf-8 string.

# Text circuit
1)  Add J3 jumper to the display to disable parity check, remove all other jumpers from the settings panel
2)  Connect RS232 RX pin directly to pin 2 on your Arduino board.
3)  Flash your Arduino with the example code

You should then see the display show "Hello world!" both in English and Russian.

# Using in your own projects
This displays have an RS232 to TTL converter chip at the input, but testing showed that they can be used without connecting directly to the screen controller. Instead you should use software UART to be able to invert it's logic (write ` SoftwareSerial port_name(pinRX, pinTX, true);`). If for some reason you can't get it to work through the RS232 chip, you can desolder it and bridge it's input and output channel that is used for RX signal (is different for different manufacturers, you have to use a continuity tester and chip's datasheet to find it) and then use it with hardware/software UART without inverting the logic.
