#pragma once
#include "LStd.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Collision.h"
//#include "CameraManager.h"  // 차후 매니저 만들 것

#include "LDebugCamera.h"




class DebugDrawer
{

	std::shared_ptr<Shader> shader;

	std::vector<std::shared_ptr<VertexBuffer>> vertexBufferList;
	std::vector<std::shared_ptr<IndexBuffer>> indexBufferList;

	int pass = 0;
public:
	DebugDrawer();
	~DebugDrawer();

	void DrawLine(TVector3 start, TVector3 end, Color color);
	void DrawBox(Cube box, Color color);
	void DrawRect(std::vector<TVector3>& points, Color color);

	void Update();
	void Render();
};

