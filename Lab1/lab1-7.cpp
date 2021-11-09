
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
	//-------------------------------------------------------------------------//
	// COPY YOUR CODE FROM 1.6 HERE
	// Update rotation angle here, for example
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
	{
		//implement reloading of the shaders on the fly
		std::string vertex_shader_str = readFile("../lab1-7_vs.glsl");
		std::string fragment_shader_str = readFile("../lab1-7_fs.glsl");
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

int main(int argc, char const *argv[])
{
	// start GL context and O/S window using the GLFW helper library
  
	glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);
  
  
	int w_height = 600;
	int w_width = 800;

	glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);
  
	GLFWwindow* window = glfwCreateWindow (w_width, w_height, "Hello Icosahedron", NULL, NULL);
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

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
  
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// Set up geometry, VBO, EBO, VAO
	float points[] = { -0.5f, -0.5f, -0.5f, 1.0f, //vertex 0
						-0.5f, -0.5f, 0.5f, 1.0f,//vertex 1
						-0.5f, 0.5f, 0.5f, 1.0f,  //vertex 2
						-0.5f, 0.5f, -0.5f, 1.0f, //vertex 3
						0.5f, -0.5f, -0.5f, 1.0f, //vertex 4
						0.5f, -0.5f, 0.5f, 1.0f,  //vertex 5
						0.5f, 0.5f, 0.5f, 1.0f,   //vertex 6
						0.5f, 0.5f, -0.5f, 1.0f,  //vertex 7
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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// load and compile shaders  "../lab1-7_vs.glsl" and "../lab1-7_fs.glsl"
	std::string vertex_shader_str = readFile("../lab1-7_vs.glsl");
	std::string fragment_shader_str = readFile("../lab1-7_fs.glsl");
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
	GLuint shader_program = glCreateProgram ();
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// attach and link vertex and fragment shaders into a shader program
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader_program);
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	glUseProgram (shader_program);
 
	const float n = 1.0f;
	const float f = 100.0f;
  

	while (!glfwWindowShouldClose (window)) 
	{


		glfwGetFramebufferSize(window, &w_width , &w_height );

		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		//
		// Use glm::perspective to create a projection matrix
		//
		// Use glm::translate, glm::rotate and glm::inverse to create the
		// model and view matrices.
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		glm::mat4 projectionMatrix = glm::mat4(1.0f);

		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -2.0f));
		projectionMatrix = glm::perspective(glm::radians(90.0f), float(w_width / w_height), n, f);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(g_rotation[0]), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(g_rotation[1]), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 modelViewProjectionMatrix = projectionMatrix * (viewMatrix * modelMatrix);

		int shade_loc = glGetUniformLocation(shader_program, "position");
		glUniformMatrix4fv(shade_loc, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
		//
		// Multiply your matrices in the correct order to get a
		// modelViewProjection matrix and upload it to the appropriate
		// uniform variable in vertex shader.
		// -----------------------------------------------------------------------//


		// update other events like input handling 
		glfwPollEvents ();
    
		// clear the drawing surface
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		//-----------------------------------------------------------------------//
		// Issue an appropriate glDraw*() command.
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

		//-----------------------------------------------------------------------//

		glfwSwapBuffers (window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

