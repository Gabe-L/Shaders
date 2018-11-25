// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Create Mesh object and shader object
	//mesh = new TessellatedPlane(renderer->getDevice(), renderer->getDeviceContext(), 1);
	
	mesh = new TessellatedPlane(renderer->getDevice(), renderer->getDeviceContext(), 10, 10);
	plane = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());

	textureMgr->loadTexture("brick", L"res/brick1.dds");
	textureMgr->loadTexture("grass", L"res/ramp_grass.png");
	textureMgr->loadTexture("height", L"res/cratertest.png");

	shader = new TessellationShader(renderer->getDevice(), hwnd);
	grassShader = new GrassShader(renderer->getDevice(), hwnd);

	tessFactor = 1;
	wave_info = XMFLOAT4(0.f, 1.f, 1.f, 1.f);
	lightPos = XMFLOAT3(0, 2, 0);

	testLight = new Light;
	testLight->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	testLight->setDiffuseColour(0.6f, 0.6f, 0.2f, 1.0f);
	testLight->setPosition(lightPos.x, lightPos.y, lightPos.z);

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	wave_info.x += timer->getTime();

	// Send geometry data, set shader parameters, render object with shader
	mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), textureMgr->getTexture("grass"), textureMgr->getTexture("height"), tessFactor, wave_info, camera->getPosition(), testLight);
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	//plane->sendData(renderer->getDeviceContext());
	//grassShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), testLight, camera->getPosition(), wave_info.x);
	//grassShader->render(renderer->getDeviceContext(), plane->getIndexCount());

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	
	ImGui::SliderFloat("Height: ", &wave_info.y, 0.f, 1.f);
	ImGui::SliderFloat("  Freq: ", &wave_info.z, 0.5f, 10.f);
	ImGui::SliderFloat(" Speed: ", &wave_info.w, 0.5f, 3.f);

	ImGui::SliderInt("Tessellation Factor: ", &tessFactor, 1, 64);

	ImGui::SliderFloat("Light Pos X: ", &lightPos.x, -50.0f, 50.0f);
	ImGui::SliderFloat("Light Pos Y: ", &lightPos.y, -5.0f, 5.0f);
	ImGui::SliderFloat("Light Pos Z: ", &lightPos.z, -50.0f, 50.0f);

	testLight->setPosition(lightPos.x, lightPos.y, lightPos.z);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

