#ifndef DRAWOBJECTS_HPP
#define DRAWOBJECTS_HPP

#include "library/ipass.hpp"


//simple within check
bool within( int x, int a, int b ){
   return ( x >= a ) && ( x <= b );
}

/// a collidable object base class, useful to make invisible walls or make other objects collidable
class collideObject : public object{
public:
    bool collide[2];
    collideObject(windowDraw &window, hwlib::xy start, hwlib::xy end, bool collision1, bool collision2):
        object(window.window, start, end),
        collide{collision1, collision2}
    {}
    virtual void moveTo(hwlib::xy target){}
};

/// combines the aspects of a normal line from the drawObject library with the collideable object class to make a collidable line
class collideLine : public line, public collideObject{
public:
    collideLine(windowDraw &window, hwlib::xy start, hwlib::xy end, bool collision1, bool collision2):
        line(window, start, end),
        collideObject(window, start, end, collision1, collision2)
    {}
    void draw(){
        line::draw();
    }
    void moveTo(hwlib::xy target){
        line::moveTo(target);
    }
};

/// ===============================bullets====================================

/// bullets are meant to be shot from the ship, it interacts with collidable objects and removes them from play
class bullet : public collideLine{
private:
    hwlib::xy speed = hwlib::xy(0,0);
    hwlib::xy storage = hwlib::xy(-10,-10);
    void moveTo(hwlib::xy target){
        collideLine::moveTo(target); 
    }
    void moveRelative(){
        collideLine::moveRelative(speed.x, speed.y);
    }
public:
    bullet(windowDraw &window, hwlib::xy start, hwlib::xy end, bool collision1, bool collision2):
        collideLine(window, start, end, collision1, collision2)
        {}
    void draw(){
        collideLine::draw();
    }
    // move objects out of the field
    void interact(collideObject &other){
        if( this != &other && (other.collide[0] == 1 || other.collide[1] == 1)){
            if((within(line::start.x-1, other.start.x, other.start.x + other.end.x) || within(other.start.x-1, line::start.x, line::start.x + line::end.x)) &&
               (within(line::start.y-1, other.start.y, other.start.y + other.end.y) || within( other.start.y-1, line::start.y, line::start.y + line::end.y)))
               {other.moveTo(storage);
                moveTo(storage);}
        }
    }
    // move to the ship and accelerate in accordance to the ship
    void shot(hwlib::xy location, hwlib::xy speedNew){
        speed.x = speedNew.x * 1.5 +5;
        speed.y = speedNew.y * 1.5;
        moveTo(location);
    }

    void move(){
        moveRelative();
    }

};


///==================================ship=====================================

/// a ship object that can move around and collide with objects or stay within ceryain bounds
class ship : public collideObject{
private:
    bool colBounce;
    hwlib::xy speed = hwlib::xy(0,0);
    line back, front;
    hwlib::xy boundS,
              boundE;

    // relative movement from position
    void moveRelative(int x, int y){
        start.x = start.x + x;
        start.y = start.y + y;
        end.x = end.x + x;
        end.y = end.y + y;
        back.moveRelative(x, y);
        front.moveRelative(x, y);
    }
public:
    ship(windowDraw &window, hwlib::xy start, hwlib::xy end, bool bouncy, hwlib::xy boundS, hwlib::xy boundE):
        collideObject(window, start, end, 1, 1),
        colBounce(bouncy),
        back(line(window, start, hwlib::xy(start.x, end.y))),
        front(line(window, hwlib::xy(start.x,start.y+((end.y-start.y)/2)), hwlib::xy(end.x,start.y+((end.y-start.y)/2)))),
        boundS(boundS),
        boundE(boundE)

    {}
    // draws the parts and fushes the window because this is the most relevant item
    void draw() override{
        back.draw();
        front.draw();
        window.flush();
    }
    // interact with collidable objects
    void interact(collideObject &other){
        // if they have an assigned collision axis they will flip said axis on collision
        if(colBounce && other.collide){
            if( this != &other){
                if((within(start.x-1, other.start.x, other.start.x + other.end.x) || within(other.start.x-1, start.x, start.x + end.x)) &&
                   (within(start.y-1, other.start.y, other.start.y + other.end.y) || within( other.start.y-1, start.y, start.y + end.y)))
                    {if(other.collide[0] == 1){speed.x = (speed.x/2) * -1;}
                     if(other.collide[1] == 1){speed.y = (speed.y/2) * -1;}}
            }
        }
        // if colBounce is turned off it will look at the boundaries and stay in them
        else{
            if(start.x < boundS.x){
                speed.x = (speed.x/2) * -1;
                moveTo(hwlib::xy(boundS.x,start.y));    
            }
            if(end.x > boundE.x){
                speed.x = (speed.x/2) * -1;
                moveTo(hwlib::xy(boundE.x-(end.x - start.x),start.y));
                }
            if(start.y < boundS.y){
                speed.y = (speed.y/2) * -1;
                moveTo(hwlib::xy(start.x,boundS.y));
                }
            if(end.y > boundE.y){
                speed.y = (speed.y/2) * -1;
                moveTo(hwlib::xy(start.x,boundE.y-(end.y - start.y)));
            }
        }
    }
    
    // standard acceleration from input and speed reduction if no input is given
    void moveWithSpeed(int x, int y){
        if(x == 0 && y == 0){
            speed.x = speed.x/2;
            speed.y = speed.y/2;
        }
        
        else{
            speed.x = speed.x + (x/2);
            speed.y = speed.y + (y/2);
        }
        moveRelative(speed.x, speed.y);
    }   

    // standard move operation
    void moveTo(hwlib::xy target) override{
        end = end - start + target;
        start = target;
        back.moveTo(start);
        front.moveTo(hwlib::xy(start.x,start.y+((end.y-start.y)/2)));
    }

    void shoot(bullet &bullet){
        bullet.shot(hwlib::xy(end.x,start.y+((end.y-start.y)/2)), speed);
    }
};



#endif