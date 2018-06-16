#include "Renderer.h"
#include "GLDebug.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

void Renderer::Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& s)
{
	s.Bind();
	VAO.Bind();
	IBO.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
void Renderer::Blend(const bool setting)
{
	if (setting) {
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}
	else
		GLCall(glDisable(GL_BLEND));


}

void Renderer::ZBuffer(const bool setting)
{
	if (setting)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

}
void Renderer::ProcessEvents(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}


void Renderer::Clear()
{
	GLCall(glClearColor(0.0f, 0.9f, 0.9f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}


