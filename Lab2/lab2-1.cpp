// Replace with your own code, starting from what you had in Lab 1,
// but use the geometry defined below.

#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr


//-------------------------------------------------------------------------//
// You can store the rotation angles here, for example
float g_rotation[2];
//-------------------------------------------------------------------------//

void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::string errorLog;
		errorLog.resize(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		std::cout << "shader compilation failed:" << std::endl;
		std::cout << errorLog << std::endl;
		return;
	}
	else
		std::cout << "shader compilation success." << std::endl;

	return;
}


static void error_callback(int error, const char* description)
{
	std::cerr << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//-------------------------------------------------------------------------//
	// COPY YOUR CODE FROM 1.6 HERE
	// Update rotation angle here, for example
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
	{
		//implement reloading of the shaders on the fly
		std::string vertex_shader_str = readFile("../lab2-1_vs.glsl");
		std::string fragment_shader_str = readFile("../lab2-1_fs.glsl");
		const char* vertex_shader_src = vertex_shader_str.c_str();
		const char* fragment_shader_src = fragment_shader_str.c_str();

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_shader_src, NULL);
		glCompileShader(vs);
		checkShaderCompileError(vs);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_shader_src, NULL);
		glCompileShader(fs);
		checkShaderCompileError(fs);

		GLuint shader_program = glCreateProgram();
		glAttachShader(shader_program, fs);
		glAttachShader(shader_program, vs);
		glLinkProgram(shader_program);
		glDeleteShader(vs);
		glDeleteShader(fs);
		glUseProgram(shader_program);
	}

	//-------------------------------------------------------------------------//
	// YOUR CODE GOES HERE
	// Update rotation angle here, for example'
	if ((key == GLFW_KEY_W) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		g_rotation[1] += -1.2f;
	if ((key == GLFW_KEY_S) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		g_rotation[1] += 1.2f;
	if ((key == GLFW_KEY_A) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		g_rotation[0] += -1.2f;
	if ((key == GLFW_KEY_D) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		g_rotation[0] += 1.2f;
	//-------------------------------------------------------------------------//
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

float points[] = {
	// A cube has 8 vertices, but now we have three copies of each one:
  -0.5, -0.5, -0.5, 1, //0 0
	-0.5, -0.5, -0.5, 1, //0 1
	-0.5, -0.5, -0.5, 1, //0 2
	//
  -0.5, -0.5,  0.5, 1, //1 3
  -0.5, -0.5,  0.5, 1, //1 4
  -0.5, -0.5,  0.5, 1, //1 5
	//
  -0.5,  0.5, -0.5, 1, //2 6
  -0.5,  0.5, -0.5, 1, //2 7
  -0.5,  0.5, -0.5, 1, //2 8
	//
  -0.5,  0.5,  0.5, 1, //3 9
  -0.5,  0.5,  0.5, 1, //3 10
  -0.5,  0.5,  0.5, 1, //3 11
	//
  0.5, -0.5, -0.5, 1, //4 12
  0.5, -0.5, -0.5, 1, //4 13
  0.5, -0.5, -0.5, 1, //4 14
	//
  0.5, -0.5,  0.5, 1, //5 15
  0.5, -0.5,  0.5, 1, //5 16
  0.5, -0.5,  0.5, 1, //5 17
	//
  0.5,  0.5, -0.5, 1, //6 18
  0.5,  0.5, -0.5, 1, //6 19
  0.5,  0.5, -0.5, 1, //6 20
	//
  0.5,  0.5,  0.5, 1, //7 21
  0.5,  0.5,  0.5, 1, //7 22
  0.5,  0.5,  0.5, 1, //7 23
};
  

unsigned short faces[]= {
	// ... and 12 triangular faces, 
	// defined by the following vertex indices:
	0, 9, 6, // 0 3 2
	0, 3, 9, // 0 1 3
	//
	1, 7, 18, // 0 2 6
	1, 18, 12, // 0 6 4
	//
	13, 19, 15, // 4 6 5
	15, 19, 21, // 5 6 7
	//
	16, 22, 10, // 5 7 3
	16, 10, 4, // 5 3 1
	//
	8, 11, 23, // 2 3 7
	8, 23, 20, // 2 7 6
	//
	2, 14, 5, // 0 4 1
	5, 14, 17 // 1 4 5
};

glm::vec4 normals[24];


int main( int argc, char** argv )
{
	// Insert your code from Lab 1
	// start GL context and O/S window using the GLFW helper library

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);


	int w_height = 600;
	int w_width = 800;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Hello Icosahedron", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	//Creating the normal vectors----------------------------------------------------------------------------------------//
	glm::vec4 norm;
	float p1[3];
	float p2[3];
	float p3[3];
	for (int i = 0; i < 36; i = i + 3) {
		
		unsigned short f0 = faces[i] * 4;
		unsigned short f1 = faces[i+1] * 4;
		unsigned short f2 = faces[i+2] * 4;
		
		//The different points
		p1[0] = points[f0]; p1[1] = points[f0 + 1]; p1[2] = points[f0 + 2];
		p2[0] = points[f1]; p2[1] = points[f1 + 1]; p2[2] = points[f1 + 2];
		p3[0] = points[f2]; p3[1] = points[f2 + 1]; p3[2] = points[f2 + 2];

		float a = p2[0] - p1[0];
		float b = p2[1] - p1[1];
		float c = p2[2] - p1[2];
		float a2 = p3[0] - p1[0];
		float b2 = p3[1] - p1[1];
		float c2 = p3[2] - p1[2];

		//adding the cross product of the points to a normal vector holder
		norm = glm::vec4(glm::cross(glm::vec3(a,b,c), glm::vec3(a2,b2,c2)), 0.0);
		
		
		//adding the vector holder to an array of normal vector holder
		normals[faces[i]] = norm;
		normals[faces[i+1]] = norm;
		normals[faces[i+2]] = norm;
	}
	//----------------------------------------------------------------------------------------------------------------------------------------//
	for (int i = 0; i < 24; i++) {
		printf("Norms: %f, %f, %f  \n", float(normals[i].x), float(normals[i].y), float(normals[i].z));
	}
	//Creating the VAO, VBO, EBO--------------------------------------------------------------------------------------------------------------//
	GLuint VAO, VBO, VBOn, EBO;
	// 2. Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// 3. Create VBO & EBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);

	glGenBuffers(1, &VBOn);
	glBindBuffer(GL_ARRAY_BUFFER, VBOn);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), &normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), &faces, GL_STATIC_DRAW);

	//Read in the shader files-----------------------------------------------------------------------------------------------------------------------------//
	std::string vertex_shader_str = readFile("../lab2-1_vs.glsl");
	std::string fragment_shader_str = readFile("../lab2-1_fs.glsl");
	const char* vertex_shader_src = vertex_shader_str.c_str();
	const char* fragment_shader_src = fragment_shader_str.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader_src, NULL);
	glCompileShader(vs);
	checkShaderCompileError(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader_src, NULL);
	glCompileShader(fs);
	checkShaderCompileError(fs);
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	GLuint shader_program = glCreateProgram();
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// attach and link vertex and fragment shaders into a shader program
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader_program);
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

	const float n = 1.0f;
	const float f = 100.0f;

	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &w_width, &w_height);

		//-----------------------------------------------------------------------//
		// Copying from previous task
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		glm::mat4 projectionMatrix = glm::mat4(1.0f);

		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -2.0f));
		projectionMatrix = glm::perspective(glm::radians(90.0f), (float(w_width) / w_height), n, f);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(g_rotation[0]), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(g_rotation[1]), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
		int MVM_loc = glGetUniformLocation(shader_program, "modelViewMatrixpos");
		glUniformMatrix4fv(MVM_loc, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

		glm::mat4 modelViewProjectionMatrix = projectionMatrix * (viewMatrix * modelMatrix);
		int shade_loc = glGetUniformLocation(shader_program, "projectionModelViewMatrixpos");
		glUniformMatrix4fv(shade_loc, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
		// -----------------------------------------------------------------------//


		// update other events like input handling 
		glfwPollEvents();

		// clear the drawing surface
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//-----------------------------------------------------------------------//
		// Issue an appropriate glDraw*() command.
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

		//-----------------------------------------------------------------------//

		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
	return 0;
}
