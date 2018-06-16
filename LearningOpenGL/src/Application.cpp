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


bool firstMouse = true;
float yaw = -90.0f;	
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 120.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

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
	window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Hello World", NULL, NULL);
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	/* Create a vertex and index arrays */
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		unsigned int indices[] = {
			// Vertices bottom left to bottom right (anti-clockwise)
			0, 1, 2,
			0, 3, 2

		};
		
		VertexBuffer VBO(vertices, sizeof(vertices));

		VBLayout VBL;
		VBL.Set<float>(3);
		VBL.Set<float>(2);


		VertexArray VAO;
		VAO.Set(VBO, VBL);
		VAO.Bind();


		IndexBuffer IBO(indices, sizeof(indices) / sizeof(unsigned int));

		/* Generate shader program*/

		glm::mat4 view(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 proj(1.0f);
		proj = glm::perspective(glm::radians(45.0f), SCREENWIDTH / SCREENHEIGHT, 0.1f, 100.0f);

		Shader Shader("res/shaders/Shaders.shader");
		Shader.Bind();
	
		Texture texture("res/textures/pic2.png");
		Texture texture2("res/textures/pic.jpg");
		Shader.SetUniform1i("u_Texture", 0);
		Shader.SetUniform1i("u_Texture2", 1);
		
		texture.Bind();
		texture2.Bind(1);


		Renderer Screen;
		Screen.Blend(true);
		Screen.ZBuffer(true);
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Manage Events */

			Screen.ProcessEvents(window);

			float cameraSpeed = 0.2f; 
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				cameraPos += cameraSpeed * cameraFront;
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				cameraPos -= cameraSpeed * cameraFront;
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			view = glm::translate(view, glm::vec3(0.0f, -21.0f, -3.0f));

			/* Clear Screen*/

			Screen.Clear();

			/* Render here */
			proj = glm::perspective(glm::radians(fov), (float)SCREENWIDTH / (float)SCREENHEIGHT, 0.1f, 10.0f);

			std::vector<glm::vec3> chunk;

			for (int x = (int)glfwGetTime() - 20 ; x < (int)glfwGetTime(); x++) {

				for (int y = 0; y < 20; y++) {
					for (int z = 0; z < 20; z++) {
						chunk.push_back( {x, y, z} );
					}
				}
			}

			
			for (glm::vec3 block : chunk)
			{

				glm::mat4 model(1.0f);
				model = glm::translate(model, block);


				glm::mat4 u_MVP(1.0f);
				u_MVP = proj * view * model;
				Shader.SetUniformMat4f("u_MVP", u_MVP);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
	
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

		}
	}
	glfwTerminate();
	return 0;
}
