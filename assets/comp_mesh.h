#ifndef COMP_MESH
#define COMP_MESH

#include "ecs.h"
#include "gltfloader.h"

typedef struct { float x, y; } coord;
typedef struct { int a, b, c; } tri;

typedef enum {
	MESHTYPE_NONE,
	MESHTYPE_CUSTOM,
	MESHTYPE_CUBE,
	MESHTYPE_TEAPOT,
	MESHTYPE_ARWING
} MeshType;



class Mesh : public Component {
	private:
		static void drawTriangle(Vector3 A, Vector3 B, Vector3 C, coord D, coord E, coord F);
		void drawArwing();
	public:

		unsigned int texture_id;
		MeshType mesh_type;
		struct GLTFData* gltfdata;
		struct Vector3 color;
		void draw();
		virtual void awake();

		Mesh(int id, Entity* parent):Component(id, parent) { awake(); };
};

#endif