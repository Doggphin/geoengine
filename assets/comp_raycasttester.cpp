#include "comp_raycasttester.h"
#include "ecs.h"
#include "input.h"
#include "physics.h"
#include "CSCIx229.h"
#include "openglhelpers.h"
#include "gizmodrawer.h"

void RaycastTester::update(float delta) {
    //printf("test1\n");
    if(Input::getKey(KEYCODE_E)) {
        
        struct Vector3 forward(0, 0, -3);
        struct Vector3 target;
        Quaternion::rotate(&entity->transform->rotation, &forward, &target);
        target += entity->transform->position;
        struct RaycastResults out;
        if(testmesh != NULL && Physics::raycast(&entity->transform->position, &target, 2147483647, &out)) {
            testmesh->entity->transform->position = out.point;
            testmesh->mesh_type = MESHTYPE_TEAPOT;   
            
            std::cout << out.collider->entity->name << std::endl;    
        } else {
            testmesh->mesh_type = MESHTYPE_NONE;
        }
    }
    else {
        testmesh->mesh_type = MESHTYPE_NONE;
    }
}

void RaycastTester::lateupdate(float delta) {

}

void RaycastTester::awake() {
    testmesh = NULL;
}