#pragma once
#include "Object.h"
#include "gl\glut.h"
#include "GL/freeglut.h"

class TexObject :public Object
{

public:
	GLuint texName;

	TexObject(const char* fileName, vec3 _pos, double _rot, vec3 _color, int _type, const char* texFile)
	:Object(fileName, _pos, _rot,  _color,  _type)
	{
		texInit(texFile);
	}

	TexObject(const char* fileName, vec3 _pos, double _rot, vec3 _color, int _type, Object* _pairObject,const char* texFile)
		:Object(fileName, _pos, _rot, _color, _type, _pairObject)
	{
		texInit(texFile);
	}

	void texInit(const char* texFile)
	{
		glGenTextures(1, &texName);
		int imgWidth, imgHeight, channels;
		glBindTexture(GL_TEXTURE_2D, texName);

		uchar* img = readImageData(texFile, &imgWidth, &imgHeight, &channels);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	void drawObj(void)
	{
		//texture
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	//polygon의 원래 색상은 무시하고 texture로 덮음
		glEnable(GL_TEXTURE_2D);

		glColor3f(color[0], color[1], color[2]);
		if (_Tvertices.size() != 0)
			glBindTexture(GL_TEXTURE_2D, this->texName);

		for (int i = 0; i < _faces.size(); i++) {
			glBegin(GL_POLYGON);
			
			for (int j = 0; j < 3; j++) {
				glNormal3f(_Nfaces[i]->v[j]->pos.x(), _faces[i]->v[j]->pos.y(), _faces[i]->v[j]->pos.z());
				if (_Tvertices.size() != 0)
					glTexCoord2d(_Tfaces[i]->v[j]->pos.x(), _Tfaces[i]->v[j]->pos.y());
				glVertex3f(_faces[i]->v[j]->pos.x(), _faces[i]->v[j]->pos.y(), _faces[i]->v[j]->pos.z());

				//printf("%d/%d ",  _faces[i]->v[j]->index + 1, _Tfaces[i]->v[j]->index + 1);
			}
			//printf("\n");
			glEnd();
		}
		//glutSolidSphere(1, 30, 30);
	}
};