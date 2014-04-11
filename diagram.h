#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Diagram{
public:
    int x;
    int y;
    int width;
    int height;

    int p;
    int q;
    int r;

    Button(sf::RenderWindow* myWindow,int myX,int myY,int myWidth,int myHeight,std::string str){
        x=myX;
        y=myY;
        width=myWidth;
        height=myHeight;

        if(str.find('{')!=std::string::npos && str.rfind('}')!=std::string::npos){//if both { and } are present
            str=str.substr(str.find('{')+1,str.rfind('}')-str.find('{')-1);
        }

        if(str.find('/')!=std::string::npos){
            p=std::atoi(str.substr(0,str.find('/')-1));
            std::cout<<p;
            q=std::atoi(str.substr(str.find('/')+1,std::string::npos));
            std::cout<<q;
        }else{
            p=std::atoi(str);
            std::cout<<p;
            q=1;
            std::cout<<q;
        }
    };
private:
    sf::RenderWindow* w;
    sf::RectangleShape rectangle;
};
