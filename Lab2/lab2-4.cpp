// This code only shows how to load an object. Input this into your
// program to also draw it.

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include "readfile.hpp"

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr


//-------------------------------------------------------------------------//
// Store the rotation angles and number of lightw here
float g_rotation[2];
int nr_lights;
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
		std::string vertex_shader_str = readFile("../lab2-4_vs.glsl");
		std::string fragment_shader_str = readFile("../lab2-4_fs.glsl");
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
		g_rotation[1] += -1.8f;
	if ((key == GLFW_KEY_S) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		g_rotation[1] += 1.8f;
	if ((key == GLFW_KEY_A) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		g_rotation[0] += -1.8f;
	if ((key == GLFW_KEY_D) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		g_rotation[0] += 1.8f;
	//-------------------------------------------------------------------------//
	// Update number of lights
	if ((key == GLFW_KEY_1) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		nr_lights = 1;
	if ((key == GLFW_KEY_2) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		nr_lights = 2;
	if ((key == GLFW_KEY_3) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		nr_lights = 3;
	if ((key == GLFW_KEY_4) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
		nr_lights = 4;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main(int argc, char** argv)
{

	std::string inputfile = "../../common/data/bunny.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	nr_lights = 1; //initial value for the number of lights

	std::string err;
	int flags = 0; // see load_flags_t enum for more information.

	// Load the object
	bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), 0, flags);

	// Check for errors
	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}
	else
	{
		std::cout << "Loaded " << inputfile
			<< " ("
			<< shapes.size() << " shapes"
			<< ")"
			<< "\n";
		if (shapes.size() > 0)
		{
			std::cout << shapes[0].mesh.positions.size() << " points "
				<< "(" << shapes[0].mesh.positions.size() * sizeof(float) << " bytes), "
				<< shapes[0].mesh.indices.size() << " indices "
				<< "(" << shapes[0].mesh.indices.size() * sizeof(unsigned int) << " bytes), "
				<< shapes[0].mesh.normals.size() << " normals.\n"
				;
		}
	}

	// Populate your VBO with shapes[0].mesh.positions and shapes[0].mesh.normals
	// ...
	// Populate your EBO with shapes[0].mesh.indices
	// ...
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


	//Creating the VAO, VBO, EBO--------------------------------------------------------------------------------------------------------------//
	GLuint VAO, VBO, VBOn, EBO;
	// 2. Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// 3. Create VBO & EBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.positions.size() * sizeof(float), &(shapes[0].mesh.positions[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glGenBuffers(1, &VBOn);
	glBindBuffer(GL_ARRAY_BUFFER, VBOn);
	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.normals.size() * sizeof(float), &(shapes[0].mesh.normals[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size() * sizeof(float), &(shapes[0].mesh.indices[0]), GL_STATIC_DRAW);

	//Read in the shader files-----------------------------------------------------------------------------------------------------------------------------//
	std::string vertex_shader_str = readFile("../lab2-4_vs.glsl");
	std::string fragment_shader_str = readFile("../lab2-4_fs.glsl");
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

		//-------------------------------------------------------------------------------------------------------------------------------------------//
		// Copying from previous task -> Creating the neccecary matrices and sending them to the shader
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		glm::mat4 projectionMatrix = glm::mat4(1.0f);

		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -2.3f));
		projectionMatrix = glm::perspective(glm::radians(90.0f), (float(w_width) / w_height), n, f);

		// Trying to scale //
		modelMatrix = glm::rotate(modelMatrix, glm::radians(g_rotation[0]), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(g_rotation[1]), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(10, 10, 10));
		//	--	--	--	//

		glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
		int MVM_loc = glGetUniformLocation(shader_program, "modelViewMatrixpos");
		glUniformMatrix4fv(MVM_loc, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

		glm::mat4 modelViewProjectionMatrix = projectionMatrix * (viewMatrix * modelMatrix);
		int shade_loc = glGetUniformLocation(shader_program, "projectionModelViewMatrixpos");
		glUniformMatrix4fv(shade_loc, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

		//Creating One light source-------------------------------------------------------------------------------------------------------------------//
		/**glm::vec3 lightPosition(0.0f, 5.0f, 1.0f);	//Position
		int lightPosition_pos = glGetUniformLocation(shader_program, "light_position");
		glUniform3fv(lightPosition_pos, 1, glm::value_ptr(lightPosition));


		glm::vec3 lightColour(1.0f, 1.0f, 1.0f);	//Colour
		int lightColour_pos = glGetUniformLocation(shader_program, "light_colour");
		glUniform3fv(lightColour_pos, 1, glm::value_ptr(lightColour));
		*/

		//Creating Multiple lights----------------------------------------------------------------------------------------------------------//
		glm::vec3 lightPosition[4] = { {1.0f, 1.0f, 1.0f}, {-3.0f, 0.0f, 0.0f}, {0.0f, 3.0f, 0.0f}, {0.0f, 0.0f, 3.0f} };	//Position
		int lightPosition_pos = glGetUniformLocation(shader_program, "light_position");
		glUniform3fv(lightPosition_pos, 4, glm::value_ptr(*lightPosition));

		glm::vec3 lightColour[4] = { {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };	//Colour
		int lightColour_pos = glGetUniformLocation(shader_program, "light_colour");
		glUniform3fv(lightColour_pos, 4, glm::value_ptr(*lightColour));
		//--------------------------------------------------------------------------------------------------------------------------------//

		glm::uint numOfLightSorce(nr_lights);	//Number of lights
		int light_count_position = glGetUniformLocation(shader_program, "light_count");
		glUniform1i(light_count_position, numOfLightSorce);

		// update other events like input handling 
		glfwPollEvents();

		// clear the drawing surface
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 1.0f, 0.0f);


		//-----------------------------------------------------------------------//
		// Issue an appropriate glDraw*() command.
		glDrawElements(GL_TRIANGLES, 208353, GL_UNSIGNED_INT, 0);

		//-----------------------------------------------------------------------//

		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}



