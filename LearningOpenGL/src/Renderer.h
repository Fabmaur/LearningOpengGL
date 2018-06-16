#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

struct GLFWwindow;

class Renderer
{
public:
	void Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& s);
	void Blend(const bool setting);
	void ZBuffer(const bool setting);
	void ProcessEvents(GLFWwindow* window);
	void Clear();
};