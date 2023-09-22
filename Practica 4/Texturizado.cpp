/*
Semestre 2021-2
Práctica Texturizado
Usando librería stb_image.h
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>

//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
//#include "Sphere.h"
#include"Window.h"
#include "Camera.h"
#include"Texture.h"

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
float codo = 0.0f;

Texture T_ladrillos;
Texture T_asfalto;
Texture T_dadoa;
Texture T_dadoocho;
Camera camera;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader_t.vert";
static const char* fShader = "shaders/shader_t.frag";
//Sphere sp = Sphere(1, 20, 20);

void CreateObject(){
	unsigned int indices[] = {
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.5f,	0.0f,	0.0f,
		0.0f,-0.5f,0.5f,	1.0f,	0.0f,
		0.5f,-0.5f, 0.0f,	1.0f,	1.0f,
		0.0f,0.5f,0.0f,		0.0f,	1.0f
	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}
//ejercicio 1 para hacer en clase, el cubo
void CrearCubo(){
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4,6,7,
		7,5,4,
		// left
		8,10,11,
		11,9,8,
		// top
		12,13,14,
		14,15,12,
		// back
		16,17,18,
		18,19,16,
		// bottom
		22,23,21,
		21,20,22,
	};

	GLfloat cubo_vertices[] = {
		// front (6)
		//x		y		z		u/s		v/t
		-0.5f, -0.5f,  0.5f,	0.74f,	0.01f,//0
		0.5f, -0.5f,  0.5f,		0.51f,	0.01f,//1
		0.5f,  0.5f,  0.5f,		0.51f,	0.32f,//2
		-0.5f,  0.5f,  0.5f,	0.74f,	0.32f,//3
		//right (3)
		//x		y		z		u/s		v/t
		0.5f, -0.5f,  0.5f,		0.51f,	0.34f,//4
		0.5f,  0.5f,  0.5f,		0.74f,	0.34f,//5
		0.5f, -0.5f,  -0.5f,	0.51f,	0.65f,//6
		0.5f,  0.5f,  -0.5f,	0.74f,	0.65f,//7
		//left (4)
		-0.5f, -0.5f,  0.5f,	0.76f,	0.33f,//8
		-0.5f,  0.5f,  0.5f,	0.99f,	0.33f,//9
		-0.5f, -0.5f,  -0.5f,	0.76f,	0.66f,//10
		-0.5f,  0.5f,  -0.5f,	0.99f,	0.66,//11
		//top (2)
		-0.5f,  0.5f,  0.5f,	0.26f,	0.34f,//12
		0.5f,  0.5f,  0.5f,		0.49f,	0.34f,//13
		0.5f,  0.5f,  -0.5f,	0.49f,	0.65f,//14
		-0.5f,  0.5f,  -0.5f,	0.26f,	0.65f,//15
		// back (1)
		-0.5f, -0.5f, -0.5f,	0.24f,	0.34f,//16
		0.5f, -0.5f, -0.5f,		0.01f,	0.34f,//17
		0.5f,  0.5f, -0.5f,		0.01f,	0.65f,//18
		-0.5f,  0.5f, -0.5f,	0.24f,	0.65f,//19
		//bottom (5)
		-0.5f, -0.5f,  0.5f,	0.51f,	0.66f,//20
		0.5f, -0.5f,  0.5f,		0.74f,	0.66f,//21
		-0.5f, -0.5f,  -0.5f,	0.51f,	0.99f,//22
		0.5f, -0.5f,  -0.5f,	0.74f,	0.99f,//23

	};
	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 120, 36);
	meshList.push_back(cubo);
}

//ejercicio 2. crear dado de 8 caras
void CrearDado() {
	unsigned int indices_dado[] = {
		//1era cara
		0,1,2,
		//2da cara
		3,4,5,
		//3ra cara
		6,7,8,
		//4ta cara
		9,10,11,
		//5ta cara
		12,13,14,
		//6ta cara
		15,16,17,
		//7ma cara
		18,20,19,
		//8va cara
		22,21,23,
	};

	GLfloat dado_vertices[] = {
		//1era cara
		//x		y		z		u/s		v/t
		-0.5f, 0.0f,  0.5f,		0.76f,	0.01f,//0
		0.5f, 0.0f,  0.5f,		0.99f,	0.01f,//1
		0.0f,  0.7f,  0.0f,		0.875f,	0.32f,//2
		
		//2da cara
		//x		y		z		u/s		v/t
		0.5f,  0.0f,  0.5f,		0.01f,	0.34f,//3
		0.5f, 0.0f,  -0.5f,		0.24f,	0.34f,//4
		0.0f,  0.7f,  0.0f,		0.125f,	0.64f,//5
		
		//3ra cara
		//x		y		z		u/s		v/t
		0.5f,  0.0f,  -0.5f,	0.51f,	0.34f,//6
		-0.5f, 0.0f,  -0.5f,	0.74f,	0.34f,//7
		0.0f, 0.7f, 0.0f,		0.625f,	0.65f,//8
		
		//4ta cara
		//x		y		z		u/s		v/t
		-0.5f,  0.0f, -0.5f,	0.26f,	0.34f,//9
		-0.5f, 0.0f, 0.5f,		0.49f,	0.34f,//10
		0.0f, 0.7f, 0.0f,		0.375f,	0.65f,//11

		//5ta cara
		//x		y		z		u/s		v/t
		-0.5f, 0.0f,  0.5f,		0.51f,	0.99f,//12
		0.5f, 0.0f,  0.5f,		0.74f,	0.99f,//13
		0.0f,  -0.7f,  0.0f,	0.625f,	0.66f,//14

		//6ta cara
		//x		y		z		u/s		v/t
		0.5f,  0.0f,  0.5f,		0.26f,	0.32f,//15
		0.5f, 0.0f,  -0.5f,		0.49f,	0.32f,//16
		0.0f,  -0.7f,  0.0f,	0.375f,	0.01f,//17

		//7ma cara
		//x		y		z		u/s		v/t
		0.5f,  0.0f,  -0.5f,	0.76f,	0.65f,//18
		-0.5f, 0.0f,  -0.5f,	0.99f,	0.65f,//19
		0.0f, -0.7f, 0.0f,		0.875f,	0.34f,//20

		//8va cara
		//x		y		z		u/s		v/t
		-0.5f,  0.0f, -0.5f,	0.51f,	0.32f,//21
		-0.5f, 0.0f, 0.5f,		0.74f,	0.32f,//22
		0.0f, -0.7f, 0.0f,		0.625f,	0.01f,//23
	};
	Mesh *dado = new Mesh();
	dado->CreateMesh(dado_vertices, indices_dado, 120, 24);
	meshList.push_back(dado);
};

void CreateShaders(){
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main(){
	mainWindow = Window(900, 700);
	mainWindow.Initialise();
	CreateObject();
	CrearCubo();
	CrearDado();
	CreateShaders();
	camera = Camera(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 3.0f, 0.3f);
	//para crear la textura
	T_ladrillos = Texture("Textures/ladrillos.tga");
	T_ladrillos.LoadTexture();
	T_asfalto = Texture("Textures/dirt.png");
	T_asfalto.LoadTexture();
	T_dadoa = Texture("Textures/dado.tga");
	T_dadoa.LoadTexture();
	T_dadoocho = Texture("Textures/dado8caras.tga");
	T_dadoocho.LoadTexture();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose()){
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		//ejercicio 1:
		glm::mat4 model(1.0);

		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		T_ladrillos.UseTexture();
		meshList[0]->RenderMesh();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 1.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//T_asfalto.UseTexture();
		T_dadoa.UseTexture();
		//T_dadoocho.UseTexture();
		meshList[1]->RenderMesh();
		/*
		//ejercicio 2:
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 1.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		T_dadoocho.UseTexture();
		meshList[2]->RenderMesh();
		*/
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}