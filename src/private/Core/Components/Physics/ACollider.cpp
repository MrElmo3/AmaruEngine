#include <Core/Components/Physics/ACollider.h>
#include <Core/Render/Render.h>
#include <Core/Render/Color.h>

ACollider::ACollider() {}

ACollider::~ACollider() {}

glm::vec3 ACollider::GetSupportPoint(glm::vec3 direction){}

void ACollider::UpdateVertexPoints() {}

void ACollider::DrawDebugOutline() {
	glm::vec3 debugColliderColor = Color::GREEN;

	for (uint i = 0; i < vertexPoints.size(); i++)
	{
		if(i == vertexPoints.size() - 1){
			Render::GetInstance().DrawLineSegment(
				vertexPoints[i],
				vertexPoints[0],
				debugColliderColor
			);
			continue;
		}

		Render::GetInstance().DrawLineSegment(
			vertexPoints[i],
			vertexPoints[i+1],
			debugColliderColor
		);
	}
	
}