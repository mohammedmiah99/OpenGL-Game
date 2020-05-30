#include "NewShape.h"
CNewShape::CNewShape()
{}
CNewShape::~CNewShape()
{
	Release();
}
//Create the octahedron
void CNewShape::Create(string filename)
{
	//load texture
	m_texture.Load(filename);
	//Set Sampler Object Parameters
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	//create VBO and bind
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create();
	m_vbo.Bind();

	// Write the code to add interleaved vertex attributes to the VBO 
	//front
	glm::vec3 v0 = glm::vec3(-1, -0.5, 1);
	glm::vec3 v1 = glm::vec3(1, -0.5, 1);
	glm::vec3 v2 = glm::vec3(0, 0.5, 0);
	glm::vec3 v3 = glm::vec3(-1, -0.5, 1);
	glm::vec3 v4 = glm::vec3(0, -1.5, 0);
	glm::vec3 v5 = glm::vec3(1, -0.5, 1);
	glm::vec3 n = glm::vec3(0, 0, 1);
	glm::vec2 t0 = glm::vec2(0, 0);
	glm::vec2 t1 = glm::vec2(1, 0);
	glm::vec2 t2 = glm::vec2(0.5, 1);
	glm::vec2 t3 = glm::vec2(0, 0);
	glm::vec2 t5 = glm::vec2(1, 0);
	glm::vec2 t4 = glm::vec2(0.5, -1);

	//back
	glm::vec3 v11 = glm::vec3(-1, -0.5, -1);
	glm::vec3 v00 = glm::vec3(1, -0.5, -1);
	glm::vec3 v22 = glm::vec3(0, 0.5, 0);
	glm::vec3 v55 = glm::vec3(-1, -0.5, -1);
	glm::vec3 v44 = glm::vec3(0, -1.5, 0);
	glm::vec3 v33 = glm::vec3(1, -0.5, -1);
	glm::vec3 nn = glm::vec3(0, 0, -1);
	glm::vec2 t00 = glm::vec2(0, 0);
	glm::vec2 t11 = glm::vec2(1, 0);
	glm::vec2 t22 = glm::vec2(0.5, 1);
	glm::vec2 t33 = glm::vec2(0, 0);
	glm::vec2 t55 = glm::vec2(1, 0);
	glm::vec2 t44 = glm::vec2(0.5, -1);

	//right
	glm::vec3 v000 = glm::vec3(1, -0.5, 1);
	glm::vec3 v111 = glm::vec3(1, -0.5, -1);
	glm::vec3 v222 = glm::vec3(0, 0.5, 0);
	glm::vec3 v333 = glm::vec3(1, -0.5, 1);
	glm::vec3 v444 = glm::vec3(0, -1.5, 0);
	glm::vec3 v555 = glm::vec3(1, -0.5, -1);
	glm::vec3 nnn = glm::vec3(1, 0, 0);
	glm::vec2 t000 = glm::vec2(0, 0);
	glm::vec2 t111 = glm::vec2(1, 0);
	glm::vec2 t222 = glm::vec2(0.5, 1);
	glm::vec2 t333 = glm::vec2(0, 0);
	glm::vec2 t555 = glm::vec2(1, 0);
	glm::vec2 t444 = glm::vec2(0.5, -1);

	//left
	glm::vec3 v0000 = glm::vec3(-1, -0.5, -1);
	glm::vec3 v1111 = glm::vec3(-1, -0.5, 1);
	glm::vec3 v2222 = glm::vec3(0, 0.5, 0);
	glm::vec3 v3333 = glm::vec3(-1, -0.5, -1);
	glm::vec3 v4444 = glm::vec3(0, -1.5, 0);
	glm::vec3 v5555 = glm::vec3(-1, -0.5, 1);
	glm::vec3 nnnn = glm::vec3(-1, 0, 0);
	glm::vec2 t0000 = glm::vec2(0, 0);
	glm::vec2 t1111 = glm::vec2(1, 0);
	glm::vec2 t2222 = glm::vec2(0.5, 1);
	glm::vec2 t3333 = glm::vec2(0, 0);
	glm::vec2 t5555 = glm::vec2(1, 0);
	glm::vec2 t4444 = glm::vec2(0.5, -1);

	//add data to VBO of vertices, normals, textures
	m_vbo.AddData(&v0, sizeof(glm::vec3));
	m_vbo.AddData(&t0, sizeof(glm::vec2));
	m_vbo.AddData(&n, sizeof(glm::vec3));
	m_vbo.AddData(&v1, sizeof(glm::vec3));
	m_vbo.AddData(&t1, sizeof(glm::vec2));
	m_vbo.AddData(&n, sizeof(glm::vec3));
	m_vbo.AddData(&v2, sizeof(glm::vec3));
	m_vbo.AddData(&t2, sizeof(glm::vec2));
	m_vbo.AddData(&n, sizeof(glm::vec3));
	m_vbo.AddData(&v3, sizeof(glm::vec3));
	m_vbo.AddData(&t3, sizeof(glm::vec2));
	m_vbo.AddData(&n, sizeof(glm::vec3));
	m_vbo.AddData(&v4, sizeof(glm::vec3));
	m_vbo.AddData(&t4, sizeof(glm::vec2));
	m_vbo.AddData(&n, sizeof(glm::vec3));
	m_vbo.AddData(&v5, sizeof(glm::vec3));
	m_vbo.AddData(&t5, sizeof(glm::vec2));
	m_vbo.AddData(&n, sizeof(glm::vec3));

	m_vbo.AddData(&v00, sizeof(glm::vec3));
	m_vbo.AddData(&t00, sizeof(glm::vec2));
	m_vbo.AddData(&nn, sizeof(glm::vec3));
	m_vbo.AddData(&v11, sizeof(glm::vec3));
	m_vbo.AddData(&t11, sizeof(glm::vec2));
	m_vbo.AddData(&nn, sizeof(glm::vec3));
	m_vbo.AddData(&v22, sizeof(glm::vec3));
	m_vbo.AddData(&t22, sizeof(glm::vec2));
	m_vbo.AddData(&nn, sizeof(glm::vec3));
	m_vbo.AddData(&v33, sizeof(glm::vec3));
	m_vbo.AddData(&t33, sizeof(glm::vec2));
	m_vbo.AddData(&nn, sizeof(glm::vec3));
	m_vbo.AddData(&v44, sizeof(glm::vec3));
	m_vbo.AddData(&t44, sizeof(glm::vec2));
	m_vbo.AddData(&nn, sizeof(glm::vec3));
	m_vbo.AddData(&v55, sizeof(glm::vec3));
	m_vbo.AddData(&t55, sizeof(glm::vec2));
	m_vbo.AddData(&nn, sizeof(glm::vec3));

	m_vbo.AddData(&v000, sizeof(glm::vec3));
	m_vbo.AddData(&t000, sizeof(glm::vec2));
	m_vbo.AddData(&nnn, sizeof(glm::vec3));
	m_vbo.AddData(&v111, sizeof(glm::vec3));
	m_vbo.AddData(&t111, sizeof(glm::vec2));
	m_vbo.AddData(&nnn, sizeof(glm::vec3));
	m_vbo.AddData(&v222, sizeof(glm::vec3));
	m_vbo.AddData(&t222, sizeof(glm::vec2));
	m_vbo.AddData(&nnn, sizeof(glm::vec3));
	m_vbo.AddData(&v333, sizeof(glm::vec3));
	m_vbo.AddData(&t333, sizeof(glm::vec2));
	m_vbo.AddData(&nnn, sizeof(glm::vec3));
	m_vbo.AddData(&v444, sizeof(glm::vec3));
	m_vbo.AddData(&t444, sizeof(glm::vec2));
	m_vbo.AddData(&nnn, sizeof(glm::vec3));
	m_vbo.AddData(&v555, sizeof(glm::vec3));
	m_vbo.AddData(&t555, sizeof(glm::vec2));
	m_vbo.AddData(&nnn, sizeof(glm::vec3));

	m_vbo.AddData(&v0000, sizeof(glm::vec3));
	m_vbo.AddData(&t0000, sizeof(glm::vec2));
	m_vbo.AddData(&nnnn, sizeof(glm::vec3));
	m_vbo.AddData(&v1111, sizeof(glm::vec3));
	m_vbo.AddData(&t1111, sizeof(glm::vec2));
	m_vbo.AddData(&nnnn, sizeof(glm::vec3));
	m_vbo.AddData(&v2222, sizeof(glm::vec3));
	m_vbo.AddData(&t2222, sizeof(glm::vec2));
	m_vbo.AddData(&nnnn, sizeof(glm::vec3));
	m_vbo.AddData(&v3333, sizeof(glm::vec3));
	m_vbo.AddData(&t3333, sizeof(glm::vec2));
	m_vbo.AddData(&nnnn, sizeof(glm::vec3));
	m_vbo.AddData(&v4444, sizeof(glm::vec3));
	m_vbo.AddData(&t4444, sizeof(glm::vec2));
	m_vbo.AddData(&nnnn, sizeof(glm::vec3));
	m_vbo.AddData(&v5555, sizeof(glm::vec3));
	m_vbo.AddData(&t5555, sizeof(glm::vec2));
	m_vbo.AddData(&nnnn, sizeof(glm::vec3));
	

	// Upload data to GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}
void CNewShape::Render()
{
	//Bind to VAO
	glBindVertexArray(m_vao);
	m_texture.Bind();
	// Call glDrawArrays to render each side
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 3, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 6, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 9, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 15, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 18, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 21, 3);

}
void CNewShape::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}