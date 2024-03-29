#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <vector>
#include <string>


struct GLTFData {
    std::vector<float> vertices;
    std::vector<float> indices;
    std::vector<float> normals;
    std::vector<float> texcoords;
};

class ModelLoader {
    GLTFData* loadMesh(std::string name)
};



#endif