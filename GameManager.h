#pragma once


class GameManager
{
public:
	int curState; //0: start, 1: stage, 2: clear, 3: over 4: hint
	int differenece;

	double limitTime; //stage time
	double timer;

    double HlimitTime; //hint time
    double Htimer; 

    /* texture mapping set variable */
    GLuint g_nCubeTex;
    double g_nSkySize = 100.0f;

public:
	GameManager()
	{
		differenece = 0;
		curState = 0;
		limitTime = 1000; //sec
        HlimitTime = 20;
	}
	~GameManager()
	{

	}

	void GameClear()
	{
		printf("Game Clear!\n");
        curState = 2;
	}

	void GameOver()
	{
		printf("Game Over!\n");
        curState = 3;
	}

    void cubeTexture() {
        glGenTextures(1, &g_nCubeTex);
        int imgWidth, imgHeight, channels;
        uchar* img0 = readImageData("Texture/Right.bmp", &imgWidth, &imgHeight, &channels);
        uchar* img1 = readImageData("Texture/Left.bmp", &imgWidth, &imgHeight, &channels);
        uchar* img2 = readImageData("Texture/Up.bmp", &imgWidth, &imgHeight, &channels);
        uchar* img3 = readImageData("Texture/Down.bmp", &imgWidth, &imgHeight, &channels);
        uchar* img4 = readImageData("Texture/Front.bmp", &imgWidth, &imgHeight, &channels);
        uchar* img5 = readImageData("Texture/Back.bmp", &imgWidth, &imgHeight, &channels);

        glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img0); //오른쪽
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img1); //왼쪽
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img2); //위
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img3); //아래
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img4); //앞
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img5); //뒤

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    }

    void drawSkyBox()
    {
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
        glBegin(GL_QUADS);
        // px
        glTexCoord3d(1.0, -1.0, -1.0);
        glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
        glTexCoord3d(1.0, -1.0, 1.0);
        glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
        glTexCoord3d(1.0, 1.0, 1.0);
        glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
        glTexCoord3d(1.0, 1.0, -1.0);
        glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
        // nx
        glTexCoord3d(-1.0, -1.0, -1.0);
        glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
        glTexCoord3d(-1.0, -1.0, 1.0);
        glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
        glTexCoord3d(-1.0, 1.0, 1.0);
        glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
        glTexCoord3d(-1.0, 1.0, -1.0);
        glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);

        // py
        glTexCoord3d(1.0, 1.0, 1.0);
        glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
        glTexCoord3d(-1.0, 1.0, 1.0);
        glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
        glTexCoord3d(-1.0, 1.0, -1.0);
        glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
        glTexCoord3d(1.0, 1.0, -1.0);
        glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
        // ny
        glTexCoord3d(1.0, -1.0, 1.0);
        glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
        glTexCoord3d(-1.0, -1.0, 1.0);
        glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
        glTexCoord3d(-1.0, -1.0, -1.0);
        glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
        glTexCoord3d(1.0, -1.0, -1.0);
        glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);

        // pz
        glTexCoord3d(1.0, -1.0, 1.0);
        glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
        glTexCoord3d(-1.0, -1.0, 1.0);
        glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
        glTexCoord3d(-1.0, 1.0, 1.0);
        glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
        glTexCoord3d(1.0, 1.0, 1.0);
        glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
        // nz
        glTexCoord3d(1.0, -1.0, -1.0);
        glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
        glTexCoord3d(-1.0, -1.0, -1.0);
        glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
        glTexCoord3d(-1.0, 1.0, -1.0);
        glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
        glTexCoord3d(1.0, 1.0, -1.0);
        glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
        glEnd();

        glDisable(GL_TEXTURE_CUBE_MAP);
        glEnable(GL_LIGHTING);
    }
};