#pragma once
#include "Common.h" 
#include "Texture.h"
#include "VertexBufferObject.h"
class CRoof
{
public:
	CRoof();
	~CRoof();
	void Create();
	void Render();
	void Release();
private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture;
};