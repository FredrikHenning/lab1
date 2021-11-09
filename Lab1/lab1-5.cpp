
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
 
	if(isCompiled == GL_FALSE)
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
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
    
	if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
	{
		//-----------------------------------------------------------------------//
		// COPY YOUR PREVIOUS SOLUTION FROM lab1-3.cpp HERE.
		// Reload shaders
		std::string vertex_shader_str = readFile("../lab1-5_vs.glsl");
		std::string fragment_shader_str = readFile("../lab1-5_fs.glsl");
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
		//Done-------------------------------------------------------------------//
	} 
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char const *argv[])
{
	// start GL context and O/S window using the GLFW helper library
  
	glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);
  
	GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Cube", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent (window);
                                  
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit ();

	//-------------------------------------------------------------------------//
	// YOUR CODE GOES HERE.
	// Create geometry and VBO and VAO, and also an EBO.
	float points[] = { -0.5f, -0.5f, -0.5f, //vertex 0
						-0.5f, -0.5f, 0.5f, //vertex 1
						-0.5f, 0.5f, 0.5f,  //vertex 2
						-0.5f, 0.5f, -0.5f, //vertex 3
						0.5f, -0.5f, -0.5f, //vertex 4
						0.5f, -0.5f, 0.5f,  //vertex 5
						0.5f, 0.5f, 0.5f,   //vertex 6
						0.5f, 0.5f, -0.5f,  //vertex 7
						};
	unsigned short faces[] = {
							0, 2, 3, //Triangle 1 face 1(left)
							0, 1, 2, //Triangle 2 face 1(left)
							1, 6, 2, //Triangle 3 face 2(front)
							1, 5, 6, //Triangle 4 face 2(front)
							5, 7, 6, //Triangle 5 face 3(right)
							5, 4, 7, //Triangle 6 face 3(right)
							4, 3, 7, //Triangle 7 face 4(back)
							4, 0, 3, //Triangle 8 face 4(back)
							2, 7, 3, //Triangle 9 face 5(Top)
							2, 6, 7, //Triangle 10 face 5(Top)
							0, 5, 1, //Triangle 11 face 6(Bottom)
							0, 4, 5//Triangle 12 face 6(Bottom)
	};
	GLuint VAO, VBO, EBO;
	// 2. Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// 3. Create VBO & EBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// (The geometry will be new, so you cannot only copy your previous code.)
	//-------------------------------------------------------------------------//

	//-------------------------------------------------------------------------//
	// load and compile shaders  "../lab1-4_vs.glsl" and "../lab1-4_fs.glsl"
	// See corresponding code from the previous tasks.
    std::string vertex_shader_str = readFile("../lab1-5_vs.glsl");
	std::string fragment_shader_str = readFile("../lab1-5_fs.glsl");
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
	//Done---------------------------------------------------------------------//
 
	GLuint shader_program = glCreateProgram ();
	//-------------------------------------------------------------------------//
	// attach and link vertex and fragment shaders into a shader program
	// See corresponding code from the previous tasks.
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader_program);
	//Done---------------------------------------------------------------------//

 
	while (!glfwWindowShouldClose (window)) 
	{

		// update other events like input handling 
		glfwPollEvents ();
    
		// clear the drawing surface
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE.
		// Issue an appropriate glDraw*() command.
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		//Done-------------------------------------------------------------------//
    
		glfwSwapBuffers (window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

