#include "system.h"

Tmpl8::Renderer* renderer;

struct RenderData
{
	float posx, posy, posz, dummy1;
	float targetx, targety, targetz, dummy2;
	float p0x, p0y, p0z, dummy3;
	float p1x, p1y, p1z, dummy4;
	float p2x, p2y, p2z, dummy5;
};

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
void Tmpl8::Game::Init()
{
	Tmpl8::Scene* myScene = new Scene();
	renderer = new Renderer(myScene, renderSurface);
	Tmpl8::Camera* camera = myScene->camera;
#if OCL_GAME_TMPL == 1
	// load shader and texture
	clOutput = new Tmpl8::Texture(SCRWIDTH, SCRHEIGHT, Tmpl8::Texture::FLOAT);
	shader = new Tmpl8::Shader("shaders/vignette.vert", "shaders/vignette.frag");
	// load OpenCL code
	GPURenderFunction = new Tmpl8::Kernel("programs/program.cl", "WhittedStyleRender");
	/*
	unsigned int *out_data,
	RenderData renderData,
	const int width,
	const int height,
	const int number_of_triangles,
	const int number_of_lights
	*/

	// Create RenderData to send to GPU
	RenderData rd = RenderData();
	rd.posx = camera->position.x;
	rd.posy = camera->position.y;
	rd.posz = camera->position.z;
	rd.targetx = camera->viewDirection.x;
	rd.targety = camera->viewDirection.y;
	rd.targetz = camera->viewDirection.z;
	rd.p0x = camera->p0.x;
	rd.p0y = camera->p0.y;
	rd.p0z = camera->p0.z;
	rd.p1x = camera->p1.x;
	rd.p1y = camera->p1.y;
	rd.p1z = camera->p1.z;
	rd.p2x = camera->p2.x;
	rd.p2y = camera->p2.y;
	rd.p2z = camera->p2.z;

	Buffer* renderDataBuffer = new Buffer(80, 0, &rd); // 5 * 16 bytes

	// Create SceneData to send to GPU
	// Every triangle consist of 5 float4's, 3 vertices, one normal, one color
	int triangleCount = myScene->triangleCount;
	int triangleDataSize = (5 * triangleCount);
	std::vector<vec4> triangleData = std::vector<vec4>();
	triangleData.reserve(triangleDataSize);
	for (uint i = 0; i < triangleCount; i++) {
		Triangle* t = myScene->primitives[i];
		triangleData.push_back(vec4(t->v0, 0));				// 1st vertex
		triangleData.push_back(vec4(t->v1, 0));				// 2nd vertex
		triangleData.push_back(vec4(t->v2, 0));				// 3rd vertex
		triangleData.push_back(vec4(t->normal, 0));			// normal
		triangleData.push_back(vec4(t->material.color, 0));	// color
	}

	Buffer* triangleDataBuffer = new Buffer(80 * triangleCount, 0, &triangleData);

	// Every light is a point light, consists of 2 float 4's, position and color
	int lightCount = myScene->lightCount;
	int lightDataSize = (2 * lightCount);
	std::vector<vec4> lightData = std::vector<vec4>();
	lightData.reserve(lightDataSize);
	
	for (uint i = 0; i < lightCount; i++) {
		Light* l = myScene->lights[i];
		lightData.push_back(vec4(l->position, 0));		// 1st position
		lightData.push_back(vec4(l->position, 0));	// 2nd color		
	}

	Buffer* lightDataBuffer = new Buffer(32 * lightCount, 0, &lightData);

	// link cl output texture as an OpenCL buffer
	outputBuffer = new Tmpl8::Buffer(clOutput->GetID(), Tmpl8::Buffer::TARGET);
	GPURenderFunction->SetArgument(0, outputBuffer);
	GPURenderFunction->SetArgument(1, renderDataBuffer);
	GPURenderFunction->SetArgument(2, SCRWIDTH);
	GPURenderFunction->SetArgument(3, SCRHEIGHT);
	GPURenderFunction->SetArgument(4, myScene->triangleCount);
	GPURenderFunction->SetArgument(5, myScene->lightCount);

	printf("Game::Init -> Finished construction!");
#endif
}

// -----------------------------------------------------------
// Input handling
// -----------------------------------------------------------
#define DEBUG 0

void Tmpl8::Game::HandleInput(float dt) {}
void Tmpl8::Game::MouseUp(int _Button) { /* implement if you want to detect mouse button presses */ }
void Tmpl8::Game::MouseDown(int _Button) { /* implement if you want to detect mouse button presses */ }
void Tmpl8::Game::MouseMove(int _X, int _Y) { /* implement if you want to detect mouse movement */ }
	 

void Tmpl8::Game::KeyUp(int a_Key) {}
void Tmpl8::Game::KeyDown(int a_Key)
{
	Tmpl8::Camera* camera = renderer->scene->camera;
	mat4 transform = camera->transformMatrix;
	mat4 previousTransform = transform;

	float rotationSpeed = 0.05f;
	float movementSpeed = 0.1f;
	// Adjust viewdirection

	//LeftRight
	if (a_Key == 79 || a_Key == 80)
	{
		if (a_Key == 80)
			//transform = rotate(transform, -1 * rotationSpeed, camera->rUp);
			printf("left");
		else
			printf("");
			//transform = rotate(transform, rotationSpeed, camera->rUp);
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;

	}
	//UpDown
	if (a_Key == 82 || a_Key == 81)
	{
		if (a_Key == 82)
			//transform = rotate(transform, -1 * rotationSpeed, camera->rRight);
			printf("");
		else
			//transform = rotate(transform, rotationSpeed, camera->rRight);
			printf("");
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;

	}
	//Left/RightCtrl
	if (a_Key == 224 || a_Key == 228)
	{
		if (a_Key == 224)
			//transform = rotate(transform, -1 * rotationSpeed, camera->viewDirection);
			printf("");
		else
			//transform = rotate(transform, rotationSpeed, camera->viewDirection);
			printf("");
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;

	}
	//a,d
	if (a_Key == 4 || a_Key == 7)
	{
		if (a_Key == 4)
			//transform = translate(transform, -camera->rRight *movementSpeed);
			printf("");

		else
			//transform = translate(transform, camera->rRight *movementSpeed);
			printf("");
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;
	}

	//w,s
	if (a_Key == 26 || a_Key == 22)
	{
		if (a_Key == 26)
			//transform = translate(transform, camera->viewDirection *movementSpeed);
			printf("");

		else
			//transform = translate(transform, -camera->viewDirection * movementSpeed);
			printf("");
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;
	}

	//lShift-rShift
	if (a_Key == 225 || a_Key == 229)
	{
		if (a_Key == 225)
			//transform = translate(transform, camera->rUp *movementSpeed);
			printf("");

		else
			//transform = translate(transform, -camera->rUp  *movementSpeed);
			printf("");
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;
	}

}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Tmpl8::Game::Tick(float dt)
{
#if OCL_GAME_TMPL == 1
	GPURenderFunction->Run(outputBuffer);
	shader->Bind();
	shader->SetInputTexture(GL_TEXTURE0, "color", clOutput);
	shader->SetInputMatrix("view", mat4::identity());
	DrawQuad();
	

#else
	renderer->Render();
	char buffer[500];
	sprintf(
		buffer,
		"FPS: %f \n Resolution : %i x %i ",
		1 / dt,
		SCRWIDTH,
		SCRHEIGHT);
	renderSurface->Print(buffer, 2, 2, 0xffffff);
#endif

}