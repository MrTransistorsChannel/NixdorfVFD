# NixdorfVFD
An Arduino library for interfacing with Nixdorf BA63 VFD customer displays

![57bbeae1eb703](https://github.com/user-attachments/assets/cda35c11-a46f-4f38-abc6-252f53dc3629)

Currently has support for English (CP437) and Russian (CP866) code pages with built in conversion from utf-8 string

# Using these displays in your projects
Displays have both **TX** and **RX** pins for data, as well as **RTS** and **CTS** for flow control, but only **RX** is needed for sending data/commands to the display. My guess is that **TX** pin is only used for setting some system parameters or maybe even flashing a dedicated ROM chip (that can be found installed on some of the displays), but the ![manual](https://github.com/MrTransistorsChannel/NixdorfVFD/blob/master/BA63_Display_Operating_Manual_english.pdf) doesn't mention any commands that use **TX** pin to get data from the display

Despite having an RS232 to TTL converter chip at the input, these displays most of the time can be used without bypassing it. Since RS232 uses inverted logic (positive voltage is logic zero and negative - logic one), you should use software UART to be able to have inverted logic UART on Arduino boards

  ```SoftwareSerial port_name(pinRX, pinTX, true);```

If for some reason you can't get it to work through the RS232 chip, you can desolder it and bridge it's input and output pins, that are used for **RX** signal. I actually did that to all of my displays so I don't need to worry about inverting the UART logic

***WARNING: This particular customer display comes with many different board revisions, so you should check which line driver your board uses and what channel of that driver is used for the RX signal!***

To do the check simply use a continuity tester and find out which pin of the RS232 line driver is connected directly to the **RX** pin of the mini DIN socket and then, using the driver's datasheet, find which pin is the output for that channel. For example, if your **RX** pin is connected to R1_IN pin of the line driver, find the R1_OUT pin in the datasheet and solder the two pins together with a jumper wire

In my testing I had access to two versions of the display board, those being

  * ***Futaba M202MD25A***
  * ***Noritake Itron CU20029-T325A***

![20251107_000005](https://github.com/user-attachments/assets/1287652d-3c13-45ea-9e1c-bfb469184067)
![20251107_001424](https://github.com/user-attachments/assets/3a8a7029-8968-49b0-b3f3-85436ba3df99)

Both of these boards use pin-to-pin compatible RS232 driver chips (ST202EC and SP232A), which both have 2 TX and 2 RX lines used for **TX**, **RX**, **RTS** and **CTS** signals, but the channels they use for each of the signal differ between the two boards. By using a continuity tester I found out that the Futaba board uses ***RX channel 2*** (pins 8 and 9 of the driver IC) for UART RX signal and the Noritake one uses ***RX channel 1*** (pins 12 and 13). You can see those pins bridged on the photos above

The ROM chip is M27C256B UV erasable EPROM + OTP ROM chip or similar. Testing showed that the displays work totaly fine even with it removed, in fact, only the two Noritake boards that I have, had it installed in the first place. I haven't looked at its contents so you are free to experiment with the chip yourself, if you have one installed in the socket on your board, and send back the results, so we can update this guide!

# Test circuit
1)  Add **J3** jumper to the display to disable parity check, remove all other jumpers from the settings panel. Complete info on the jumpers can be found in the ![manual](https://github.com/MrTransistorsChannel/NixdorfVFD/blob/master/BA63_Display_Operating_Manual_english.pdf) on ***page 18***
2)  Connect RS232 RX pin directly to pin 3 (or whichever you use for the **SoftwareSerial TX**) on your Arduino board. You can insert wires into the mini DIN socket, solder them to the socket`s pads, or use dedicated pads found on the **Futaba** displays (can be seen on the photos to the right of the mini DIN connector pads, I labelled them **R** for **RX**, **T** for **TX**, **+** for **+12V** and **G** for **GND**). Mini DIN socket pinout can be found on ***page 20*** of the display's ![manual](https://github.com/MrTransistorsChannel/NixdorfVFD/blob/master/BA63_Display_Operating_Manual_english.pdf)
3)  Connect 12V power supply to display`s **+12V** and **GND** pins
4)  Flash your Arduino with this example code (can be found in the Examples folder for this library)

```c
#include <NixdorfVFD.h>
#include <SoftwareSerial.h>

NixdorfVFD VFD;
SoftwareSerial vfd(2, 3, false); // SoftwareSerial port on pins #2 and #3, inverted logic

void setup() {
  vfd.begin(9600);              // Starting serial communication at 9600bod
  VFD.begin(vfd);               // Initialising the display
  VFD.clear();                  // Clearing screen
  VFD.home();                   // Returning cursor to the start position
  VFD.setCodePage(CP_866);      // Enabling cyrillic code page

  VFD.setCursor(4, 0);          // Moving cursor to 5 column in the first row
  VFD.print("Hello world!");    // Printing text
  VFD.setCursor(4, 1);
  VFD.print("Привет, мир!");

}

void loop() {
  // Nothing here
}
```

![20251107_013943](https://github.com/user-attachments/assets/852a7c17-b2d7-45db-95cf-58df52898f93)

You should then see the display show "Hello world!" both in English and Russian.

# Reference (can be found in NixorfVFD.h)
```c
    /************** CODE PAGES *************/
    typedef enum {
        CP_USA           = 0,
        CP_FRANCE,
        CP_GERMANY,
        CP_GB,
        CP_DENMARK1,
        CP_SWEDEN,
        CP_ITALY,
        CP_SPAIN1,
        CP_JAPAN,
        CP_NORWAY,
        CP_DENMARK2,
        CP_SPAIN2,
        CP_LAT_AM,
        CP_437           = 0x30,
        CP_850,
        CP_852,
        CP_857,
        CP_866           = 0x35,
        CP_737,
        CP_862,
        CP_IBM813,
        CP_KATAKANA      = 0x63
    } code_page;
    /***************************************/

    void begin(HardwareSerial&, int);   // Init on hardware serial
                                        // (needs an external TTL inverter or the RS232 chip must be bridged)
    void begin(Stream&);                // Init on a stream object (e.g. SoftSerial)
    
    void setCodePage(code_page);        // Code page selection
    void clear();                       // Clear screen (without return to (0,0))
    void home();                        // Return to (0,0)
    void deleteToEOL();                 // Deletes evetything till the end of line
    void setCursor(int, int);           // Set cursor to col/row starting from 0.
    int cursorX();                      // Get cursor column. Returns the position tracked by the library and not the actual display position
    int cursorY();                      // Get cursor row
    
    virtual size_t write(uint8_t);      // Reserved for Print.h integration. Same as write(uint8_t, SYMBOL);
    void write(uint8_t, write_type);    // Send a byte to display. write_type selects whether a symbol should be
                                        // sent directly to display (CMD) or treated as a symbol (code page conversion +
                                        // cursor pos changer) (SYMBOL)
```
The `NixdorfVFD` class implements methods from Arduino's `Print.h`, so you can also use various print functions found in there, such as
```c
    size_t print(...)
    size_t println(...)
```
similar to using `Serial.print(...)`/`Serial.println(...)`

Using `void setCodePage(code_page)` sends a code page select command to the display, as well as enables built-in code page conversion. For now the only code page handler available is for ***Russian CP866***, but they can be added as new functions into ![codepages.cpp](https://github.com/MrTransistorsChannel/NixdorfVFD/blob/master/src/codepages.cpp). If the selected code page doesn't have a dedicated handler, the symbols are sent directly to the display without conversion
