// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	terrain = nullptr;
	terrainShader = nullptr;
	orthoMesh = nullptr;
	textureShader = nullptr;
	cameraDepthTexture = nullptr;
	depthShader = nullptr;
	cubeMesh = nullptr;
	shadowShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Load textures
	textureMgr->loadTexture("grass", L"res/ramp_grass.png");
	textureMgr->loadTexture("height", L"res/height2.png");
	textureMgr->loadTexture("mud", L"res/mud.jpg");
	textureMgr->loadTexture("compose", L"res/smoke-compose.png");
	textureMgr->loadTexture("burn", L"res/burn.png");
	textureMgr->loadTexture("zepplin", L"res/Zepplin/zepp_col.jpg");

	// Create Mesh object and shader object for terrain
	int terrainScale = 25;
	int terrainPatchSize = 10;
	terrainDimensions = terrainScale * terrainPatchSize;

	terrain = new TessellatedPlane(renderer->getDevice(), renderer->getDeviceContext(), terrainScale, terrainPatchSize);
	terrainShader = new TerrainShader(renderer->getDevice(), hwnd);

	windPos = XMFLOAT3(terrainDimensions, 20.0f, terrainDimensions);

	// Creating mesh and shader objects for post processing
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);
	targetTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	
	
	// Creating mesh and shader objects for depth
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	cameraDepthTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);

	// Vars
	timeTrack = 0.0f;

	// UI Vars
	tessFactor = 1.0f;

	// Test cube
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	testPlane = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture("brick", L"res/brick1.dds");
	camera->setPosition(55.f, 6.0f, 55.f);

	// Zepplin
	zepplin = new Model(renderer->getDevice(), renderer->getDeviceContext(), "res/Zepplin/zepplin_tri.obj");

	// Explosion
	explosionShader = new ExplosionShader(renderer->getDevice(), hwnd);
	explosion = new Explosion(terrainDimensions, renderer->getDevice(), renderer->getDeviceContext(), hwnd, explosionShader, depthShader);
	timeTrack = 0.0f;

	// Depth of Field / Blur
	blurShader = new BlurShader(renderer->getDevice(), hwnd);
	dofShader = new DoFShader(renderer->getDevice(), hwnd);
	horizontalBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	verticalBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	cameraDepth = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	DoFTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	// Spot light
	zepplinPos = XMFLOAT3(25.f, 30.0f, 55.f);

	spotLight = new Light;
	spotLight->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	spotLight->setDiffuseColour(1.0f, 1.0f, 0.0f, 1.0f);
	spotLight->setDirection(0.7f, -0.7f, 0.0f);
	spotLight->setPosition(45.f , 20.0f, 55.f);
	spotLight->generateProjectionMatrix(0.1f, 100.f);
	spotLight->generateViewMatrix();
	
	spotLightDepth = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D objects

	if (terrain) {
		delete terrain;
		terrain = 0;
	}

	if (terrainShader) {
		delete terrainShader;
		terrainShader = 0;
	}

	if (orthoMesh) {
		delete orthoMesh;
		orthoMesh = 0;
	}

	if (textureShader) {
		delete textureShader;
		textureShader = 0;
	}
	
	if (cameraDepthTexture) {
		delete cameraDepthTexture;
		cameraDepthTexture = 0;
	}

	if (depthShader) {
		delete depthShader;
		depthShader = 0;
	}

	if (cubeMesh) {
		delete cubeMesh;
		cubeMesh = 0;
	}

	if (shadowShader) {
		delete shadowShader;
		shadowShader = 0;
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

void App1::depthPass(XMMATRIX viewMatrix, XMMATRIX projectionMatrix, RenderTexture* texture_target)
{
	// GENERATE VIEW MATRIX BEFORE STARTING DEPTH PASS

	// Set the render target to be the render to texture.
	texture_target->setRenderTarget(renderer->getDeviceContext());
	texture_target->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	// get the world, view, and projection matrices from the camera and d3d objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	
	// Render floor
	//terrain->sendData(renderer->getDeviceContext());
	//terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, nullptr, textureMgr->getTexture("height"), textureMgr->getTexture("mud"), explosion, 1, camera->getPosition(), timeTrack, windPos);
	//terrainShader->render(renderer->getDeviceContext(), terrain->getIndexCount());

	//depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	//depthShader->render(renderer->getDeviceContext(), terrain->getIndexCount());

	// Render test plane
	testPlane->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	depthShader->render(renderer->getDeviceContext(), testPlane->getIndexCount());

	// Render test cube
	worldMatrix = XMMatrixTranslation(55.f, 3.0f, 57.f);
	cubeMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// Render Explosion
	explosion->RenderDepth(viewMatrix, projectionMatrix);

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

RenderTexture* App1::FirstPass(RenderTexture* inputTexture)
{
	// Multiple render targets (change output in pixel shader too)
	//ID3D11RenderTargetView* renderViews[2];
	//renderViews[0] = inputTexture->getRenderTargetView();
	//renderViews[1] = cameraDepthTexture->getRenderTargetView();
	//renderer->getDeviceContext()->OMSetRenderTargets(2, renderViews, inputTexture->getDepthStencilView());
	//inputTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);
	//cameraDepthTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 0.0f, 0.0f, 1.0f);
	//lightDepth->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	
	inputTexture->setRenderTarget(renderer->getDeviceContext());
	inputTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);


	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Generate the view matrix based on the camera's position.
	camera->update();
	spotLight->setPosition(zepplinPos.x, zepplinPos.y, zepplinPos.z);
	spotLight->generateViewMatrix();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Send geometry data, set shader parameters, render object with shader
	timeTrack += timer->getTime();

	// Render terrain
	windPos.x += timer->getTime();
	windPos.z += timer->getTime();

	if (windPos.x > terrainDimensions) { windPos.x = 0.0f; }
	if (windPos.z > terrainDimensions) { windPos.z = 0.0f; }

	terrain->sendData(renderer->getDeviceContext());
	terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("grass"), textureMgr->getTexture("height"), textureMgr->getTexture("mud"), explosion, tessFactor, camera->getPosition(), timeTrack, windPos, spotLight, spotLightDepth->getShaderResourceView());
	terrainShader->render(renderer->getDeviceContext(), terrain->getIndexCount());

	// Render test plane
	testPlane->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), explosion, spotLight, spotLightDepth->getShaderResourceView());
	lightShader->render(renderer->getDeviceContext(), testPlane->getIndexCount());

	// Render zepplin
	worldMatrix = XMMatrixTranslation(zepplinPos.x, zepplinPos.y, zepplinPos.z);
	zepplin->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("zepplin"), explosion, spotLight, spotLightDepth->getShaderResourceView());
	lightShader->render(renderer->getDeviceContext(), zepplin->getIndexCount());

	// Render test cube
	worldMatrix = XMMatrixTranslation(55.f, 3.0f, 57.f);

	cubeMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), explosion, spotLight, spotLightDepth->getShaderResourceView());
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// Render explosion
	worldMatrix = XMMatrixTranslation(lightPos.x, lightPos.y, lightPos.z);

	explosion->Update(timer->getTime());
	explosion->Render(viewMatrix, projectionMatrix, textureMgr->getTexture("compose"), textureMgr->getTexture("burn"), timeTrack);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();

	return inputTexture;
}

RenderTexture* App1::HorizontalBlur(RenderTexture * inputTexture)
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenSizeX = (float)horizontalBlurTexture->getTextureWidth();
	horizontalBlurTexture->setRenderTarget(renderer->getDeviceContext());
	horizontalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 0.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	orthoMatrix = inputTexture->getOrthoMatrix();

	// Render for Horizontal Blur
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	blurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, inputTexture->getShaderResourceView(), screenSizeX, XMFLOAT2(1.0f, 0.0f));
	blurShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
	return horizontalBlurTexture;
}

RenderTexture* App1::VerticalBlur(RenderTexture * inputTexture)
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenSizeY = (float)verticalBlurTexture->getTextureWidth();
	verticalBlurTexture->setRenderTarget(renderer->getDeviceContext());
	verticalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 0.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	orthoMatrix = inputTexture->getOrthoMatrix();

	// Render for Horizontal Blur
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	blurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, inputTexture->getShaderResourceView(), screenSizeY, XMFLOAT2(0.0f, 1.0f));
	blurShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
	return verticalBlurTexture;
}

RenderTexture * App1::DoFPass(RenderTexture * inputTexture)
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	DoFTexture->setRenderTarget(renderer->getDeviceContext());
	DoFTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 0.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	orthoMatrix = DoFTexture->getOrthoMatrix();

	// Render for up sample
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	dofShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, targetTexture->getShaderResourceView(), inputTexture->getShaderResourceView(), cameraDepth->getShaderResourceView(), 1.0f, SCREEN_DEPTH, SCREEN_NEAR, 2.0f);
	dofShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();

	return DoFTexture;

}

void App1::FinalPass(RenderTexture* inputTexture)
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// RENDER THE RENDER TEXTURE SCENE
	// Requires 2D rendering and an ortho mesh.
	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	orthoMesh->sendData(renderer->getDeviceContext());
	
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, inputTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	
	renderer->setZBuffer(true);

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();
}

bool App1::render()
{
	// Gather depth info from explosion point light, spot light and camera
	for (int i = 0; i < 6; i++) {
		depthPass(explosion->GenerateView(i), explosion->getLight()->getProjectionMatrix(), explosion->getShadowMap(i));
	}
	depthPass(spotLight->getViewMatrix(), spotLight->getProjectionMatrix(), spotLightDepth);

	depthPass(camera->getViewMatrix(), renderer->getProjectionMatrix(), cameraDepth);

	RenderTexture* currentTexture;
	currentTexture = FirstPass(targetTexture);
	
	currentTexture = HorizontalBlur(currentTexture);
	currentTexture = VerticalBlur(currentTexture);
	currentTexture = DoFPass(currentTexture);

	// Pass in texture to be rendered to screen. Debug texture can be used here
	if (testRender) {
		FinalPass(targetTexture);
	}
	else {
		FinalPass(currentTexture);
	}
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
	ImGui::Text("Camera Position: X-%.2f, Y-%.2f, Z%.2f", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

	ImGui::SliderInt("Tessellation Factor: ", &tessFactor, 1, 64);

	ImGui::SliderFloat("Zepp X: ", &zepplinPos.x, 0.0f, 100.0f);
	ImGui::SliderFloat("Zepp Y: ", &zepplinPos.y, 0.0f, 50.0f);
	ImGui::SliderFloat("Zepp Z: ", &zepplinPos.z, 40.f, 60.0f);

	ImGui::Checkbox("Render Test", &testRender);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

