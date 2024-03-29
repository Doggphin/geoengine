#include <iostream>
#include <vector>
#include <fstream>
#include <vector>

#include "gltfloader.h"
#include "json.h"
#include "CSCIx229.h"

using namespace std;

void GLTFLoader::loadMesh(std::string name, GLTFData* output) {	
	std::ifstream jsonFile(name, ios::binary);
	Json::Value json;
	try{
    	jsonFile >> json;
		}catch(const std::exception& e){
			std::cerr << "Json parsing error: " << e.what() << std::endl;
		}
	jsonFile.close();

	size_t binLength = json["buffers"][0]["byteLength"].asUInt();
	std::ifstream binFile = std::ifstream(json["buffers"][0]["uri"].asString(), std::ios::binary | std::ios::ate);
	binFile.seekg(0);
	std::vector<char> bin(binLength);
	binFile.read(bin.data(), binLength);
	binFile.close();

	Json::Value& primitive = json["meshes"][0]["primitives"][0];

	Json::Value& positionAccessor = json["accessors"][primitive["attributes"]["POSITION"].asInt()];
	Json::Value& vertexBufferView = json["bufferViews"][positionAccessor["bufferView"].asInt()];
	float* vertexBuffer = (float*)(bin.data() + vertexBufferView["byteOffset"].asInt());

	Json::Value& texcoordAccessor = json["accessors"][primitive["attributes"]["TEXCOORD_0"].asInt()];
	Json::Value& texBufferView = json["bufferViews"][texcoordAccessor["bufferView"].asInt()];
	float* texcoordsBuffer = (float*)(bin.data() + texBufferView["byteOffset"].asInt());

	Json::Value& indicesAccessor = json["accessors"][primitive["indices"].asInt()];
	Json::Value& indicesBufferView = json["bufferViews"][indicesAccessor["bufferView"].asInt()];
	unsigned short* indicesBuffer = (unsigned short*)(bin.data() + indicesBufferView["byteOffset"].asInt());

	unsigned int indicesCount = 0;
	for(int i=0; i<indicesAccessor["count"].asInt(); i++) {
		output->indices.push_back(indicesBuffer[i]);
		indicesCount++;
	}
	for(int i=0; i<texcoordAccessor["count"].asInt(); i++) {
		output->texcoords.push_back(new Vector2(texcoordsBuffer[i * 2], texcoordsBuffer[i*2 + 1]));
	}
	for(int i=0; i<positionAccessor["count"].asInt(); i++) {
		output->vertices.push_back(new Vector3(vertexBuffer[i * 3], vertexBuffer[i * 3 + 1], vertexBuffer[i * 3 + 2]));
	}
	output->indicesCount = indicesCount;
}

unsigned int GLTFLoader::loadBMPTex(const char* file) {
    unsigned int id = LoadTexBMP(file);
    return id;
}