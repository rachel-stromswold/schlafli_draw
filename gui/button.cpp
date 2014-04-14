#include <SFML/Graphics.hpp>
#include <string>
#include "button.h"
#include "diagram.h"
#include <iostream>

int toInt(std::string str){
    int ret=0;
    for(int i=str.length()-1;i>=0;i--){
        ret+=pow(10,str.length()-1-i)*(str[i]-'0');
    }
    return ret;
}

sf::VertexArray Button::getDiagram(std::string str){
    int p=0;int q=1;
    if(str.find('{')!=std::string::npos && str.rfind('}')!=std::string::npos){//if both { and } are present
        str=str.substr(str.find('{')+1,str.rfind('}')-str.find('{')-1);
    }

    if(str.find('/')!=std::string::npos){
        p=toInt(str.substr(0,str.find('/')));
        q=toInt(str.substr(str.find('/')+1,std::string::npos));
    }else{
        p=toInt(str);
        q=1;
    }

    sf::VertexArray ret(sf::Lines,p);

    int currAngle=0;
    int nextAngle=0;
    int scale=64;
    int offset=100;

    for(int i=0;i<ret.getVertexCount();i++){
        //std::cout<<i*6.283/p<<std::endl;
        //std::cout<<(i+q)%p*6.283/p<<std::endl;

        //calculates the angle that each given point will be at
        currAngle=i*6.283/p;
        nextAngle=(i+q)%p*6.283/p;

        //Actually adds in the lines to the diagram
        ret[i].position=sf::Vector2f(cos(currAngle)*scale+offset,sin(currAngle)*scale+offset);
        ret[i+1].position=sf::Vector2f(cos(nextAngle)*scale+offset,sin(nextAngle)*scale+offset);
        ret[i].color=sf::Color::White;
        ret[i+1].color=sf::Color::White;
    }

    return ret;
}

void Button::draw(){
    w->draw(rectangle);
    w->draw(text);
    w->draw(shape);
}

void Button::press(int xP, int yP, InputBox b){
    if(x<xP && xP<x+width*15 && y<yP && yP<y+height*15){
        //d=Diagram(w,0,0,100,100,b.stored.getString());
        shape=getDiagram(b.stored.getString());
    }
}
