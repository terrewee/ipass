#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "hwlib.hpp"

namespace target = hwlib::target;

//===========================================================================================

// initializes your display on the given pins, standard scl and sda pins are adviced.
class display{   
private:
public:
    hwlib::i2c_bus_bit_banged_scl_sda i2c_bus;

    display(target::pin_oc &scli, target::pin_oc &sdai) :  
        i2c_bus(hwlib::i2c_bus_bit_banged_scl_sda( scli, sdai ))
    {}
};

///window for drawing objects
class windowDraw{
private:    
    hwlib::glcd_oled oled;
public:
    hwlib::window_part window;
    windowDraw(display &display, hwlib::xy start, hwlib::xy end):
        oled(hwlib::glcd_oled(display.i2c_bus, 0x3c)),
        window(hwlib::window_part(oled, start, end))
    {window.clear();}
};

///window for writing text
class windowText : windowDraw{
private:
    hwlib::font_default_8x8 x8 = hwlib::font_default_8x8();
    hwlib::font_default_16x16 x16 = hwlib::font_default_16x16();
public:
    hwlib::terminal_from area;
    windowText(display &display, hwlib::xy start, hwlib::xy end, hwlib::font_default_8x8 statei):
        windowDraw(display, start, end),
        area(hwlib::terminal_from(window, x8))
        {}
    windowText(display &display, hwlib::xy start, hwlib::xy end, hwlib::font_default_16x16 statei):
        windowDraw(display, start, end),
        area(hwlib::terminal_from(window, x16))
        {}
};

#endif