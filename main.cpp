#include "objects.hpp"


int main( void ){	
    
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    tiltSwitch ballx = tiltSwitch(target::pins::d50);

    joyStick joy = joyStick(target::pin_adc(target::ad_pins::a4), target::pin_adc(target::ad_pins::a5), target::pin_in(target::pins::d47));

    auto scl = target::pin_oc(target::pins::scl);
    auto sda = target::pin_oc(target::pins::sda);
    display screen = display(scl, sda );

    windowText w0(screen, hwlib::xy(0,0), hwlib::xy(128,64), hwlib::font_default_16x16());
    windowText w2(screen, hwlib::xy(0,0), hwlib::xy(128,64), hwlib::font_default_8x8());
    w0.area 
            << "\fJoystick controlled\ninterface"
            << hwlib::flush;
    hwlib::wait_ms(1000);        
    w2.area
            << "\fStarting up\nplease wait..."
            << hwlib::flush;
    hwlib::wait_ms(1000);

    windowDraw w1(screen, hwlib::xy(0,0), hwlib::xy(128,64));
    ship player = ship(w1, hwlib::xy(0,0), hwlib::xy(7,7), 0, hwlib::xy(1,1),hwlib::xy(126,62));

    collideLine topLine = collideLine(w1, hwlib::xy(0,0), hwlib::xy(127,0), 0, 1);
    collideLine leftLine = collideLine(w1, hwlib::xy(0,0), hwlib::xy(0,63), 1, 0);
    collideLine botLine = collideLine(w1, hwlib::xy(0,63), hwlib::xy(127,63), 0, 1);
    collideLine rightLine = collideLine(w1, hwlib::xy(127,0), hwlib::xy(127,63), 1, 1);

    bullet bul1 = bullet(w1, hwlib::xy(-10,-10), hwlib::xy(-7,-10), 0, 0);
    bullet bul2 = bullet(w1, hwlib::xy(-10,-10), hwlib::xy(-7,-10), 0, 0);
    bullet bul3 = bullet(w1, hwlib::xy(-10,-10), hwlib::xy(-7,-10), 0, 0);

    std::array<collideObject*, 8> objectList = {&bul1, &bul2, &bul3, &topLine, &leftLine, &botLine, &rightLine, &player};

    std::array<bullet*, 3> bulletInteraction = {&bul1, &bul2, &bul3};


    int count = 1;
    while(1){
        
        if(joy.readButton()){
            count++;
            if(count == 1){
                player.shoot(bul1);}
            if(count == 2){
                player.shoot(bul2);}
            if(count == 3){
                player.shoot(bul3);
                count = 0;}
        }
        
        player.moveWithSpeed(joy.readX(), joy.readY());
        for(auto &bullets : bulletInteraction){bullets->move();}

        for(auto &objects : objectList){
            player.interact(*objects);
        }

        w1.window.clear();
        for(auto &objects : objectList){objects->draw();}
        for(auto &bullets : bulletInteraction){bullets->draw();}


    }
}