// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	mesh = nullptr;
	mesh_two = nullptr;
	shader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Create Mesh object and shader object
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	mesh_two = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture("brick", L"../res/brick1.dds");
	textureMgr->loadTexture("height", L"../res/height.png");
	shader = new LightShader(renderer->getDevice(), hwnd);
	
	point_light[0] = new Light;
	point_light[0]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	point_light[0]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	point_light[0]->setPosition(50.0f, 100.0f, 50.0f);

	point_light[1] = new Light;
	point_light[1]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	point_light[1]->setDiffuseColour(1.0f, 0.f, 0.0f, 1.0f);
	point_light[1]->setPosition(50.0f, 10.0f, 50.0f);

	direction_light[0] = new Light;
	direction_light[0]->setAmbientColour(0.2f, 0.2f, 0.2f, 0.f);
	direction_light[0]->setDiffuseColour(0.5f, 0.f, 0.f, 1.f);
	direction_light[0]->setDirection(0.f, 0.5f, 0.f);

	direction_light[1] = new Light;
	direction_light[1]->setAmbientColour(0.2f, 0.2f, 0.2f, 0.f);
	direction_light[1]->setDiffuseColour(0.f, 1.f, 0.f, 1.f);
	direction_light[1]->setDirection(0.f, -1.f, 0.f);

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (mesh)
	{
		delete mesh;
		mesh = 0;
	}

	if (mesh_two)
	{
		delete mesh_two;
		mesh_two = 0;
	}

	if (shader)
	{
		delete shader;
		shader = 0;
	}
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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, translation, rotation;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();
	translation = XMMatrixTranslation(100.f, 20.f, 0.f);
	rotation = XMMatrixRotationRollPitchYaw(0.f, 0.f, 3.14);

	// Send geometry data, set shader parameters, render object with shader
	mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), textureMgr->getTexture("height"),point_light, direction_light);
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	worldMatrix = XMMatrixMultiply(rotation, translation);

	mesh_two->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), textureMgr->getTexture("height"), point_light, direction_light);
	shader->render(renderer->getDeviceContext(), mesh_two->getIndexCount());

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
	ImGui::Text("Press E to raise camera \nto see the plane being rendered");

	//setting colour through colour edit box
	ImGui::ColorEdit4("Light Colour", colour);
	point_light[0]->setDiffuseColour(colour[0], colour[1], colour[2], 1.f);

	//setting position with sliders
	ImGui::SliderFloat("Light X:", &pos[0], 0.f, 100.f);
	ImGui::SliderFloat("Light Y:", &pos[1], 0.f, 10.f);
	ImGui::SliderFloat("Light Z:", &pos[2], 0.f, 100.f);

	ImGui::ShowDemoWindow();

	point_light[0]->setPosition((float)pos[0], (float)pos[1], (float)pos[2]);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

