#ifndef GLTFLOADER_H
#define GLTFLOADER_H

#include <vector>
#include "vector3.h"
#include "vector2.h"
#include <string>
#include "json.h"


struct GLTFData {
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords;
    std::vector<Vector3> vertices;
    std::vector<unsigned int> indices;
    unsigned int indicesCount;
};

class GLTFLoader {
    private:
        static GLTFData* gltfBuffer;
    public:
        static unsigned int loadBMPTex(const char* file);
        static void loadMesh(std::string name, GLTFData* output);

        static GLTFData* getMesh(unsigned int model_id);
        static void drawMesh(std::string name, unsigned int texture_id);
};



#endif