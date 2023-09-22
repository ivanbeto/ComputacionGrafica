/*
Semestre 2021-2
Práctica : Iluminación
Cambios en el shader, en lugar de enviar la textura en el shader de fragmentos, enviaremos el finalcolor
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"


//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;


Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;
Model automovil;
Model Rampa;
Model Circuito;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
float posXavion;
float posYavion;
float posXauto = 0.0f;
float posZavion = 0.0f;
float posYauto = 0.0f;
float posZauto = 0.0f;
float offset = 0.0f;
float banderaBH;
float angle_auto = 0;
float angle_height = 0;
float angle = 0;
float dirX_luz = 0.0f;
float dirZ_luz = 0.0f;
float animacion;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}




void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};


	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main(){

	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 2.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt.obj");
	automovil = Model();
	automovil.LoadModel("Models/automovil.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");
	/*Rampa = Model();
	Rampa.LoadModel("Models/rampa.obj");*/
	Circuito = Model();
	Circuito.LoadModel("Models/circuito.obj");
	Dado_M = Model();
	Dado_M.LoadModel("Models/dado8caras.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//posición inicial del helicóptero
	glm::vec3 posblackhawk = glm::vec3(-20.0f, 6.0f, -1.0);
	glm::vec3 posAuto = glm::vec3(0.0f, 0.5f, -1.5f);
	glm::vec3 desplazamiento;
	glm::vec3 desplazaAuto;
	

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		2.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	/*
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f, //RGB
		1.0f, 2.0f,  //intensidad
		5.0f, 10.0f, 0.0f, //posicion de la luz
		0.0f, -5.0f, 0.0f, //direccion de la luz
		1.0f, 0.0f, 0.0f, //coeficientes de la ec. diferencial de la luz
		15.0f);			//rango de la luz
	spotLightCount++;
	*/
	//luz del coche
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 6.0f,
		6.0f, 0.16f, 0.5f,
		-5.0f,0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		16.0f);
	spotLightCount++;
	//luz de helicóptero spotlights
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 6.0f,
		posblackhawk.x+mainWindow.getmuevex(), 6.0f, -0.5f,
		0.0f, -6.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);
	
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//luz ligada a la cámara de tipo flash 
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		
		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		//agregar material al plano de piso
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		
		//Desplazamiento del helicoptero
		offset += 0.5f;
		posYavion = sin(0.9*offset * toRadians);

		desplazamiento = glm::vec3(posXavion, posYavion, posZavion);
		//Para el mov al frente del helicoptero
		if (banderaBH == 0) {
			posXavion -= 0.08f;
			//angle = 0.0;
			posZavion += 0.0f;
			if (posXavion <= -250) {
				banderaBH = 0.5;
			}
		}
		if (banderaBH == 0.5) {
			posXavion += 0.0f;
			angle -= 0.2;
			posZavion -= 0.02f;
			if (angle <= -180) {
				banderaBH = 1;
			}
		}
		//Para el caso de mov de atras del helicoptero
		if (banderaBH == 1) {
			posXavion += 0.08f;
			posZavion += 0.0f;
			if (posXavion >= 250) {
				banderaBH = 0.25;
			}
		}
		if (banderaBH == 0.25) {
			posXavion += 0.0f;
			angle -= 0.2;
			posZavion += 0.02f;
			if (angle <= -360) {
				banderaBH = 0;
			}
		}

		//agregar incremento en X con teclado
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-20.0f+mainWindow.getmuevex(), 6.0f+mainWindow.getmueveblackhawky(), -1.0));
		model = glm::translate(model, posblackhawk + desplazamiento);
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, angle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//agregar material al helicóptero
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Blackhawk_M.RenderModel();
		
		//luz del helicoptero
		spotLights[2].SetPos(glm::vec3(posblackhawk.x + posXavion, posblackhawk.y+posYavion, posblackhawk.z+posZavion));
		
		//Desplazamiento del automovil
		desplazaAuto = glm::vec3(posXauto, posYauto, posZauto);
		glm::vec3 dirLight = glm::vec3(sin(angle_auto* toRadians -1.5), 0, cos(angle_auto * toRadians -1.5));
		//Uñtimo caso y se detiene
		if (animacion == 3.5) {
			posXauto = posAuto.x;
			posXauto += 0.0f;
			posYauto = posAuto.y;
			posYauto += 0.0f;
			posZauto = posAuto.z;
			posZauto += 0.0f;
			animacion = mainWindow.initAnimation();
		}
		//Para el caso del frente del auto
			
		if (animacion == 0) {
				posXauto -= 0.09f;
				//angle_auto += 0.0f;
				posZauto += 0.0f;
				spotLights[1].SetFlash(glm::vec3(6.0f + posXauto, 0.16, 0.5f + posZauto), glm::vec3(-1.0f, 0.0f, 0.0f));
				if (posXauto <= -190) {
					animacion = 0.25;

				}
		}
			//Primera curva
			if (animacion == 0.25) {
				posXauto += 0.0f;
				angle_auto -= 0.12;
				posZauto -= 0.075f;
				spotLights[1].SetFlash(glm::vec3(6.0f + posXauto, 0.16, -1.6f + posZauto), dirLight);
				if (angle_auto <= -180) {
					animacion = 0.5; //0.16
				}
			}

			//Para la llegada a la primera rampa del auto
			if (animacion == 0.5) {
				posXauto += 0.08f;
				posYauto += 0.0;
				posZauto += 0.0f;
				spotLights[1].SetFlash(glm::vec3(-4.0f + posXauto, 0.16f, -3.5f + posZauto), glm::vec3(1.0f, 0.0f, 0.0f));
				if (posXauto >= -152) { //posXauto >= -150
					//Caso de inclinacion hacia arriba a la rampa
					posXauto += 0.0;
					posYauto += 0.0151;
					angle_height -= 0.046;
					if (angle_height <= -8.5) {
						animacion = 0.75;
					}
				}
			}
			if (animacion == 0.75) {
				//caso de subida a la rampa
				posXauto += 0.04f;
				posYauto += 0.0037;
				spotLights[1].SetFlash(glm::vec3(-4.0f + posXauto, 0.16f, -3.5f + posZauto), glm::vec3(1.0f, 1.0f, 0.0f));
				if (posXauto >= -108) {
					//caso de enderezamiento del auto
					angle_height += 0.05;
					posXauto += 0.00;
					posYauto -= 0.01;
					if (angle_height >= 0) {
						animacion = 1;
					}
				}
			}
			//recorrido de la parte recta de la primera rampa
			if (animacion == 1) {
				posXauto += 0.095f;
				posYauto += 0.0f;
				posZauto += 0.0f;
				angle_height = 0.0f;
				spotLights[1].SetFlash(glm::vec3(-4.0f + posXauto, 0.16f, -3.5f + posZauto), glm::vec3(1.0f, 0.0f, 0.0f));
				if (posXauto >= 98) {
					animacion = 1.25;
				}
			}
			//caso de inclinacion hacia abajo para la bajada
			if (animacion == 1.25) {
				posXauto += 0.01f;
				posYauto -= 0.0101;
				angle_height += 0.046;
				if (angle_height >= 4.5) {
					animacion = 1.5;
				}
			}
			//caso de la bajada de la primera rampa
			if (animacion == 1.5) {
				posXauto += 0.04f;
				posYauto -= 0.0037;
				spotLights[1].SetFlash(glm::vec3(-4.0f + posXauto, 0.16f, -3.5f + posZauto), glm::vec3(1.0f, 0.0f, 0.0f));
				if (posXauto >= 140) {
					//caso de enderezamiento para la parte recta antes de la segunda curva
					angle_height -= 0.05;
					posXauto += 0.0f;
					posYauto -= 0.01;
					if (angle_height <= 0.95) {
						//recorrido antes de llegar a la segunda curva
						posXauto += 0.08f;
						posYauto = 0.06f;
						angle_height = 0.0f;
						if (posXauto >= 170) {
							animacion = 1.75;
						}
					}
				}
			}
			//Segunda curva
			if (animacion == 1.75) {
				posXauto += 0.0f;
				angle_height = 0.0f;
				posYauto += 0.0f;
				angle_auto -= 0.12;
				posZauto += 0.075f;
				spotLights[1].SetFlash(glm::vec3(6.0f + posXauto, 0.16, -0.5f + posZauto), dirLight);
				if (angle_auto <= -360) {
					animacion = 2;
				}
			}
			//Recorrido antes de la segunda rampa
			if (animacion == 2) {
				posXauto -= 0.09f;
				//angle_auto += 0.0f;
				posZauto += 0.0f;
				spotLights[1].SetFlash(glm::vec3(6.0f + posXauto, 0.16, 0.5f + posZauto), glm::vec3(-1.0f, 0.0f, 0.0f));
				if (posXauto <= 89) {
					animacion = 2.25;
				}
			}
			if (animacion == 2.25) {
				//caso de inclinacion para subida
				posXauto -= 0.0f;
				posYauto += 0.0095;
				angle_height -= 0.05;
				if (angle_height <= -3.2) {
					//caso de subida de la segunda rampa
					posXauto -= 0.04f;
					posYauto += 0.01;
					spotLights[1].SetFlash(glm::vec3(-4.0f + posXauto, 0.16f, -3.5f + posZauto), glm::vec3(-1.0f, 1.0f, 0.0f));
					if (posXauto <= 75.5) {
						animacion = 2.5;
					}
				}
			}
			//caso de enderezamiento de la segunda rampa
			if (animacion == 2.5) {
				angle_height += 0.05;
				posXauto -= 0.01;
				posYauto -= 0.01;
				if (angle_height >= 0) {
					animacion = 2.75;
				}
			}
			//caso de recorrido de la segunda rampa
			if (animacion == 2.75) {
				posXauto -= 0.095f;
				posYauto += 0.0f;
				posZauto += 0.0f;
				angle_height = 0.0f;
				spotLights[1].SetFlash(glm::vec3(-4.0f + posXauto, 0.16f, -3.5f + posZauto), glm::vec3(-1.0f, 0.0f, 0.0f));
				if (posXauto <= 21) {
					animacion = 3;
				}
			}
			//caso de inclinacion de la bajada de la rampa
			if (animacion == 3) {
				posXauto -= 0.01f;
				posYauto -= 0.01031;
				angle_height += 0.046;
				if (angle_height >= 16) {
					animacion = 3.25;
				}
			}
			//Caso de la bajada de la segunda rampa
			if (animacion == 3.25) {
				
				posXauto -= 0.1f;
				posYauto -= 0.0017;
				spotLights[1].SetFlash(glm::vec3(6.0f + posXauto, 0.16f, -3.5f + posZauto), glm::vec3(-1.0f, 0.0f, 0.0f));
				if (posXauto >= 9) {
					//caso de enderezamiento para la parte recta antes de la segunda curva
					angle_height -= 0.3;
					posXauto += 0.01f;
					posYauto += 0.01;
					spotLights[1].SetFlash(glm::vec3(6.0f + posXauto, 0.16, 0.5f + posZauto), glm::vec3(-1.0f, 0.0f, 0.0f));
					if (angle_height <= 0) {
					
						posXauto -= 0.08f;
						posZauto += 0.0f;
						posYauto = 0.6f;
						angle_height = 0.0;
						spotLights[1].SetFlash(glm::vec3(6.0f + posXauto, 0.16, 0.5f + posZauto), glm::vec3(-1.0f, 0.0f, 0.0f));
						if (posXauto >= 8) {
							animacion = 3.5;
						}
					}
				}
			}
	
		
		//automovil
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f+mainWindow.getmueveauto(), 0.5f, -1.5f+mainWindow.getmueveautoz()));
		model = glm::translate(model, posAuto + desplazaAuto);
		model = glm::rotate(model, angle_auto * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //angulo de la curva
		model = glm::rotate(model, angle_height* toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //angulo de la altura
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Kitt_M.RenderModel();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		automovil.RenderModel();

		//luz del automovil
		//spotLights[1].SetPos(glm::vec3(6.0f+posXauto, 0.16, 0.5f+mainWindow.getmueveautoz()));
		
		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.0f, -0.4f, 5.9f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, )
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(13.5f, -0.4f, 5.9f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.0f, -0.4f, -0.7f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(13.5f, -0.4f, -0.7f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 15.0f, 0.0f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_M.RenderModel();

		//Agave
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -20.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Tagave.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		//glDisable(GL_BLEND);

		//Modelo del circuito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -305.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Circuito.RenderModel();
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
