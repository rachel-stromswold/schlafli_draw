#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <GL/glut.h>
#include <SFML/Graphics.hpp>
#include <string>

class Diagram {
private:
    int m_p;
    int m_q;
    int m_r;
    int m_s;
    bool m_tess=false;

    int m_scale;
    int m_centerX;
    int m_centerY;
    int m_centerZ;

    sf::RenderWindow* m_w;

    sf::VertexArray m_shape;
public:
    Diagram();
    Diagram(sf::RenderWindow* window, int centerX, int centerY, std::string str = "");

    bool IsGood(sf::Vertex vert1, sf::Vertex vert2);
    //bool CanAdd(int x, int y, sf::VertexArray arr);
    void SetPQR(std::string  str); // Sets p, q, r, and s

    void MakePoly(std::string str = "");
    void MakeDiagram();
    void Tessellate();
    void CreateNet(int scale);
    void MakeSolid();

    sf::Color Colorgen(int seed);
    void Draw();
};

// Converts a string with a number in it to an integer containing that number; move to better location in the future
int ToInt(std::string str);
sf::Color HSVtoRGB(int hue, double sat, double val);

#endif //H_DIAGRAM
