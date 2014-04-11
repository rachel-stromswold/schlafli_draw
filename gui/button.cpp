#include <SFML/Graphics.hpp>
#include <string>
#include "button.h"
#include "diagram.h"

void Button::draw(){
    w->draw(rectangle);
    w->draw(text);
}

void Button::press(int xP, int yP, InputBox b){
    if(x<xP && xP<x+width*15 && y<yP && yP<y+height*15){
        d=Diagram(w,0,0,100,100,b.stored.getString());
    }
}
