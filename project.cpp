#include "CSCIx229.h"
#include "vector3.h"
#include "vector2.h"
#include "Quaternion.h"
#include "ecs.h"
#include "input.h"
#include "window.h"
#include <math.h>
#include <unordered_map>
#include "physics.h"
#include <stdlib.h>
#include <iostream>
#include "gizmodrawer.h"
#include "drawer.h"
#include "openglhelpers.h"
#include "gltfloader.h"

#include "comp_camera.h"
#include "comp_collider.h"
#include "comp_mesh.h"
#include "comp_cameracontroller.h"
#include "comp_playercontroller.h"
#include "comp_entityrotator.h"
#include "comp_raycasttester.h"
#include "comp_neonsign.h"
#include "comp_sawblade.h"

using namespace std;

double DIM = 2;
int windowLength = 800;
int windowHeight = 800;

float current_dim = 3;

float deltaTime = 0;
float cumulativeTime = 0;
int mouseDeltaX = 0;
int mouseDeltaY = 0;
float current_fov = 90;
int shadingMode = GL_FLAT;

int axes = 0;
int drawColliderGizmos = 1;
int drawAABBGizmos = 1;
unsigned int skyboxid = 0;
unsigned int temptexid = 0;
unsigned int bonestesttexid = 0;

struct GLTFData temp;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_LIGHTING);
	glShadeModel(shadingMode);

	struct Vector3 cameraPosition;
	for(auto i : ECS::hashes[typeid(Camera*).hash_code()]) {
		Camera* cam = static_cast<Camera*>(i.second);
		current_fov = cam->fov;
		Entity* e = cam->entity;

		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		Project(current_fov, Window::asp, current_dim, 0.01, 25);
		cameraPosition = e->transform->position;
		Vector3 positionLook;
		Vector3 forward = Vector3Forward;
		Quaternion::rotate(&cam->entity->transform->rotation, &forward, &positionLook);
		positionLook += &e->transform->position;
		Vector3 up = Vector3Up;
		gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z, positionLook.x, positionLook.y, positionLook.z, up.x, up.y, up.z);
	}

	Drawer::skybox(&cameraPosition, skyboxid);
	
	for(auto i : ECS::hashes[typeid(Mesh*).hash_code()]) {
		Mesh* mesh = static_cast<Mesh*>(i.second);
		glPushMatrix();
		struct TransformFields t;
		i.second->entity->transform->hierarchyTransform(&t);
		OpenGLHelpers::applyTransformFields(&t);
		mesh->draw();
		glPopMatrix();
	}

	glDisable(GL_TEXTURE_2D);

	if(drawColliderGizmos || drawAABBGizmos) {
		for(auto i : ECS::hashes[typeid(Collider*).hash_code()]) {
			Collider* col = static_cast<Collider*>(i.second);
			glPushMatrix();

			if(drawAABBGizmos) {
				glColor3f(1, 0, 0);
				if(col->drawGizmo == 1) {
					GizmoDrawer::drawAABB(col->colliderAABBTransformed);
				}
			}
			if(drawColliderGizmos) {
				glColor3f(0, 1, 0);
				GizmoDrawer::drawCollider(col);
			}

			glPopMatrix();
		}
	}

	glColor3f(1, 1, 1);
	if (axes)
	{
		const double len = 1.5;
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(len, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, len, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, len);
		glEnd();
		//  Label axes
		glRasterPos3d(len, 0.0, 0.0);
		Print("X");
		glRasterPos3d(0.0, len, 0.0);
		Print("Y");
		glRasterPos3d(0.0, 0.0, len);
		Print("Z");
	}

	ErrCheck("display");							//<-- this thing says stack overflow?
	glFlush();
	glutSwapBuffers();
}

void idle() {
	// Do time thingies
	static double t0 = -1.;
	double dt, t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	if (t0 < 0.0)
	  t0 = t;
	dt = t - t0;
	t0 = t;
	deltaTime = dt;
	cumulativeTime += deltaTime;
 
	// Run game loop step
	ECS::runStarts();				// First run start functions on any newly instantiated components
	Physics::updateAABBs();
	ECS::runUpdates(deltaTime);		// Next, run updates on all subscribed components
	Physics::updateAABBs();
	Input::clearMouseDelta();		// Clear mouse delta buffer after updates are run
	glutPostRedisplay();			// Next, draw scene to the screen
	ECS::runLateUpdates(deltaTime);			// Next, run late updates on all subscribed components
	Input::setBufferToCurrent();	// Clear buffered inputs for next frame
}

void initScene() {
	int scene = 1;
	
	// THIS CODE WILL BE CONVERTED INTO A JSON FILE
	// in the meantime enjoy this mess of instantiating stuff
	unsigned int missingtexture = GLTFLoader::loadBMPTex("missing_textures.bmp");

	if(scene == 1) {
		Entity* player = ECS::instantiate();
			player->transform->position = new Vector3(-2, 0.5, 0);
			player->transform->rotation = IdentityQuaternion;
		Camera* player_camera = player->addComponent<Camera>();
			player_camera->fov = 100;
			player_camera->dim = 1;
		Collider* player_collider = player->addComponent<Collider>();
			player_collider->colliderShape = new Sphere(.15);
			player_collider->colliderShapeType = SHAPE_SPHERE;
			player_collider->drawGizmo = 0;
			player_collider->layer = 0;
		player->addComponent<CameraController>();
		player->addComponent<PlayerController>();
		RaycastTester* player_raycasttester = player->addComponent<RaycastTester>();
		player->name = "playersphere";

		Entity* object1 = ECS::instantiate();
			object1->transform->position = new Vector3(1, 0, 0);
			object1->transform->scale = new Vector3(0.1, 0.1, 0.1);
		Collider* object1_collider = object1->addComponent<Collider>();
			object1_collider->colliderShape = new Capsule(17, 2);
			object1_collider->colliderShapeType = SHAPE_CAPSULE;
			static_cast<Capsule*>(object1_collider->colliderShape)->direction = DIRECTION_BACKWARD;
		Mesh* object1_mesh = object1->addComponent<Mesh>();
			GLTFLoader::loadMesh("guitar.gltf", object1_mesh->gltfdata);
			object1_mesh->texture_id = GLTFLoader::loadBMPTex("guitartex.bmp");
			object1_mesh->mesh_type = MESHTYPE_CUSTOM;
		EntityRotator* object1_er = object1->addComponent<EntityRotator>();
			object1_er->timeOffset = 0;
		object1->name = "capsule";

		Entity* object2 = ECS::instantiate();
		Collider* object2_collider = object2->addComponent<Collider>();
			object2_collider->colliderShape = new Sphere(.25);
			object2_collider->colliderShapeType = SHAPE_SPHERE;
		EntityRotator* object2_er = object2->addComponent<EntityRotator>();
			object2_er->timeOffset = 3;
		object2->name = "sphere";

		Entity* object3 = ECS::instantiate();
		Collider* object3_collider = object3->addComponent<Collider>();
			object3_collider->colliderShape = new Cuboid(new Vector3(.4, .4, .4));
			object3_collider->colliderShapeType = SHAPE_CUBE;
		EntityRotator* object3_er = object3->addComponent<EntityRotator>();
			object3_er->timeOffset = 6;
		object3->name = "cuboid";

		Entity* object4 = ECS::instantiate();
		Collider* object4_collider = object4->addComponent<Collider>();
			object4_collider->colliderShape = new AABB(new Vector3(.2, .2, .2), new Vector3(-.2, -.2, -.2));
			object4_collider->colliderShapeType = SHAPE_AABB;
		EntityRotator* object4_er = object4->addComponent<EntityRotator>();
			object4_er->timeOffset = 9;
		object4->name = "aabb";

		Entity* object5 = ECS::instantiate();
		object5->transform->scale = new Vector3(0.1, 0.1, 0.1);
		Mesh* object5_mesh = object5->addComponent<Mesh>();
			object5_mesh->mesh_type = MESHTYPE_TEAPOT;
			object5_mesh->texture_id = missingtexture;
		object5->name = "teapot_testobject";

		Entity* object6 = ECS::instantiate();
		object6->transform->scale = new Vector3(4, .01, 4);
		object6->transform->position = new Vector3(0, -2, 0);
		Collider* object6_collider = object6->addComponent<Collider>();
			object6_collider->colliderShape = new Cuboid(new Vector3(2, 2, 2));
			object6_collider->colliderShapeType = SHAPE_CUBE;
			object6_collider->drawGizmo = false;
		Mesh* object6_mesh = object6->addComponent<Mesh>();
			GLTFLoader::loadMesh("betterwrappedcube.gltf", object6_mesh->gltfdata);
			object6_mesh->texture_id = missingtexture;
			object6_mesh->mesh_type = MESHTYPE_CUSTOM;
		object6->name = "floor";

		Entity* object7 = ECS::instantiate();
		object7->transform->position = new Vector3(3, -1.5, 1.5);
		object7->transform->scale = new Vector3(0.1, 0.1, 0.1);
		Mesh* object7_mesh = object7->addComponent<Mesh>();
			GLTFLoader::loadMesh("neonsign.gltf", object7_mesh->gltfdata);
			object7_mesh->mesh_type = MESHTYPE_CUSTOM;
		NeonSign* object7_neonsign = object7->addComponent<NeonSign>();
			object7_neonsign->offtexture = GLTFLoader::loadBMPTex("neonsignoff.bmp");
			object7_neonsign->ontexture = GLTFLoader::loadBMPTex("neonsignon.bmp");
		Collider* object7_collider = object7->addComponent<Collider>();
			object7_collider->drawGizmo = false;
			object7_collider->colliderShape = new Cuboid(new Vector3(.97785, 5.0452, 14.296));
			object7_collider->colliderShapeType = SHAPE_CUBE;
		object7->name = "neonsign";

					Entity* block1 = ECS::instantiate();
					block1->transform->position = new Vector3(4.5, -2.3, -2.2);
					block1->transform->scale = new Vector3(0.3, 0.3, 0.3);
					Mesh* block1_mesh = block1->addComponent<Mesh>();
						GLTFLoader::loadMesh("betterwrappedcube.gltf", block1_mesh->gltfdata);
						block1_mesh->texture_id = GLTFLoader::loadBMPTex("dirtblock.bmp");
						block1_mesh->mesh_type = MESHTYPE_CUSTOM;
					Collider* block1_collider = block1->addComponent<Collider>();
						block1_collider->drawGizmo = false;
						block1_collider->colliderShape = new Cuboid(new Vector3(2, 2, 2));
						block1_collider->colliderShapeType = SHAPE_CUBE;
					block1->name = "block1";

					Entity* block2 = ECS::instantiate();
					block2->transform->position = new Vector3(6, -2, -1.1);
					block2->transform->scale = new Vector3(0.3, 0.3, 0.3);
					Mesh* block2_mesh = block2->addComponent<Mesh>();
						block2_mesh->gltfdata = block1_mesh->gltfdata;
						block2_mesh->texture_id = block1_mesh->texture_id;
						block2_mesh->mesh_type = MESHTYPE_CUSTOM;
					Collider* block2_collider = block2->addComponent<Collider>();
						block2_collider->drawGizmo = false;
						block2_collider->colliderShape = new Cuboid(new Vector3(2, 2, 2));
						block2_collider->colliderShapeType = SHAPE_CUBE;
					block2->name = "block2";

					Entity* block3 = ECS::instantiate();
					block3->transform->position = new Vector3(8.5, -2, 0);
					block3->transform->scale = new Vector3(0.65, 0.3, 0.3);
					Mesh* block3_mesh = block3->addComponent<Mesh>();
						block3_mesh->gltfdata = block1_mesh->gltfdata;
						block3_mesh->texture_id = block1_mesh->texture_id;
						block3_mesh->mesh_type = MESHTYPE_CUSTOM;
					Collider* block3_collider = block3->addComponent<Collider>();
						block3_collider->drawGizmo = false;
						block3_collider->colliderShape = new Cuboid(new Vector3(2, 2, 2));
						block3_collider->colliderShapeType = SHAPE_CUBE;
					block3->name = "block3";

		Entity* sawblade = ECS::instantiate();
		sawblade->transform->scale = new Vector3(0.2, 0.2, 0.2);
		Mesh* sawblade_mesh = sawblade->addComponent<Mesh>();
			GLTFLoader::loadMesh("killer.gltf", sawblade_mesh->gltfdata);
			sawblade_mesh->texture_id = GLTFLoader::loadBMPTex("Saw.bmp");
			sawblade_mesh->mesh_type = MESHTYPE_CUSTOM;
		Collider* sawblade_collider = sawblade->addComponent<Collider>();
			sawblade_collider->drawGizmo = false;
			sawblade_collider->colliderShape = new Cuboid(new Vector3(1.3, 7.4, 7.4));
			sawblade_collider->colliderShapeType = SHAPE_CUBE;
		Sawblade* sawblade_sawblade = sawblade->addComponent<Sawblade>();
			sawblade_sawblade->centerpoint = new Vector3(4, -1.9, 0);
		sawblade->name = "sawblade of death";

		Entity* sawblade2 = ECS::instantiate();
		sawblade2->transform->scale = new Vector3(0.2, 0.2, 0.2);
		Mesh* sawblade2_mesh = sawblade2->addComponent<Mesh>();
			sawblade2_mesh->gltfdata = sawblade_mesh->gltfdata;
			sawblade2_mesh->texture_id = sawblade_mesh->texture_id;
			sawblade2_mesh->mesh_type = MESHTYPE_CUSTOM;
		Collider* sawblade2_collider = sawblade2->addComponent<Collider>();
			sawblade2_collider->drawGizmo = false;
			sawblade2_collider->colliderShape = new Cuboid(new Vector3(1.3, 7.4, 7.4));
			sawblade2_collider->colliderShapeType = SHAPE_CUBE;
		Sawblade* sawblade2_sawblade = sawblade2->addComponent<Sawblade>();
			sawblade2_sawblade->centerpoint = new Vector3(8, -1.65, 1);
		sawblade2->name = "sawblade of death 2";
		
		Entity* catcube = ECS::instantiate();
		catcube->transform->position = new Vector3(3, -1.5, 3);
		catcube->transform->scale = new Vector3(0.4, 0.4, 0.4);
		Mesh* catcube_mesh = catcube->addComponent<Mesh>();
			GLTFLoader::loadMesh("TESTING_CUBE_BONES.gltf", catcube_mesh->gltfdata);
			catcube_mesh->texture_id = GLTFLoader::loadBMPTex("awful.bmp");
			catcube_mesh->mesh_type = MESHTYPE_CUSTOM;
		Collider* catcube_collider = catcube->addComponent<Collider>();
			catcube_collider->colliderShape = new Cuboid(new Vector3(2,2,2));
			catcube_collider->colliderShapeType = SHAPE_CUBE;
		catcube->name = "the cat cube";

		player_raycasttester->testmesh = object5_mesh;
	}

	skyboxid = GLTFLoader::loadBMPTex("cloudysea.bmp");
	bonestesttexid = GLTFLoader::loadBMPTex("awful.bmp");
	Input::setCursorLocked(1);
	Physics::updateAABBs();

	Input::clearMouseDelta();
	Input::allowInputs = 1;
}


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(windowLength, windowHeight);
	glutCreateWindow("PainEngine");
#ifdef USEGLEW
	//  Initialize GLEW
	if (glewInit() != GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
	glutDisplayFunc(display);
	glutReshapeFunc(Window::reshape);
	glutKeyboardFunc(Input::setKeyDown);
	glutKeyboardUpFunc(Input::setKeyUp);
	glutPassiveMotionFunc(Input::passive);
	glutIdleFunc(idle);
	ErrCheck("init");

	initScene();

	glutMainLoop();
	return 0;
}