#pragma once

#include <vector>
#include "Vec3.h"

using namespace std;

struct Vertex
{
	int index;
	Vec3<double> pos;

	Vertex(int _index, vec3 _pos)
	{
		index = _index;
		pos = _pos;
	}
};

struct Face
{
	int index;
	vector<Vertex*> v;
	Vec3<double> normal;

	Face(int _index, Vertex* v0, Vertex* v1, Vertex* v2)
	{
		index = _index;
		v.push_back(v0);
		v.push_back(v1);
		v.push_back(v2);

		auto v10 = v0->pos - v1->pos;
		auto v20 = v0->pos - v2->pos;

		normal = v10.cross(v20);
	}
};

class Object
{

public:
	int name;
	int type; //0: 상호작용 X, 1: 위치 변경 O, 2: Color 변경 O, 3: 위치 & Color 변경 O
	Vec3<double> center; //오브젝트 위치 world coord
	Vec3<double> color;
	vector<Vertex*> _vertices;
	vector<Face*> _faces;
	
public:
	Object(const char* fileName, vec3 _center, vec3 _color)
	{
		type = 2;
		color = _color;
		center = _center;
		loadObj(fileName);
	}
	~Object();

	void loadObj(const char* filename)
	{
		FILE* fp;
		char header[256] = { 0 };
		double pos[3];
		int v_index[3];
		int index = 0;

		fopen_s(&fp, filename, "r");
		while (fscanf(fp, "%s %lf %lf %lf", header, &pos[0], &pos[1], &pos[2]) != EOF) {
			if (header[0] == 'v' && header[1] == NULL) {

				_vertices.push_back(new Vertex(index++, vec3(pos[0], pos[1], pos[2])));
			}
		}
		//printf("num. vertices : %d\n", _vertices.size());

		index = 0;
		fseek(fp, 0, SEEK_SET);
		while (fscanf(fp, "%s %d %d %d", header, &v_index[0], &v_index[1], &v_index[2]) != EOF) {
			if (header[0] == 'f' && header[1] == NULL) {
				auto v0 = _vertices[v_index[0] - 1];
				auto v1 = _vertices[v_index[1] - 1];
				auto v2 = _vertices[v_index[2] - 1];
				
				
				_faces.push_back(new Face(index++, v0, v1, v2));
			}
		}

		fclose(fp);
	}

	void drawObj(void)
	{
		glColor3f(color[0], color[1], color[2]); //100
		for (auto f : _faces) {
			glBegin(GL_POLYGON);
			glNormal3f(f->normal.x(), f->normal.y(), f->normal.z());
			for (int j = 0; j < 3; j++) {
				glVertex3f(f->v[j]->pos.x(), f->v[j]->pos.y(), f->v[j]->pos.z());
			}
			glEnd();
		}
		//glutSolidSphere(1, 30, 30);
	}

};