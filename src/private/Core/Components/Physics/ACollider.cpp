#include <Core/Components/Physics/ACollider.h>
#include <Core/Render/Render.h>
#include <Core/Render/Color.h>
#include <Util/Logger.h>

ACollider::ACollider() {
	debugColor = Color::GREEN;
}

ACollider::~ACollider() {}

glm::vec3 ACollider::GetSupportPoint(glm::vec3 direction){ 
	Logger::Warning("GetSupportPoint its not well defined or direction is 0");
	return glm::vec3(NAN);
}

void ACollider::UpdateWorldValues() {
	UpdateWorldVertexPoints();
	UpdateCenterValue();
	UpdateAABBContainer();
}

void ACollider::UpdateLocalVertexPoints() {}

void ACollider::UpdateWorldVertexPoints() {}

void ACollider::UpdateCenterValue() {
	center = glm::vec3(0);
	for (auto worldPoint : worldVertexPoints) {
		center += worldPoint;
	}
	center /= (float)worldVertexPoints.size();
}

void ACollider::DrawDebugOutline() {
	glm::vec3 debugColliderColor = debugColor;

	for (unsigned int i = 0; i < worldVertexPoints.size(); i++)
	{
		if(i == worldVertexPoints.size() - 1){
			Render::GetInstance().DrawLineSegment(
				worldVertexPoints[i],
				worldVertexPoints[0],
				debugColliderColor
			);
			continue;
		}

		Render::GetInstance().DrawLineSegment(
			worldVertexPoints[i],
			worldVertexPoints[i+1],
			debugColliderColor
		);
	}
}

void ACollider::UpdateAABBContainer() {
	aabbMin = glm::vec3(FLT_MAX);
	aabbMax = glm::vec3(-FLT_MAX);
	std::vector<glm::vec3> directions = { 
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
		{-1, 0, 0}, 
		{0, -1, 0},
		{0, 0, -1}
	};

	for(auto direction : directions) {
		glm::vec3 supportPoint = GetSupportPoint(direction);
		if(std::isnan(supportPoint.x)) continue;
		aabbMin = glm::vec3(
			std::min(aabbMin.x, supportPoint.x), 
			std::min(aabbMin.y, supportPoint.y), 
			std::min(aabbMin.z, supportPoint.z)
		);
		aabbMax = glm::vec3(
			std::max(aabbMax.x, supportPoint.x), 
			std::max(aabbMax.y, supportPoint.y), 
			std::max(aabbMax.z, supportPoint.z)
		);
	}
}