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

void ACollider::DrawDebugOutline() {
	glm::vec3 debugColliderColor = Color::GREEN;
	float extraOffset = 1.f;

	for (unsigned int i = 0; i < vertexPoints.size(); i++)
	{
		if(i == vertexPoints.size() - 1){
			Render::GetInstance().DrawLineSegment(
				vertexPoints[i] * extraOffset,
				vertexPoints[0] * extraOffset,
				debugColliderColor
			);
			continue;
		}

		Render::GetInstance().DrawLineSegment(
			vertexPoints[i] * extraOffset,
			vertexPoints[i+1] * extraOffset,
			debugColliderColor
		);
	}
	
}