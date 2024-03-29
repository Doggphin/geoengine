#ifndef DRAWER_H
#define DRAWER_H

#include "CSCIx229.h"
#include "vector3.h"
#include <unordered_set>
#include "gltfloader.h"

class Drawer {
    public:
        static unsigned int loadBMPTex(const char* file);
        static void drawMesh(GLTFData* gltf, unsigned int texture_id);
        static void skybox(Vector3* cameraPosition, unsigned int texture_id);
};

#endif