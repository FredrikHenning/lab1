
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

float horiz_move;
float vertic_move;

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
    
	if ((key == GLFW_KEY_R) && action == GLFW_PRESS) {
		//-----------------------------------------------------------------------//
		// COPY YOUR PREVIOUS SOLUTION FROM lab1-3.cpp HERE.
		// Reload shaders
		std::string vertex_shader_str = readFile("../lab1-4_vs.glsl");
		std::string fragment_shader_str = readFile("../lab1-4_fs.glsl");
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

	//-----------------------------------------------------------------------//
	// YOUR CODE GOES HERE
	// Update some parameter for the vertex shader on arrow keys.
	//-----------------------------------------------------------------------//
	if ((key == GLFW_KEY_LEFT) && ( (action == GLFW_PRESS) || (action == GLFW_REPEAT))){
		horiz_move = horiz_move - 0.02f;
	}
	if ((key == GLFW_KEY_RIGHT) && ( (action == GLFW_PRESS) || (action == GLFW_REPEAT))){
		horiz_move = horiz_move + 0.02f;
	}
	if ((key == GLFW_KEY_UP) && ( (action == GLFW_PRESS) || (action == GLFW_REPEAT))){
		vertic_move = vertic_move + 0.02f;
	}
	if ((key == GLFW_KEY_DOWN) && ( (action == GLFW_PRESS) || (action == GLFW_REPEAT))){
		vertic_move = vertic_move - 0.02f;
	}
}

static void scroll_callback(GLFWwindow* window, double scroll_v, double scroll_h)
{
	//-----------------------------------------------------------------------//
	// YOUR CODE GOES HERE
	// Update some parameter for the fragment shader when scrolling.
	//-----------------------------------------------------------------------//
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
  
	GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Triangle", NULL, NULL);
	glfwSetKeyCallback( window, key_callback);
	glfwSetScrollCallback( window, scroll_callback); 
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
	// COPY YOUR SOLUTION FROM lab1-2.cpp HERE.
	// 1. Create geometry
	float points[] = { -0.5f, -0.5f, 0.0f,
						0.0f, 0.5f, 0.0f,
						0.5f, -0.5f, 0.0f };
	GLuint VAO, VBO;
	// 2. Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// 3. Create VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Done---------------------------------------------------------------------//

	//-------------------------------------------------------------------------//
	// load and compile shaders  "../lab1-4_vs.glsl" and "../lab1-4_fs.glsl"
	// See corresponding code from the previous tasks.
	std::string vertex_shader_str = readFile("../lab1-4_vs.glsl");
	std::string fragment_shader_str = readFile("../lab1-4_fs.glsl");
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
		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		// Update uniform variables in your shader_program
		int shade_loc = glGetUniformLocation(shader_program, "position_offset");
		glUniform2f(shade_loc, horiz_move, vertic_move);
		//-----------------------------------------------------------------------//

		// update other events like input handling 
		glfwPollEvents ();
		// clear the drawing surface
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-----------------------------------------------------------------------//
		// COPY YOUR SOLUTION FROM lab1-2.cpp HERE.
		// Issue an appropriate glDraw*() command.
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//Done-------------------------------------------------------------------//

		glfwSwapBuffers (window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

