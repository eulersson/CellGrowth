#include <iostream>
#include <fstream>
#include <QVector3D>

// For reading the shader files
std::string readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

// Initializes an array (well, a vector) of QVector3D objects with positions
// arranged around a sphere. It's like a parametric way to build the points
// of a sphere.
void createSpherePoints(
    std::vector<QVector3D>& vertices,
    float radius,
    unsigned int rings,
    unsigned int sectors)
{
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);

    for(unsigned int r = 0; r < rings; ++r) {
        for(unsigned int s = 0; s < sectors; ++s) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            vertices.push_back(QVector3D(x,y,z) * radius);
        }
    }
}
