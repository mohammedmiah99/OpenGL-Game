#pragma once
#include "Common.h" 
#include "Texture.h"
#include "VertexBufferObject.h"
class CCube
{
public:
	CCube();
	~CCube();
	void Create();
	void Render();
	void Release();
private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture, m_texture1;
};