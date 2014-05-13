#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <SFML/OpenGL.hpp>
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
    sf::Vector3f m_center;
    sf::RenderWindow* m_w;

    sf::VertexArray m_shape;
    std::vector<sf::Vector3f> m_vertices;
public:
    Diagram();
    Diagram(sf::RenderWindow* window, int centerX, int centerY, std::string str = "");

    bool IsGood(sf::Vertex vert1, sf::Vertex vert2);
    bool IsGood(sf::Vector3f vect1, sf::Vector3f vect2, sf::Vector3f vect3);
    bool CanAdd(int x, int y, int z, double * arr, int len);
    void SetPQR(std::string  str); // Sets p, q, r, and s
    double GetAngle(); // Returns the "bend angle" for polyhedra

    void MakePoly(std::string str = "");
    void MakeDiagram();
    void Tessellate();
    void MakeSolid();
    void CreateNet(int scale, double * arr, int len);

    sf::Vector3f RotatePointAboutLine(sf::Vector3f p, double theta, sf::Vector3f p1, sf::Vector3f p2);
    sf::Color Colorgen(int seed);
    void Draw();
};

// Converts a string with a number in it to an integer containing that number; move to better location in the future
int ToInt(std::string str);
sf::Color HSVtoRGB(int hue, double sat, double val);

bool IsApproxEqual(sf::Vector3f v1, sf::Vector3f v2);

#endif //H_DIAGRAM
