#include <SFML/Graphics.hpp>
#include <string>
#include "text.h"

void InputBox::draw(){
    w->draw(rectangle);
    w->draw(stored);
}

void InputBox::enterText(char n){
    if(n!=8 && n!=13 && n<128){
        stored.setString(stored.getString()+n);
    }else if(n==8){
        std::string temp=stored.getString();
        if(temp.length()>0)
            stored.setString(temp.substr(0,temp.length()-1));
    }else if(n==13){
        //do whatever we do when people hit enter
    }
}
