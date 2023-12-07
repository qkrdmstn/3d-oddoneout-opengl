#pragma once

#include <vector>
#include "Vec3.h"
#include "bmpfuncs.h"

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

	Face(int _index, Vertex* v0, Vertex* v1, Vertex* v2)
	{
		index = _index;
		v.push_back(v0);
		v.push_back(v1);
		v.push_back(v2);
	}
};

class Object
{

public:
	int name;
	int type; //0: 상호작용 X, 1: 위치 변경 O, 2: Color 변경 O, 3: 위치 & Color 변경 O

public:
	double rot; //오브젝트 회전값
	Vec3<double> pos; //오브젝트 위치
	Vec3<double> color; //오브젝트 색

public:
	vector<Face*> _faces;
	vector<Face*> _Tfaces;
	vector<Face*> _Nfaces;
	vector<Vertex*> _vertices;
	vector<Vertex*> _Tvertices;
	vector<Vertex*> _Nvertices;

public:
	Object* pairObject;

public:

	Object()
	{
	}

	Object(const char* fileName,  vec3 _pos, double _rot, vec3 _color, int _type)
	{
		type = _type;
		pos = _pos;
		rot = _rot;
		color = _color;

		loadObj(fileName);
	}

	Object(const char* fileName, vec3 _pos, double _rot, vec3 _color, int _type, Object* _pairObject)
	{
		type = _type;
		pos = _pos;
		rot = _rot;
		color = _color;
		pairObject = _pairObject;
		loadObj(fileName);
	}

	~Object()
	{
	}

	void loadObj(const char* filename)
	{
		FILE* fp;
		char header[256] = { 0 };
		double pos[3];
		int v_index[3];
		int vt_index[3];
		int vn_index[3];
		int index = 0;
		vec3 _center(0,0,0);
		fopen_s(&fp, filename, "r");
		//Read Vertex
		while (fscanf(fp, "%s %lf %lf %lf", header, &pos[0], &pos[1], &pos[2]) != EOF) {
			if (strcmp(header, "v") == 0) {
				_vertices.push_back(new Vertex(index++, vec3(pos[0], pos[1], pos[2])));
				_center += vec3(pos[0], pos[1], pos[2]);
			}
		}
		//printf("num. vertices : %d\n", _vertices.size());

		//object centralized
		_center /= _vertices.size();
		for (auto v : _vertices)
		{
			v->pos -= _center;
		}

		//Read Normal
		index = 0;
		fseek(fp, 0, SEEK_SET);
		while (fscanf(fp, "%s %lf %lf %lf", header, &pos[0], &pos[1], &pos[2]) != EOF) {
			if (strcmp(header, "vn") == 0) {
				_Nvertices.push_back(new Vertex(index++, vec3(pos[0], pos[1], pos[2])));
			}
		}
		//printf("num. Nvertices : %d\n", _vertices.size());

		//Read UV pos
		index = 0;
		fseek(fp, 0, SEEK_SET);
		while (fscanf(fp, "%s %lf %lf", header, &pos[0], &pos[1]) != EOF) {
			if (strcmp(header, "vt") == 0) {
				_Tvertices.push_back(new Vertex(index++, vec3(pos[0], pos[1], 0)));
			}
		}
		//printf("num. Tvertices : %d\n", _Tvertices.size());

		//Read Face index
		index = 0;
		fseek(fp, 0, SEEK_SET);
		while (fscanf(fp, "%s %d/%d/%d %d/%d/%d %d/%d/%d", header, &v_index[0], &vt_index[0], &vn_index[0], &v_index[1], &vt_index[1], &vn_index[1], &v_index[2], &vt_index[2], &vn_index[2]) != EOF) {
			if (header[0] == 'f' && header[1] == NULL) {
				auto v0 = _vertices[v_index[0] - 1];
				auto v1 = _vertices[v_index[1] - 1];
				auto v2 = _vertices[v_index[2] - 1];

				auto vt0 = _Tvertices[vt_index[0] - 1];
				auto vt1 = _Tvertices[vt_index[1] - 1];
				auto vt2 = _Tvertices[vt_index[2] - 1];

				auto vn0 = _Nvertices[vn_index[0] - 1];
				auto vn1 = _Nvertices[vn_index[1] - 1];
				auto vn2 = _Nvertices[vn_index[2] - 1];

				_faces.push_back(new Face(index, v0, v1, v2));
				_Tfaces.push_back(new Face(index, vt0, vt1, vt2));
				_Nfaces.push_back(new Face(index++, vn0, vn1, vn2));
			}
		}
		//printf("num. faces : %d\n", _faces.size());
		//printf("num. tfaces : %d\n", _Tfaces.size());
		fclose(fp);
	}

	virtual void drawObj(void)
	{
		glColor3f(color[0], color[1], color[2]);
		glDisable(GL_TEXTURE_2D);
		for (int i = 0; i < _faces.size(); i++) {
			glBegin(GL_POLYGON);
			for (int j = 0; j < 3; j++) {
				glNormal3f(_Nfaces[i]->v[j]->pos.x(), _faces[i]->v[j]->pos.y(), _faces[i]->v[j]->pos.z());
				glVertex3f(_faces[i]->v[j]->pos.x(), _faces[i]->v[j]->pos.y(), _faces[i]->v[j]->pos.z());

				//printf("%d/%d ",  _faces[i]->v[j]->index + 1, _Tfaces[i]->v[j]->index + 1);
			}
			//printf("\n");
			glEnd();
		}
		//glutSolidSphere(1, 30, 30);
	}

	bool isCorrect()
	{
		double colorDist = (this->color - pairObject->color).length();
		double geoDist = (this->pos - pairObject->pos).length();
		double angDist = this->rot - pairObject->rot;

		if (geoDist > 1 || colorDist > 1 || angDist > 10) //두 벡터 사이의 거리로 정답 판단
			return false;
		else
			return true;
	}
};