#include <Arduino.h>
#include "NixdorfVFD.h"

uint8_t NixdorfVFD::toSelectedCodePage(uint8_t c){
    switch(_codePage){
        case CP_866:
            return to_cp866(c);
    }
}

uint8_t NixdorfVFD::to_cp866(uint8_t c){
    if(c == 0xd0 || c == 0xd1){
        _sel = c;
        return 0;
    }
    else{
        if(_sel == 0xd0){
            _sel = 0;
            if(c >= 0x90 && c <= 0xBF)
                return c - 0x10;
            else if(c == 0x1)
                return 0xf0;
        }
        else if(_sel == 0xd1){
            _sel = 0;
            if(c >= 0x80 && c <= 0x8F)
                return c + 0x60;
            else if(c == 0x91)
                return 0xf1;
        }
        else return c;
    }
}