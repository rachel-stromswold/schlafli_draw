#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <math.h>
#ifndef H_DIAGRAM
#define H_DIAGRAM

class Diagram{
public:
    int x;
    int y;
    int width;
    int height;

    int p;
    int q;
    int r;

    sf::VertexArray shape;

    Diagram(){
        x=0;y=0;width=0;height=0;p=1;q=1;r=0;
    }

    Diagram(sf::RenderWindow* myWindow,int myX,int myY,int myWidth,int myHeight,std::string str): shape(){
        x=myX;
        y=myY;
        width=myWidth;
        height=myHeight;

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

        shape.setPrimitiveType(sf::LinesStrip);
        shape.resize(p);

        for(int i=0;i<shape.getVertexCount();i++){
            std::cout<<cos(i*6.283/p)+x<<std::endl;
            std::cout<<sin(i*6.283/p)+y<<std::endl;
            shape[i].position=sf::Vector2f(cos(i*6.283/p)+x,sin(i*6.283/p)+y);
            shape[(i+1)%p].position=sf::Vector2f(cos((i+1)%p*6.283/p)+x,sin((i+1)%p*6.283/p)+y);
            shape[i].color=sf::Color::White;
            shape[(i+1)%p].color=sf::Color::White;
        }
    };

    void draw();
private:
    sf::RenderWindow* w;
    sf::RectangleShape rectangle;

    int toInt(std::string str){
        int ret=0;
        for(int i=str.length()-1;i>=0;i--){
            ret+=pow(10,str.length()-1-i)*(str[i]-'0');
        }
        return ret;
    }
};

#endif //H_DIAGRAM
