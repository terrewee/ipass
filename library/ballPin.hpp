#ifndef BALL_PIN_HPP
#define BALL_PIN_HPP

class tiltSwitch{
private:
    hwlib::target::pin_in pin;
public:
    tiltSwitch(hwlib::target::pins pinin):
        pin(hwlib::target::pin_in(pinin))
    {}
    
    bool read(){
        return pin.read();
    }
};

#endif