#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <SFML/Graphics.hpp>
#include <string>

class Diagram {
private:
    // The inputted numbers {p/q, r/s}
    int m_p;
    int m_q;
    int m_r;
    int m_s;

    bool m_tess = false;   // True if we have a 2D tesselation, false otherwise (2D polygon or 3D polytope)
    bool m_displayEdges = true; // Whether or not we want to display the edges or faces of the polyhedron
    bool m_allWhite = false; // Whether or not we want to display colors on lines (does not affect faces)

    double m_rotateSpeed;  // The speed with which we rotate

    int m_scale;           // How large we make our polytope
    sf::Vector3f m_center; // The center of our polytope
    sf::RenderWindow* m_w; // The window to which we draw

    std::vector<sf::Vector3f> m_vertices; // Vertices of the polyhedron if it exists
    std::vector<std::vector<sf::Vector3f> > m_faces; // Faces of the polyhedron if it exists
    std::vector<sf::Color> m_colors; // Colors for the faces

    sf::VertexArray m_shape; // Our shape that actually gets drawn to the screen



    // Determine whether or not we already have added an edge (for 2D polygons) (false if the edge exists, else true)
    bool IsGood(sf::Vertex vert1, sf::Vertex vert2);
    // Determine whether or not we already have added an edge (for 3D polyhedra) (false if the edge exists, else true)
    bool IsGood(sf::Vector3f vect1, sf::Vector3f vect2);
    // Determine whether or not we already have added a face (for 3D polyhedra) (false if the edge exists, else true)
    bool IsGood(std::vector<sf::Vector3f> face);
    // Determine whether or not we already have added a color (for 2D polygons) (false if the edge exists, else true)
    bool IsGood(sf::Color c);

    // Sets p, q, r, and s from the input string
    void SetPQR(std::string  str);
    // Returns the "deflection angle" for polyhedra
    double GetAngle();

    // Orders the face array from lowest z-position (furthest from the camera) to highest z-position (closest)
    void OrderFaces();

    void MakePolygon();
    // Creates a 2D Tessellation from the existing pqrs values, and readies it to be drawn
    void Tessellate();
    // Creates a 3D Polyhedron from the existing pqrs values, if appropriate. Calls RotateSolid() to ready it to be drawn
    void MakePolyhedron();

    // Rotates point p by theta around the line defined by points p1 and p2
    sf::Vector3f RotatePointAboutLine(sf::Vector3f p, double theta, sf::Vector3f p1, sf::Vector3f p2);
    // Generates a pseudo-random color for a vertex based on the inputted seed
    sf::Color Colorgen(int seed);

public:
    // Default constructor -- does not have a window, so cannot draw
    Diagram();
    // Constructor with a window to draw to, the center point, and possibly a string to determine pqrs
    Diagram(sf::RenderWindow* window, int centerX, int centerY, std::string str = "");

    // General function to create a polytope from the pqrs of the string, or existing pqrs if no string. Calls one
    // of MakePolygon(), Tessellate(), or MakePolyhedron()
    void MakePoly(std::string str = "");
    // Creates a 2D Polygon from the existing pqrs values, and readies it to be drawn

    // Rotates the existing 3D Polyhedron around the vector <xDir, yDir, zDir> from its center, and readies the rotated
    // Polyhedron to be drawn. If autoRotate is true, rotates it around a predefined vector instead.
    void RotateSolid(int xDir, int yDir, int zDir, bool autoRotate, double rotateSpeed = 2500);


    // Toggle between displaying faces and edges
    void ToggleEdges();
    // Toggle between colorful and all-white lines
    void ToggleColors();

    // Draws our stored shape to the window
    void Draw();
};

// Converts a string with a number in it to an integer containing that number; move to better location in the future
int ToInt(std::string str);
// Converts a color given in HSV to its RGB equivalent
sf::Color HSVtoRGB(int hue, double sat, double val);

#endif //H_DIAGRAM
