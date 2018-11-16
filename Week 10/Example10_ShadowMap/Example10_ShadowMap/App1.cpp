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
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphere = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	lightSphere = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	cube = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());

	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, screenWidth / 2.7, screenHeight / 2.7);	// Full screen size
	
	orthoMesh2 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, -screenWidth / 2.7, screenHeight / 2.7);	// Full screen size

	model = new Model(renderer->getDevice(), renderer->getDeviceContext(), "res/teapot.obj");
	textureMgr->loadTexture("brick", L"res/brick1.dds");

	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);

	int shadowmapWidth = 4096;
	int shadowmapHeight = 4096;
	int sceneWidth = 100;
	int sceneHeight = 100;

	// This is your shadow map
	shadowMap = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);
	shadowMap2 = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);

	light = new Light;
	light->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	light->setDirection(1.0f, -1.0f, 0.0f);
	light->setPosition(-10.f, 10.f, 0.f);
	light->generateProjectionMatrix(0.1f, 100.f);
	//light->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	light2 = new Light;
	light2->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light2->setDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	light2->setDirection(0.0f, -0.7f, 0.7f);
	lightPos = XMFLOAT3(0, 10, -15);
	lightDir = XMFLOAT3(0, -1, 1);
	light2->setPosition(lightPos.x, lightPos.y, lightPos.z);
	light2->generateProjectionMatrix(0.5f, 100.f);
	//light2->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	rotationTrack = 0.0f;

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
	rotationTrack += timer->getTime();
	// Perform depth pass for light one and light2
	depthPass(light, shadowMap, 0);
	depthPass(light2, shadowMap2, 1);

	// Render scene
	finalPass();

	return true;
}

void App1::depthPass(Light* light_used, RenderTexture* texture_target, int matrix_type)
{
	// Set the render target to be the render to texture.
	texture_target->setRenderTarget(renderer->getDeviceContext());
	texture_target->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	// get the world, view, and projection matrices from the camera and d3d objects.
	//light_used->generateViewMatrix();
	//XMMATRIX lightViewMatrix = light_used->getViewMatrix();

	XMVECTOR lgtPos = XMLoadFloat3(&light_used->getPosition());
	XMVECTOR lgtDir = XMLoadFloat3(&light_used->getDirection());


	XMFLOAT3 lightRight;
	XMVECTOR lgtRight;

	XMFLOAT3 globalUp;
	XMVECTOR glblUp;

	globalUp.x = 0; globalUp.y = 1; globalUp.z = 0;
	// Set right vector to forward
	lightRight = light_used->getDirection();

	lgtRight = XMLoadFloat3(&lightRight);
	glblUp = XMLoadFloat3(&globalUp);

	// Calculate right (cross between forward and global up)
	lgtRight = XMVector3Cross(lgtRight, glblUp);

	// Calculate up (cross between right & forward)
	XMVECTOR lgtUp = XMVector3Cross(lgtRight, lgtDir);

	//lgtDir = XMVector3Normalize(lgtDir);
	//lgtUp = XMVector3Normalize(lgtUp);

	XMMATRIX testMatrix = XMMatrixLookToLH(lgtPos, lgtDir, lgtUp);

	XMMATRIX lightViewMatrix = testMatrix;

	XMMATRIX lightProjectionMatrix = light_used->getProjectionMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	// Render model
	model->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), model->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(-5.f, 2.f, 0.f);
	scaleMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	// Render Sphere
	sphere->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), sphere->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(5.f, 2.f, 0.f);

	// Rotation animation
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYaw(0, rotationTrack, 0);

	scaleMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, rotateMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	// Render cube
	cube->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), cube->getIndexCount());

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::finalPass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
	camera->update();
	light2->setPosition(lightPos.x, lightPos.y, lightPos.z);
	light2->setDirection(lightDir.x, lightDir.y, lightDir.z);

	// get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, 
		textureMgr->getTexture("brick"), shadowMap->getShaderResourceView(),shadowMap2->getShaderResourceView(), light, light2);
	shadowShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	// Render model
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	model->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), shadowMap->getShaderResourceView(), shadowMap2->getShaderResourceView(), light, light2);
	shadowShader->render(renderer->getDeviceContext(), model->getIndexCount());


	// Render sphere
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(-5.f, 2.f, 0.f);
	scaleMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	sphere->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), shadowMap->getShaderResourceView(), shadowMap2->getShaderResourceView(), light, light2);
	shadowShader->render(renderer->getDeviceContext(), sphere->getIndexCount());

	// Render sphere
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(lightPos.x, lightPos.y, lightPos.z);
	lightSphere->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"));
	textureShader->render(renderer->getDeviceContext(), lightSphere->getIndexCount());

	// Render cube
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(5.f, 2.f, 0.f);

	XMMATRIX rotateMatrix = XMMatrixRotationY(rotationTrack);

	scaleMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, rotateMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);

	cube->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), shadowMap->getShaderResourceView(), shadowMap2->getShaderResourceView(), light, light2);
	shadowShader->render(renderer->getDeviceContext(), cube->getIndexCount());

	// Rendering to ortho mesh
	renderer->setZBuffer(false);
	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	orthoMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMap->getShaderResourceView());
	//textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	orthoMesh2->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, shadowMap2->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh2->getIndexCount());

	renderer->setZBuffer(true);

	gui();
	renderer->endScene();
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
	ImGui::SliderFloat("Light Pos X: ", &lightPos.x, -10, 10);
	ImGui::SliderFloat("Light Pos Y: ", &lightPos.y, 0, 10);
	ImGui::SliderFloat("Light Pos Z: ", &lightPos.z, -15, 0);

	ImGui::SliderFloat("Light Dir X: ", &lightDir.x, -1, 1);
	ImGui::SliderFloat("Light Dir Y: ", &lightDir.y, -1, 1);
	ImGui::SliderFloat("Light Dir Z: ", &lightDir.z, -1, 1);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

