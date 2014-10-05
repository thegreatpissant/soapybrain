#ifndef VBOTORUS_H
#define VBOTORUS_H

#include "Model.h"

class VBOTorus : public Model
{
private:
    int faces, rings, sides;

    void generateVerts(float * , float * ,float *, unsigned int *,
                       float , float);

public:
    VBOTorus(float, float, int, int);
    void render ( EntityState &e) const;

	int getVertexArrayHandle();
};

#endif // VBOTORUS_H
