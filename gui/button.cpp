#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>
#include "button.h"
#include "diagram.h"
#include <iostream>

#define PI 3.1415926535897932
#define TAU 2 * PI
#define GROWTH_NUM 3

bool isGood(sf::VertexArray* a, int x, int y){
    sf::VertexArray arr=*a;
    int xx;
    int yy;
    if(arr.getVertexCount()==0)
        return true;
    for(int i = 0; i < arr.getVertexCount()-1; i+=2){
        xx=arr[i].position.x;
        yy=arr[i].position.y;
        if(x-xx<5 || xx-x<5 || yy-y<5 || y-yy<5){
            return false;
        }
    }
    return true;
}

void grow(sf::VertexArray* arr, int x, int y, double angle, double delta, int scale, int i){
    if(i>0/* && isGood(arr,x,y)*/ ){//if we are reasonably close
        for(double ii=angle+delta;ii<angle+2*PI;ii+=delta){
            arr->append( sf::Vertex( sf::Vector2f(x,y) ) );
            grow(arr,x+scale*cos(angle+ii),y-scale*sin(angle+ii),angle+ii,delta,scale,i-1);
        }
    }else{
        //arr->append( sf::Vertex( sf::Vector2f(x,y) ) );
    }
}

Button::Button(sf::RenderWindow* window, sf::Font font, int x, int y, int width, int height, std::string str) :
        m_text(),
        m_rectangle(sf::Vector2f(width * 15, height * 15)) ,
        m_f(font)
    {
        m_w = window;
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;

        m_text.setString(str);
        m_text.setFont(m_f);
        m_text.setCharacterSize(15);
        m_text.setColor(sf::Color::Black);
        m_rectangle.setOutlineThickness(2);
        m_rectangle.setOutlineColor(sf::Color(255, 255, 255));

        m_rectangle.setPosition(x, y);
        m_text.setPosition(x, y - 2);
    }

sf::VertexArray Button::GetDiagram(std::string str) {
    int p = 0;
    int q = 1;
    int r = -1;

    if(str.find('{') != std::string::npos && str.rfind('}') != std::string::npos) {//if both { and } are present
        str = str.substr(str.find('{') + 1, str.rfind('}') - str.find('{') - 1);
    }

    if(str.find('/') != std::string::npos) {
        p = ToInt(str.substr(0, str.find('/')));
        q = ToInt(str.substr(str.find('/') + 1, std::string::npos));
    } else if(str.find(',') != std::string::npos){
        p = ToInt(str.substr(0, str.find(',')));
        r=ToInt(str.substr(str.find(',')+1, std::string::npos));

        //(p-2)*180/p is the interior angle of 1 vertex on the regular shape
        if( r*((p-2)*180/p) !=360 ){
            std::cout<<p<<std::endl;
            r=-1;
        }
    }else{
        p = ToInt(str);
        q = 1;
    }


    //std::cout<<r;
    sf::VertexArray vertices = sf::VertexArray(sf::Points, 2*p);

    // The initial angle (for the first vertex)
    double angle = PI / 2;
    //double angle=0;
    int scale = 220;
    int centerX = 300;
    int centerY = 250;

    if(r==-1){
        // Calculate the location of each vertex
        for(int iii = 0; iii < vertices.getVertexCount()-1; iii+=2) {
            // Adds the vertices to the diagram
            vertices[iii].position = sf::Vector2f(centerX + cos(angle) * scale, centerY - sin(angle) * scale);
            angle += q*(TAU / p);
            vertices[iii+1].position = sf::Vector2f(centerX + cos(angle) * scale, centerY - sin(angle) * scale);
            angle -= (q-1)*(TAU / p);
            vertices[iii].color = sf::Color::White;
        }
    }else{
        scale=150;//smaller tessalations
        vertices = sf::VertexArray();
        vertices.setPrimitiveType(sf::Points);
        double angle=(p-2)*PI/p;

        grow(&vertices,centerX,centerY,0.0,angle,scale,4);
    }

    // Draws the lines on the diagram
    sf::VertexArray lines = sf::VertexArray(sf::Lines, vertices.getVertexCount());
    for(int iii = 0; iii < lines.getVertexCount()-1; iii+=2) {
        lines[iii] = vertices[iii];
        lines[iii+1] = vertices[iii+1];
        lines[iii].color = sf::Color(255,(iii)*50,255,255);//sf::Color::White;
        lines[iii+1].color = sf::Color(255,(iii+1)*50,255,255);//sf::Color::White;
    }
    return lines;
}

void Button::Draw(){
    m_w->draw(m_rectangle);
    m_w->draw(m_text);
    m_w->draw(m_shape);
}

void Button::Press(int xP, int yP, InputBox b){
    if(m_x < xP && xP <m_x + m_width * 15 && m_y < yP && yP < m_y + m_height * 15) {
        m_shape = GetDiagram(b.GetStoredString());
    }
}
