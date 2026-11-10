#include <Core/Scenes/ASceneController.h>
#include <Core/Render/Shader.h>
#include <thread>
#include <GLFW/glfw3.h>
#include <Core/Global.h>
#include <Core/Render/Render.h>
#include <Core/Physics/APhysicsEngine.h>
#include <Core/Physics/Physics2D/PhysicsEngine.h>
#include <Core/Physics/Physics3D/PhysicsEngine.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Core/Render/LightData.h>
#include <Util/Logger.h>
#include <Core/Objects/AObject.h>


ASceneController::ASceneController(){}

ASceneController::~ASceneController(){
	delete physicsEngine;
}

void ASceneController::AddObject(AObject* object){
	objects.push_back(object);
}

void ASceneController::Awake(){

	if(scenePhysicsType == PhysicsType::ENABLE_2D){
		physicsEngine = new Physics2D::PhysicsEngine();
	}
	else if(scenePhysicsType == PhysicsType::ENABLE_3D){
		physicsEngine = new Physics3D::PhysicsEngine();
	}

	previous = glfwGetTime();
	
	for (auto element : objects){
		if (!element->isEnabled) continue;
		element->Awake();
	}
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

	// The first frame includes the loading time: don't simulate it.
	if (firstUpdate)
		physicsTimeOffset = 0.0;

	// Cap the steps per frame so a slow frame can't trigger an ever growing
	// amount of physics steps (spiral of death).
	const int MAX_PHYSICS_STEPS_PER_FRAME = 5;
	int physicsSteps = 0;
	while (physicsTimeOffset >= Global::FIXED_DELTA_TIME){
		if (physicsSteps >= MAX_PHYSICS_STEPS_PER_FRAME) {
			physicsTimeOffset = 0.0;
			break;
		}
		FixedUpdate();
		physicsSteps++;
		physicsTimeOffset -= Global::FIXED_DELTA_TIME;
		if (Global::DEBUG)
			physicsTimeOffset = 0;
	}
	
	Update(deltaTime);
	LateUpdate();
	firstUpdate = false;
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
		
		if(element->executeAwake) {
			element->Awake();
			continue;
		}

		if(element->executeStart){
			element->Start();
			continue;
		}

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
