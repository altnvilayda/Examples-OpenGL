#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <Util/Shader.h>
#include <stb/stb_image.h>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* hWindow);



int main() {

	//1- init window context

	glfwInit();	//Pencere için gerekli kaynaklarý ayýrýr,baþlatýr. 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS için eklenir.



	GLFWwindow* window = glfwCreateWindow(640, 480, "Ilayda's Triangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	//2- init GLAD for enabling GL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//3- View Screen
	glViewport(0, 0, 640, 480);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//float vertices[] = {
	// 0.5f,  0.5f, 0.0f,  // top right
	// 0.5f, -0.5f, 0.0f,  // bottom right
	//-0.5f, -0.5f, 0.0f,  // bottom left
	//-0.5f,  0.5f, 0.0f   // top left 
	//};

	unsigned int handleTexture;
	glGenTextures(1, &handleTexture);
	glBindTexture(GL_TEXTURE_2D, handleTexture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("../../textures/wall.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);



	float vertices[] = {
		// positions         // colors			   //TextCoor
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,	 1.0f, 0.0f,									 // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	 0.0f, 0.0f,									// bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.5f, 1.0f										// top 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};


	Util::Shader shaderProgram("../../shaders/texturesExample.vert", "../../shaders/texturesExample.frag");
	

	//Use either VBO or EBO
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	


	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.1f, 0.3f, 0.2f, 1.0f);	//Arka plan renk
		glClear(GL_COLOR_BUFFER_BIT);
		//glUseProgram(shaderProgram);
		glBindTexture(GL_TEXTURE_2D, handleTexture);



		shaderProgram.use();
		shaderProgram.setInt("ourTexture", 0);

		//
		/*float timeValue = glfwGetTime();
		float value = sin(timeValue * 2.0f) / 2.0f + 0.5f;*/
		/*int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		if (value < 0.5) {
			glUniform4f(vertexColorLocation, 0.0f, 2*value, 1-2*value, 1.0f);
		}
		else {
			glUniform4f(vertexColorLocation, 2*value-1, 2-2*value, 0.0f, 1.0f);
		}*/
		//shaderProgram.setFloat("greenValue", value);
		//
		//

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* hWindow) {

	if (glfwGetKey(hWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(hWindow, true);

}