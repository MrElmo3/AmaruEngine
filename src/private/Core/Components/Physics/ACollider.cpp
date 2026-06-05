#include <Core/Components/Physics/ACollider.h>
#include <Core/Render/Render.h>
#include <Core/Render/Color.h>
#include <Util/Logger.h>
#include <stdexcept>

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

void ACollider::UpdateLocalVertexPoints() {
	throw std::logic_error("ACollider::UpdateRenderPoints() is not implemented yet.");
}

void ACollider::UpdateWorldVertexPoints() {
	throw std::logic_error("ACollider::UpdateRenderPoints() is not implemented yet.");
}

void ACollider::UpdateRenderIndexes() {
	throw std::logic_error("ACollider::UpdateRenderPoints() is not implemented yet.");
}

void ACollider::UpdateCenterValue() {
	center = glm::vec3(0);
	for (auto worldPoint : worldVertexPoints) {
		center += worldPoint;
	}
	center /= (float)worldVertexPoints.size();
}

void ACollider::DrawDebugOutline() {
	if (renderIndexes.empty()) UpdateRenderIndexes();
	glm::vec3 debugColliderColor = debugColor;

	for (unsigned int i = 0; i < renderIndexes.size(); i += 2) {
		Render::GetInstance().DrawLineSegment(
				worldVertexPoints[renderIndexes[i]],
				worldVertexPoints[renderIndexes[i+1]],
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