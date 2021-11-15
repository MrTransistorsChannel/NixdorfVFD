#include <Arduino.h>
#include <Print.h>
#include "NixdorfVFD.h"

void NixdorfVFD::begin(HardwareSerial& serial, int baudRate){
    serial.begin(baudRate);
    begin((Stream&) serial);
}

void NixdorfVFD::begin(Stream& stream){
    _stream = &stream;
}

void NixdorfVFD::setCodePage(code_page page){
    _codePage = page;
    _stream->write(CP_SEL, 2);
    _stream->write(page);
}

void NixdorfVFD::clear(){
    _stream->write(CLR_SC, 4);
}

void NixdorfVFD::home(){
    _xpos = 0;
    _ypos = 0;
    _stream->write(HOME, 6);
}

void NixdorfVFD::deleteToEOL(){
    _stream->write(DEL_TO_EOL, 4);
}


void NixdorfVFD::setCursor(int x, int y){
    _xpos = x;
    _ypos = y;

    _stream->write("\e[", 2);
    _stream->print(y + 1);
    _stream->write(';');
    _stream->print(x + 1);
    _stream->write('H');
}

int NixdorfVFD::cursorX(){
    return _xpos;
}

int NixdorfVFD::cursorY(){
    return _ypos;
}

inline size_t NixdorfVFD::write(uint8_t b){
    write(b, SYMBOL);
    return 1;
}

void NixdorfVFD::write(uint8_t b, write_type type){
    if(type == CMD){
        _stream->write(b);
    } 
    else if(type == SYMBOL){
        if(b == '\n'){
            _ypos++;
            _xpos--;
        }
        if(b == '\r') _xpos = -1;
        if(_xpos == WIDTH){
            _xpos = 0;
            _ypos++;
            _stream->write("\n\r", 2);
        }
        b = toSelectedCodePage(b);
        if(b != 0){
            _xpos++;
            _stream->write(b);
        }
    }
}