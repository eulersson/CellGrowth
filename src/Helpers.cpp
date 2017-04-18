////////////////////////////////////////////////////////////////////////////////
/// @file Helpers.cpp
/// @author Ramon Blanquer
/// @author Fanny Marstrom
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Standard
#include <vector>
#include <cmath>
// Qt
#include <QOpenGLFunctions>

// Recursion subdivision algorithm from:
// http://www.opengl.org.ru/docs/pg/0208.html

void pushTriangle(float *v1, float *v2, float *v3, std::vector<GLfloat>& _data)
{
  _data.push_back(v1[0]); _data.push_back(v1[1]); _data.push_back(v1[2]); // v1
  _data.push_back(v2[0]); _data.push_back(v2[1]); _data.push_back(v2[2]); // v2
  _data.push_back(v3[0]); _data.push_back(v3[1]); _data.push_back(v3[2]); // v3
}

void subdivide(float *v1, float *v2, float *v3, long depth,std::vector<GLfloat>& _data)
{
  GLfloat v12[3], v23[3], v31[3]; // midpoints

  if (depth == 0) {
   pushTriangle(v1, v2, v3, _data);
   return;
  }

  for (size_t i = 0; i < 3; i++) {
    v12[i] = v1[i] + v2[i];
    v23[i] = v2[i] + v3[i];
    v31[i] = v3[i] + v1[i];
  }

  float v12_len = std::sqrt(v12[0]*v12[0] + v12[1] * v12[1] + v12[2] * v12[2]);
  float v23_len = std::sqrt(v23[0]*v23[0] + v23[1] * v23[1] + v23[2] * v23[2]);
  float v31_len = std::sqrt(v31[0]*v31[0] + v31[1] * v31[1] + v31[2] * v31[2]);

  // Normalize (push to match sphere sphape)
  for (size_t i = 0; i < 3; i++)
  {
    v12[i] /= v12_len;
    v23[i] /= v23_len;
    v31[i] /= v31_len;
  }

  subdivide(v1, v12, v31,  depth-1, _data);
  subdivide(v2, v23, v12,  depth-1, _data);
  subdivide(v3, v31, v23,  depth-1, _data);
  subdivide(v12, v23, v31, depth-1, _data);
}

float lerp(float a, float b, float f)
{
  return a + f * (b -a);
}
