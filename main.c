#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INSTANCE_COUNT 3

typedef struct
{
	float x, y;
} vec2;

typedef struct
{
	float x, y, z;
} vec3;

typedef struct
{
	vec2 pos;
	vec2 size;
	vec3 color;
} InstanceData;

int leftPaddle = 0, rightPaddle = 1, ball = 2;
InstanceData instances[3] = {
	{{0.0f, 350.0f}, {100.0f, 100.0f}, {1.0f, 1.0f, 1.0f}},
	{{700.0f, 350.0f}, {100.0f, 100.0f}, {1.0f, 1.0f, 1.0f}},
	{{375.0f, 275.0f}, {50.0f, 50.0f}, {1.0f, 1.0f, 1.0f}}
};

const int speed = 10;
int moveLeft[2] = {0, 0};
int moveRight[2] = {0, 0};

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	switch(key)
	{
		case GLFW_KEY_A:
			moveLeft[0] = (action == GLFW_PRESS || action == GLFW_REPEAT);
			break;
		case GLFW_KEY_D:
			moveLeft[1] = (action == GLFW_PRESS || action == GLFW_REPEAT);
			break;
		case GLFW_KEY_J:
			moveRight[0] = (action == GLFW_PRESS || action == GLFW_REPEAT);
			break;
		case GLFW_KEY_L:
			moveRight[1] = (action == GLFW_PRESS || action == GLFW_REPEAT);
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(win, GLFW_TRUE);
			break;
	}
}

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec2 aPos;\n"
    "layout(location = 1) in vec2 iPos;\n"
    "layout(location = 2) in vec2 iSize;\n"
    "layout(location = 3) in vec3 iColor;\n"
    "uniform vec2 uRes;\n"
    "out vec3 vColor;\n"
    "void main() {\n"
    "    vec2 worldPos = 2*(iPos/uRes - 0.5) + aPos * (2*iSize/uRes);\n"
    "    gl_Position = vec4(worldPos, 0.0, 1.0);\n"
    "    vColor = iColor;\n"
    "}\n";

const char* fragmentShaderSource = "#version 330 core\n"
	"in vec3 vColor;\n"
	"out vec4 fragColor;\n"
	"void main() {\n"
	"	fragColor = vec4(vColor, 1.0); }\n";

int clamp(int* val, int min, int max)
{
	if(*val < min)
		*val = min;
	else if(*val > max)
		*val = max;
	
}

GLuint compileShader(GLenum type, const char* src)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(shader, 512, NULL, log);
		fprintf(stderr, "Shader compile error: %s\n", log);
	}
	return shader;
}

GLuint createProgram(const char* vs, const char* fs)
{
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vs);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fs);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		char log[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		fprintf(stderr, "Program compile error: %s\n", log);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

void update()
{
	instances[leftPaddle].pos.y += speed * (moveLeft[1] - moveLeft[0]);
	instances[rightPaddle].pos.y += speed * (moveRight[1] - moveRight[0]);
}

int main(void)
{
	
	if(!glfwInit())
	{
		fprintf(stderr, "GLFW failed to initialize\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Pong", NULL, NULL);
	if(!window)
	{
		fprintf(stderr, "failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(1);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD\n");
		return -1;
	}

	int fbw, fbh;
	glfwGetFramebufferSize(window, &fbw, &fbh);
	glViewport(0, 0, fbw, fbh);

	float vertices[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};

	int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	// create VAO ... 
	GLuint VAO, VBO, EBO, instanceVBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// QUAD
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position Attribute (aPos)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// per instance
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(instances), instances, GL_DYNAMIC_DRAW);

	// iPos
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)0);
	glVertexAttribDivisor(1, 1);

	// iSize
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)sizeof(vec2));
	glVertexAttribDivisor(2, 1);
	
	// iColor
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(sizeof(vec2) * 2));
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);

	GLuint shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);
	GLint res = glGetUniformLocation(shaderProgram, "uRes");
	if(res == -1)
	{
		fprintf(stderr, "Could not find 'uRes'\n");
	}

	double currentTime, lastTime = glfwGetTime();
	int nbFrames = 0;
	char title[256];
	while(!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	
		currentTime = glfwGetTime();
		nbFrames++;
		if(currentTime - lastTime >= 1.0)
		{
			snprintf(title, sizeof(title), "FPS: %d", nbFrames);
			glfwSetWindowTitle(window, title);

			nbFrames = 0;
			lastTime += 1.0;
		}
		glUseProgram(shaderProgram);
		glUniform2f(res, 800.0f, 600.0f);
		update();
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(instances), instances, GL_DYNAMIC_DRAW);
		glBindVertexArray(VAO);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, INSTANCE_COUNT);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteBuffers(1, &instanceVBO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
