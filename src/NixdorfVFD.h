#ifndef NixdorfVFD_h
#define NixdorfVFD_h

#include <Arduino.h>

/********** CONTROL SEQUENCES **********/
#define CLR_SC              "\e[2J"
#define HOME                "\e[1;1H"
#define DEL_TO_EOL          "\e[0K"
#define CP_SEL              "\eR"
/***************************************/

/********** TRANSMISSION TYPES *********/
typedef enum {
    SYMBOL = 0,
    CMD
} write_type;
/***************************************/

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

#define WIDTH               20              // Display width

// NixdorfVFD class implements methods from Print.h
class NixdorfVFD : public Print {
    public:
        void begin(HardwareSerial&, int);   // Init on hardware serial (needs an external TTL invertor)
        void begin(Stream&);                // Init on stream object (e.g. SoftSerial)
        
        void setCodePage(code_page);        // Code page selection
        void clear();                       // Clear screen (without return to (0,0))
        void home();                        // Return to (0,0)
        void deleteToEOL();                 // Deletes evetything till the end of line
        void setCursor(int, int);           // Set cursor to col/row starting from 0.
        int cursorX();                      // Get cursor column
        int cursorY();                      // Get cursor row

        virtual size_t write(uint8_t);      // Reserved for Print.h integration. Same as write(uint8_t, SYMBOL);
        void write(uint8_t, write_type);    // Send a byte to display. write_type selects whether a symbol should be
                                            // sent directly to display (CMD) or treated as symbol (code page conversion +
                                            // cursor pos changer) (SYMBOL)

    private:
        Stream* _stream;
        int _xpos = 0;
        int _ypos = 0;

        code_page _codePage = CP_GERMANY;   // Default page is German
        unsigned char _sel;

        uint8_t toSelectedCodePage(uint8_t);// Converts a symbol to selected code page or leaves as is if there's no implementation
        
        // ############## PUT CODE PAGE HANDLERS HERE ##############
        uint8_t to_cp866(uint8_t);          // Conversion to cp866
};
#endif
