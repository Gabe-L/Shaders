// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	mesh = nullptr;
	nice_mesh = nullptr;
	textureShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	textureMgr->loadTexture("brick", L"../res/brick1.dds");
	textureMgr->loadTexture("nice_bricks", L"../res/nice_bricks.png");

	// Create Mesh object and shader object
	mesh = new TexturedQuad(renderer->getDevice(), renderer->getDeviceContext());
	nice_mesh = new TexturedQuad(renderer->getDevice(), renderer->getDeviceContext());
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	rotateTrack = 0.f;
	rotateTrackNice = 0.f;

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

	if (nice_mesh) {
		delete nice_mesh;
		nice_mesh = 0;
	}

	if (textureShader)
	{
		delete textureShader;
		textureShader = 0;
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
	
	//rotateTrack += timer->getTime();
	
	if (input->isKeyDown(VK_NUMPAD4)) {
		rotateTrackNice += timer->getTime();
	}
	else if (input->isKeyDown(VK_NUMPAD6)) {
		rotateTrackNice -= timer->getTime();
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
	
	worldMatrix = XMMatrixRotationRollPitchYaw(0.f, 0.f, rotateTrack);

	// Send geometry data, set shader parameters, render object with shader
	mesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"));
	textureShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	worldMatrix = XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0.f, 0.f, rotateTrackNice), XMMatrixTranslation(2.f, 0.f, 0.f));

	nice_mesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("nice_bricks"));
	textureShader->render(renderer->getDeviceContext(), nice_mesh->getIndexCount());

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
	ImGui::SliderAngle("Rotation Slider", &rotateTrack);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

