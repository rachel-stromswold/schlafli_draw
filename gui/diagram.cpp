#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include "diagram.h"

#define PI (atan(1) * 4)
#define TAU (2 * PI)

#define TET_PHI 19.471220333
#define CUBE_PHI 35.264391
#define OCT_PHI 0.0
#define ICO_PHI 26.56505
#define DOD_PHIA 52.62263590
#define DOD_PHIB 10.81231754
//TODO: find how to actually calculate these values

Diagram::Diagram() {
    m_p = 2;
    m_q = 1;
    m_r = -1;

    m_scale = 220;
    m_centerX = 300;
    m_centerY = 250;
}

Diagram::Diagram(sf::RenderWindow* window, int centerX, int centerY, std::string str) :
    m_shape(sf::Lines, 0){
    m_w = window;
    if (!str.empty())
        SetPQR(str);
    m_scale = 200;
    m_centerX = centerX;
    m_centerY = centerY;
};

bool Diagram::IsGood(sf::Vertex vert1, sf::Vertex vert2) {
    if(m_shape.getVertexCount() <= 1) // An empty set, or only one point, doesn't have any lines for us to overlap with
        return true;
    for(int iii = 0; iii < m_shape.getVertexCount() - 1; iii += 2) { // Check all lines in m_shape
        // Returns true if the two vertices appear in sequence anywhere in m_shape (not necessarily in order)
        if(m_shape[iii].position.x == vert1.position.x && m_shape[iii].position.y == vert1.position.y &&
           m_shape[iii + 1].position.x == vert2.position.x && m_shape[iii + 1].position.y == vert2.position.y ||
           m_shape[iii].position.x == vert2.position.x && m_shape[iii].position.y == vert2.position.y &&
           m_shape[iii + 1].position.x == vert1.position.x && m_shape[iii + 1].position.y == vert1.position.y)
        {
            return false;
        }
    }
    return true;
}

bool Diagram::CanAdd(int x, int y, int z, double * arr, int len){
    for(int i=0;i+2<len;i+=3){
        if(x>arr[i]-.001 && x<arr[i]
            && y>arr[i+1]-.001 && y<arr[i+1]+.001
            && z>arr[i+2]-.001 && z<arr[i+2]+.001 ){
                return false;
        }
    }
    return true;
}

void Diagram::SetPQR(std::string str) {
    m_p = 2; // Empty schlafli set = line segment
    m_q = 1;
    m_r = -1;
    m_s = -1;

    if(str == "") return;

    if(str.find('{') != std::string::npos) {  // If '{' is present
        str = str.substr(str.find('{') + 1, str.length() - 1); // Edit the string to remove it
    }
    if(str.find('}') != std::string::npos) {  // If '}'
        str = str.substr(0, str.find('}')); // Edit the string to remove it
    }

    std::string pqStr = str.substr(0, str.find(',')); // Look only up until the first comma, if there is one
    if(pqStr.find('/') != std::string::npos) { // If there's a slash, we need to set both p and q
        m_p = ToInt(pqStr.substr(0, pqStr.find('/')));
        m_q = ToInt(pqStr.substr(pqStr.find('/') + 1, std::string::npos));
    } else { // Otherwise, we can just set p
            m_p = ToInt(pqStr); // No commas, slashes or braces, so the string is just p
    }
    if(str.find(',') != std::string::npos) { // There's a comma, so we'll have to at least set r as well
        str = str.substr(str.find(',') + 1, std::string::npos); // Look only past the first comma
        if(str.find('/') != std::string::npos) { // There's a slash in the second part
            m_r = ToInt(str.substr(0, str.find('/')));
            m_s = ToInt(str.substr(str.find('/') + 1, std::string::npos));
        } else { // No slash present
            m_r = ToInt(str);
        }
    }
        //(p-2)*180/p is the interior angle of 1 vertex on the regular shape
        if(m_r * ((m_p - 2) * 180 / m_p) == 360){
            m_tess = true; // This is a tesselation of a plane
        } else {
            m_tess = false;
        }
}

void Diagram::MakeDiagram(std::string str) {
    if(str != "")
        SetPQR(str);

    if(m_r == -1) {
        CreatePoly();
    } else if(m_tess){ // r > 0
        Tesselate();
    } else {
        double l=-2/((double)(m_r)/2-(double)(m_r)/(double)(m_p)-1);
        double verts[3*(int)l];//3 times the number of needed verts
        CreateNet(10,verts,3*(int)l);
    }
}

void Diagram::CreatePoly() {
    m_scale = (m_w->getSize().y * .99) / 2.2; // Scale to window size
    m_shape = sf::VertexArray(sf::Lines, 2 * m_p);
    // The initial angle (for the first vertex)
    double angle = PI / 2 + (PI / m_p * ((m_p + 1) % 2)); // Ensure there is a flat line on the bottom
    // Calculate the location of each vertex
    for(int iii = 0; iii < m_shape.getVertexCount() - 1; iii += 2) {
        // Adds the vertices to the diagram
        m_shape[iii].position = sf::Vector2f(m_centerX + cos(angle) * m_scale,
                                             m_centerY - sin(angle) * m_scale);
        angle += m_q * (TAU / m_p);
        m_shape[iii + 1].position = sf::Vector2f(m_centerX + cos(angle) * m_scale,
                                                 m_centerY - sin(angle) * m_scale);
        angle -= (m_q - 1) * (TAU / m_p);
        m_shape[iii].color = Colorgen(iii);
        m_shape[iii + 1].color = Colorgen(iii + 1);
    }
}

void Diagram::Tesselate() {
    m_scale = 50; // smaller tessellations
    m_shape = sf::VertexArray(sf::Lines, 1);
    double delta = (m_p - 2) * PI / m_p; // The amount by which we rotate for the tessellation
    m_shape[0] = sf::Vertex(sf::Vector2f(m_centerX, m_centerY)); // Create our starting vertex at the center
    m_shape[0].color = Colorgen(639); // Set starting color
    for(int iii = 0; iii < m_shape.getVertexCount(); iii += 2) { // Go until all vertices are off screen
        int x = m_shape[iii].position.x, y = m_shape[iii].position.y; // For ease of use
        if(0 < x && x < m_w->getSize().x && 0 < y && y < m_w->getSize().y) { // False if the vertex is out of the window
            m_shape[m_shape.getVertexCount() - 1] = m_shape[iii]; // Set the last vertex to the current center one
            double initAngle = 0;
            if(iii != 0) { // First run doesn't have any lines to work from, so just start with 0
                // Set our initial angle to be that of the line we came on, so that we draw lines relative to that
                initAngle += atan(-(m_shape[iii].position.y - m_shape[iii - 1].position.y) /
                                   (m_shape[iii].position.x - m_shape[iii - 1].position.x));
                if (x >= m_shape[iii - 1].position.x) initAngle += PI; // Compensate for the restricted range of atan
            }
            for(double angle = initAngle; angle < TAU + initAngle; angle += delta) {
                // Create the next vertex
                sf::Vertex vert = sf::Vertex(sf::Vector2f(m_shape[iii].position.x + m_scale * cos(angle),
                                                          m_shape[iii].position.y - m_scale * sin(angle)));
                vert.color = Colorgen(m_shape.getVertexCount() - 1); // Set the color
                if(IsGood(vert, m_shape[m_shape.getVertexCount() - 1])) { // Don't draw over existing lines
                    m_shape.append(vert); // Add the vertex (completes a line with the center vertex)
                    m_shape.append(m_shape[iii]); // Add the center vertex again to begin more lines
                }
            }
        }
        if(iii == 0) iii--; // Want to only deal with odd-numbered vertices after first run
    }
    m_shape.resize(m_shape.getVertexCount() - 1); // Remove the last vertex, since we have one too many
}

void Diagram::CreateNet(int scale, double * arr, int len) {
    int loops=m_r-1;//usually this works except for the dodecohedron

    int ii=0;

    double phi=0.0;
    double ang=0.0;
    double theta=0.0;

    double x=0;
    double y=0;
    double z=0;

    //TODO: actually find a formula for these values
    switch(2/(m_p/m_q - m_p/2 + 1)){
    case 4:
        phi=TET_PHI;
        break;
    case 6:
        phi=CUBE_PHI;
        break;
    case 8:
        phi=OCT_PHI;
        break;
    case 20:
        phi=ICO_PHI;
        break;
    default:
        break;
    }

    if(m_p==5){//the dodecahedron is a special and very annoying case
        loops=4;//for the dodecahedron
    }else{
        for(double i=PI/2;i>=0;i-=PI/(2*loops)){
            ang=(double)((int)(0.99+cos(i)))*ICO_PHI;
            //std::cout<<ang<<std::endl;
            //std::cout<<std::endl;
            theta=0;
            for(int j=0;j<11;j++){
                x=scale*cos(theta)*cos(ang);
                y=scale*sin(theta)*cos(ang);
                z=scale*sin(ang);
                if(CanAdd(x,y,z,arr,len) && ii+2<len){
                    arr[ii]=x;
                    std::cout<<x<<std::endl;
                    arr[ii+1]=y;
                    std::cout<<y<<std::endl;
                    arr[ii+2]=z;
                    std::cout<<z<<std::endl;
                    std::cout<<std::endl;
                    ii+=3;
                }
                theta+=TAU/(m_p);//increments the angle by the proper amount
            }
        }
    }
}

// Generate the colors for the lines so it's not all white and boring
sf::Color Diagram::Colorgen(int seed) {
    int hue = ((seed + 23657) * 15274) % 360;
    return HSVtoRGB(hue, 1, 1);
}

void Diagram::Draw() {
    m_w->draw(m_shape);
}

// Converts a string with a number in it to an integer containing that number
int ToInt(std::string str) {
    int ret = 0;
    for(int i = str.length() - 1; i >= 0; i--) {
        ret += pow(10, str.length() - 1 - i) * (str[i] - '0');
    }
    return ret;
}

// Convert HSV color values to RGB color values
sf::Color HSVtoRGB(int hue, double sat, double val) {
    hue %= 360;
    while(hue < 0) hue += 360;

    if(sat < 0) sat = 0.0;
    if(sat > 1) sat = 1.0;

    if(val < 0) val = 0.0;
    if(val > 1) val = 1.0;

    int h = hue / 60;
    double f = double(hue) / 60 - h;
    double p = val * (1 - sat);
    double q = val * (1 - sat * f);
    double t = val * (1 - sat * (1 - f));

    switch(h)
    {
        default:
        case 0:
        case 6: return sf::Color(val * 255, t * 255, p * 255);
        case 1: return sf::Color(q * 255, val * 255, p * 255);
        case 2: return sf::Color(p * 255, val * 255, t * 255);
        case 3: return sf::Color(p * 255, q * 255, val * 255);
        case 4: return sf::Color(t * 255, p * 255, val * 255);
        case 5: return sf::Color(val * 255, p * 255, q * 255);
    }
}
