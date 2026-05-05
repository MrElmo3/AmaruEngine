#include <Core/Components/Physics/ACollider.h>
#include <Core/Render/Render.h>
#include <Core/Render/Color.h>
#include <Util/Logger.h>

ACollider::ACollider() {}

ACollider::~ACollider() {}

glm::vec3 ACollider::GetSupportPoint(glm::vec3 direction){ 
	Logger::Warning("GetSupportPoint its not well defined or direction is 0");
	return glm::vec3(NAN);
}

void ACollider::UpdateVertexPoints() {}

void ACollider::UpdateWorldVertexPoints() {}

void ACollider::DrawDebugOutline() {
	glm::vec3 debugColliderColor = Color::GREEN;

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