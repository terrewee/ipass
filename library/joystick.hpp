#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

// class for an analog joystick
class joyStick {
private:
    target::pin_adc joyX;
    target::pin_adc joyY;
    target::pin_in button;


    // custom converter to change the value that is returned from the x pin into a readible format
    int translatorX(int original){
        if(original < 2020){return -9 + (original/220);}
        else if(original > 2120){return 0 + ((original-2000)/221);}
        else{return 0;}
    }

    // custom converter to change the value that is returned from the y pin into a readible format
    int translatorY(int original){
        if(original < 1980){return -9 + (original/220);}
        else if(original > 2100){return 0 + ((original-2050)/221);}
        else{return 0;}
    }
public:
    joyStick(target::pin_adc x, target::pin_adc y, target::pin_in button):
        joyX(x),
        joyY(y),
        button(button)
    {}
    
    // return a read x pin value that is converted to a range of -9 to 9
    int readX(){
        return translatorX(joyX.read());
    }

    // return a read y pin value that is converted to a range of -9 to 9
    int readY(){
        return translatorY(joyY.read());
    }
    
    // return raw x pin reading
    int rawX(){
        return joyX.read();
    }

    // return raw y pin reading
    int rawY(){
        return joyY.read();
    }
    
    // return wether the button is pressed as a bool
    bool readButton(){
        return !button.read();
    }   
};

#endif