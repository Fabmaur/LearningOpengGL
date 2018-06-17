#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLDebug.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VBLayout.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Texture.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"



glm::vec3 lightPos(1.0f, 1.0f, 1.2f);

int main()
{

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */

	static constexpr float SCREENWIDTH = 960.0f;
	static constexpr float SCREENHEIGHT = 540.0f;
	window = glfwCreateWindow((int)SCREENWIDTH, (int)SCREENHEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Glew Init Error!" << std::endl;

	/* Create a vertex and index arrays */
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		
		VertexBuffer VBO(vertices, sizeof(vertices));

		VBLayout VBL;
		VBL.Set<float>(3.0f);
		


		VertexArray VAO;
		VAO.Set(VBO, VBL);
		VAO.Bind();



		/* Generate shader program*/
		glm::vec3 lightColour(0.33f, 0.42f, 0.18f);
		glm::vec3 toyColour(1.0f, 0.5f, 0.31f);
		glm::vec3 result = lightColour * toyColour; // = (0.33f, 0.21f, 0.06f);
		Shader lightShader("res/shaders/LightSrc.shader");
		Shader cubeShader("res/shaders/Scene.shader");
		
		glm::mat4 view(1.0f); //Initalizing view matrix as identity
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::lookAt(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 proj(1.0f);
		proj = glm::perspective(glm::radians(45.0f), SCREENWIDTH / SCREENHEIGHT, 0.1f, 100.0f);

		glm::mat4 identity(1.0f);

		Renderer Screen;
		Screen.Blend(false);
		Screen.ZBuffer(false);
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Manage Events */

			Screen.ProcessEvents(window);

			/* Clear Screen*/

			Screen.Clear();

			/* Render here */
			
			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.2f));
			
			glm::mat4 u_MVP = proj * view * model;

			cubeShader.Bind();
			cubeShader.SetUniform4f("colour", 0.33f, 0.21f, 0.06f, 1.0f);
			cubeShader.SetUniformMat4f("u_MVP", u_MVP);
	
			glDrawArrays(GL_TRIANGLES, 0, 36);
			


			model = identity;
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));
			u_MVP = proj * view * model;

			

			lightShader.SetUniform4f("lightColour", 1.0f, 1.0f, 1.0f, 1.0f);
			lightShader.SetUniformMat4f("u_MVP", u_MVP);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);

			lightShader.Unbind();

			
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

		}
	}
	glfwTerminate();
	return 0;
}
