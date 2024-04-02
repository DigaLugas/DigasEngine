// GraphicEngine.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "olcConsoleGameEngine.h"


using namespace std;

struct Vec3D
{
	float x, y, z;
};

struct Triangle
{
	Vec3D p[3];
};

struct mesh
{
	vector<Triangle> tri;
};
struct MatProjection
{
	float m[4][4] = { 0 };
};

class Digas3DEngine : public olcConsoleGameEngine {
private:
	float fTheta;
	mesh meshCubo;
	MatProjection matProj;
	void MultiplyMatrixToVector(Vec3D& i, Vec3D& o, MatProjection& m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}
public:
	int DigasEngine3D() {
		m_sAppName = L"Digas3D";

	}
public:
	bool OnUserCreate() override
	{
		meshCubo.tri = {

			// sul
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// oeste                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// norte                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// lest                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// top                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// bot                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },







		};
		//Matriz de Projeção
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)ScreenHeight() / ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override {

		Fill(0, 0, ScreenWidth(), ScreenWidth(), PIXEL_SOLID, FG_BLACK);


		MatProjection matRotZ, matRotX;
		fTheta += 1.0f * fElapsedTime;


		// Rotação Y
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotação X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;


		//Desenha
		for (auto tri : meshCubo.tri)
		{
			Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			// Rotação no Eixo Z
			MultiplyMatrixToVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			MultiplyMatrixToVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			MultiplyMatrixToVector(tri.p[2], triRotatedZ.p[2], matRotZ);

			// Rotação no Eixo X
			MultiplyMatrixToVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MultiplyMatrixToVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MultiplyMatrixToVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

			// Vizualização na tela
			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

			// Projeção 3D para 2D
			MultiplyMatrixToVector(triTranslated.p[0], triProjected.p[0], matProj);
			MultiplyMatrixToVector(triTranslated.p[1], triProjected.p[1], matProj);
			MultiplyMatrixToVector(triTranslated.p[2], triProjected.p[2], matProj);

			// Escala
			triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
			triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
			triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
			triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
			triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
			triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
			triProjected.p[2].y *= 0.5f * (float)ScreenHeight();

			// Desenha
			DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y,
				PIXEL_SOLID, FG_WHITE);

		}

		return true;
	}


};


int main()
{
	Digas3DEngine demo;
	if (demo.ConstructConsole(256, 240, 4, 4))
		demo.Start();

}

