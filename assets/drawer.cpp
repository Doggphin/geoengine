
#include "drawer.h"
#include <iostream>

float skyboxdistance = 5;

void Drawer::drawMesh(GLTFData* gltf, unsigned int texture_id) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < gltf->indicesCount; ++i)
	{
        glTexCoord2f(gltf->texcoords[gltf->indices[i]].x, 1 - gltf->texcoords[gltf->indices[i]].y);
        glVertex3f(gltf->vertices[gltf->indices[i]].x, gltf->vertices[gltf->indices[i]].y, gltf->vertices[gltf->indices[i]].z);
	}
    glEnd();
}

void Drawer::skybox(Vector3* cameraPosition, unsigned int texture_id) {
    glPushMatrix();

    glTranslatef(cameraPosition->x, cameraPosition->y, cameraPosition->z);
    glScalef(skyboxdistance, skyboxdistance, skyboxdistance);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glBegin(GL_QUADS);

    float pos = 5.001;
    float neg = -5.001;
    double third = .33333334;
    double twothirds = .66666667;
    //-z
    glTexCoord2d(0.00,third); glVertex3f(neg,neg,neg);
    glTexCoord2d(.25,third); glVertex3f(pos,neg,neg);
    glTexCoord2d(.25,twothirds); glVertex3f(pos,pos,neg);
    glTexCoord2d(0.00,twothirds); glVertex3f(neg,pos,neg);  
    //+x
    glTexCoord2d(.25,third); glVertex3f(pos,neg,neg);
    glTexCoord2d(.5,third); glVertex3f(pos,neg,pos);
    glTexCoord2d(.5,twothirds); glVertex3f(pos,pos,pos);
    glTexCoord2d(0.25,twothirds); glVertex3f(pos,pos,neg);
    //+z
    glTexCoord2d(.5,third); glVertex3f(pos,neg,pos);
    glTexCoord2d(.75,third); glVertex3f(neg,neg,pos);
    glTexCoord2d(.75,twothirds); glVertex3f(neg,pos,pos);
    glTexCoord2d(0.5,twothirds); glVertex3f(pos,pos,pos);
    //-x
    glTexCoord2d(.75,third); glVertex3f(neg,neg, pos);
    glTexCoord2d(1,third); glVertex3f(neg,neg,neg);
    glTexCoord2d(1,twothirds); glVertex3f(neg,pos,neg);
    glTexCoord2d(0.75,twothirds); glVertex3f(neg,pos,pos);
    //+y
    glTexCoord2d(.25,twothirds); glVertex3f(pos,pos,neg);
    glTexCoord2d(.5,twothirds); glVertex3f(pos,pos,pos);
    glTexCoord2d(.5,1); glVertex3f(neg,pos,pos);
    glTexCoord2d(0.25,1); glVertex3f(neg,pos,neg);
    //-y
    glTexCoord2d(.25,0); glVertex3f(neg,neg,neg); 
    glTexCoord2d(.5,0); glVertex3f(neg,neg,pos); 
    glTexCoord2d(.5,third); glVertex3f(pos,neg,pos); 
    glTexCoord2d(0.25,third); glVertex3f(pos,neg,neg); 
    

    glEnd();

    glPopMatrix();
}