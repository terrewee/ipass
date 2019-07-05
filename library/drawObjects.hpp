#ifndef DRAW_HPP
#define DRAW_HPP

#include "hwlib.hpp"

///base object template to draw on windows
class object {
protected:
   hwlib::window &window;

public:
   hwlib::xy start, end;

    object(hwlib::window &window, hwlib::xy &start, hwlib::xy &size):
        window(window),
        start(start),
        end(size)
    {}

    virtual void draw();
};

/// a pixel that is drawable
class dot : object{
public:
    dot(hwlib::window &window, hwlib::xy start, bool bounce, bool collide):
        object(window, start, start)
    {}

    void draw() override{
        window.write(start);
    }
};

/// standard drawable line that can be moved around and transformed
class line : public object{
public:
    line(windowDraw &window, hwlib::xy start, hwlib::xy end):
        object(window.window, start, end)
    {}
   
    void draw() override {
        hwlib::line line(start, end);
        line.draw(window);
    }
    // move begin drawpoint to location
    void moveStart(hwlib::xy newLocation){
        start.x = newLocation.x;
        start.y = newLocation.y;
        end.x = end.x + newLocation.x;
        end.y = end.y + newLocation.y;
    }
    // move end draw point to location
    void moveEnd(hwlib::xy newEnd){
        end.x = newEnd.x;
        end.y = newEnd.y;
    }
    // move entire line relative to won location
    void moveRelative(int x, int y){
        start.x = start.x + x;
        start.y = start.y + y;
        end.x = end.x + x;
        end.y = end.y + y;
    }
    // move entire line to target location
    void moveTo(hwlib::xy target){
        end = end - start + target;
        start = target;
    }
};


#endif