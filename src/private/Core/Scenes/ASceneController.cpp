#include <Core/Scenes/ASceneController.h>
#include <Core/Render/Shader.h>
#include <thread>
#include <GLFW/glfw3.h>
#include <Core/Global.h>
#include <Core/Physics/PhysicsEngine2D.h>
#include <Core/Physics/PhysicsEngine3D.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Util/Logger.h>


ASceneController::ASceneController(){
}

ASceneController::~ASceneController(){
}

void ASceneController::AddObject(AObject* object){
	objects.push_back(object);
}

void ASceneController::Awake(){
	previous = glfwGetTime();
	
	for (auto element : objects){
		if (!element->isEnabled) continue;
		element->Awake();
	}
	PhysicsEngine3D::Awake(objects);
	PhysicsEngine2D::Awake(objects);
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
	PhysicsEngine3D::Update();
	PhysicsEngine2D::Update();
}

void ASceneController::Update(double deltaTime){
	for (auto element : objects){
		if (!element->isEnabled) continue;
		element->Update(deltaTime);
	}
}

void ASceneController::LateUpdate(){
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

void ASceneController::UseLights(Shader* shader) {
	//directional light
	shader->SetUInt("_directionalLight.type", (unsigned int)directionalLight->GetType());
	shader->SetVector3("_directionalLight.direction", directionalLight->GetDirection());
	shader->SetVector3("_directionalLight.color", directionalLight->GetColor());
	shader->SetVector3("_directionalLight.ambient", directionalLight->GetAmbient());
	shader->SetVector3("_directionalLight.diffuse", directionalLight->GetDiffuse());
	shader->SetVector3("_directionalLight.specular", directionalLight->GetSpecular());
}
