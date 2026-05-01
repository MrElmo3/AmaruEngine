#include <Core/Scenes/ASceneController.h>
#include <Core/Render/Shader.h>
#include <thread>
#include <GLFW/glfw3.h>
#include <Core/Global.h>
#include <Core/Render/Render.h>
#include <Core/Physics/APhysicsEngine.h>
#include <Core/Physics/PhysicsEngine2D.h>
#include <Core/Physics/PhysicsEngine3D.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Core/Render/LightData.h>
#include <Util/Logger.h>
#include <Core/Objects/AObject.h>


ASceneController::ASceneController(){}

ASceneController::~ASceneController(){}

void ASceneController::AddObject(AObject* object){
	objects.push_back(object);
}

void ASceneController::Awake(){

	if(scenePhysicsType == PhysicsType::ENABLE_2D){
		physicsEngine = new PhysicsEngine2D();
	}
	else if(scenePhysicsType == PhysicsType::ENABLE_3D){
		physicsEngine = new PhysicsEngine3D();
	}

	previous = glfwGetTime();
	
	for (auto element : objects){
		if (!element->isEnabled) continue;
		element->Awake();
	}
	if(physicsEngine != nullptr) physicsEngine->Awake(objects);
}

void ASceneController::Start(){
	for (auto element : objects){
		if (!element->isEnabled) continue;
		element->Start();
	}
}

void ASceneController::SceneUpdate(){
	current = glfwGetTime();
	double deltaTime = current - previous;
	previous = current;
	physicsTimeOffset += deltaTime;

	// double fps = 1.0 / deltaTime;
	// Logger::Log("FPS: " + std::to_string(fps));
	Global::TIME += deltaTime;
	while (physicsTimeOffset >= Global::FIXED_DELTA_TIME){
		FixedUpdate();
		physicsTimeOffset -= Global::FIXED_DELTA_TIME;
		if (Global::DEBUG)
			physicsTimeOffset = 0;
	}

	Update(deltaTime);
	LateUpdate();
}

void ASceneController::FixedUpdate() {
	for (auto element : objects) {
		if (!element->isEnabled) continue;
		element->FixedUpdate();
	}
	if(physicsEngine != nullptr) physicsEngine->UpdatePhysics();
}

void ASceneController::Update(double deltaTime){
	for (auto element : objects){
		if (!element->isEnabled) continue;
		element->Update(deltaTime);
	}
}

void ASceneController::LateUpdate(){
	Render::GetInstance().SetCameraValues();
	Render::GetInstance().SetLightValues();
	for (auto element : objects){
		if (!element->isEnabled) continue;
		element->LateUpdate();
	}
}

void ASceneController::End(){
	for (auto element : objects){
		element->End();
	}
}

std::vector<LightData> ASceneController::GetSceneLightsData() {
	std::vector<LightData> lightData(8, LightData{});

	if(directionalLight != nullptr)
		lightData[0] = directionalLight->GetLightData();

	int uboIndex = 1;
	for (auto light : activeLights) {
		if(uboIndex >= 8) break;
		lightData[uboIndex] = light->GetLightData();
		uboIndex++;
	}

	return lightData;
}
