#include "Core/Render/Render.h"

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stb_image.h>
#include <Core/Global.h>
#include <Core/Window.h>
#include <Core/Materials/ColorMaterial.h>
#include <Core/Objects/AObject.h>
#include <Core/Render/Shader.h>
#include <Core/Materials/AMaterial.h>
#include <Core/Components/Render/CameraComponent.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Render/LightData.h>
#include <Util/Logger.h>
#include <Core/Render/Model.h>
#include <Core/Scenes/ASceneController.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <cfloat>

// The same UVs for each of the 6 faces of the cube
static const std::vector<glm::vec2> DEFAULT_CUBE_UV = [] {
	std::vector<glm::vec2> uv;
	for (int face = 0; face < 6; face++) {
		uv.insert(uv.end(), {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f},
		});
	}
	return uv;
}();

Render::Render() {
	gizmosMaterial = new ColorMaterial();
	InitQuad();
	InitLine();
	InitCube();
	InitShadowMap();
	InitUniformCameraBuffer();
	InitUniformLightBuffer();
}

Render::~Render() {

}

Shader* Render::CreateShader(
	const std::string vertexPath, 
	const std::string fragmentPath
) {

	for (Shader* shader : shaders) {
		if (shader->CompareFiles(vertexPath, fragmentPath)) {
			return shader;
		}
	}

	Shader* newShader = new Shader(vertexPath, fragmentPath);
	shaders.push_back(newShader);
	return newShader;
}

unsigned int Render::GenerateTexture(const std::string texturePath) {

	if (textures.find(texturePath) != textures.end()) {
		return textures[texturePath];
	}

	unsigned int generatedTexture;
	glGenTextures(1, &generatedTexture);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(
		texturePath.c_str(),
		&width,
		&height,
		&nrChannels,
		0
	);

	if (data) {
		GLenum format;
		switch (nrChannels) {
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			Logger::Error("Failed to load texture");
			return 0;
		}

		glBindTexture(GL_TEXTURE_2D, generatedTexture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			width,
			height,
			0,
			format,
			GL_UNSIGNED_BYTE,
			data
		);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else {
		Logger::Error("Failed to load texture");
		return 0;
	}

	textures[texturePath] = generatedTexture;

	glBindTexture(GL_TEXTURE_2D, 0);

	return generatedTexture;
}

void Render::InitUniformCameraBuffer() {
	glGenBuffers(1, &cameraUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
	glBufferData(GL_UNIFORM_BUFFER, (2* sizeof(glm::mat4)) + sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Render::InitUniformLightBuffer() {
	glGenBuffers(1, &lightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
	glBufferData(GL_UNIFORM_BUFFER,  sizeof(LightData) * 8, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Render::InitLine() {
	glGenVertexArrays(1, &VAOLine);

	glBindVertexArray(VAOLine);

	glGenBuffers(1, &VBOLine);
	glBindBuffer(GL_ARRAY_BUFFER, VBOLine);
	glBufferData(
		GL_ARRAY_BUFFER, 
		6 * sizeof(float), 
		nullptr, 
		GL_DYNAMIC_DRAW
	);

	glVertexAttribPointer(
		0, 3, 
		GL_FLOAT, GL_FALSE, 
		3 * sizeof(float), (void*)0
	);
	glEnableVertexAttribArray(0);
}

void Render::InitQuad() {

	glGenVertexArrays(1, &VAOQuad);
	glBindVertexArray(VAOQuad);

	//position buffer
	std::vector<glm::vec3> verticesPos = {
		{-0.5f,	-0.5f,	0.0f},
		{ 0.5f,	-0.5f,	0.0f},
		{ 0.5f,	0.5f,	0.0f},
		{-0.5f,	0.5f,	0.0f}
	};
	glGenBuffers(1, &VBOQuadPos);
	glBindBuffer(GL_ARRAY_BUFFER, VBOQuadPos);
	glBufferData(
		GL_ARRAY_BUFFER,
		verticesPos.size() * sizeof(glm::vec3),
		verticesPos.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VBOQuadUv);
	glBindBuffer(GL_ARRAY_BUFFER, VBOQuadUv);
	glBufferData(
		GL_ARRAY_BUFFER,
		4 * sizeof(glm::vec2),
		nullptr,
		GL_DYNAMIC_DRAW
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//EBO
	std::vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3
	};
	glGenBuffers(1, &EBOQuad);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOQuad);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		indices.data(),
		GL_STATIC_DRAW
	);
}

void Render::InitCube() {

	//VAO
	glGenVertexArrays(1, &VAOCube);
	glBindVertexArray(VAOCube);

	//VBO POS
	std::vector<glm::vec3> verticesPos = {

		//right face
		{+0.5f,	-0.5f,	+0.5f},
		{+0.5f,	-0.5f,	-0.5f},
		{+0.5f,	+0.5f,	-0.5f},
		{+0.5f,	+0.5f,	+0.5f},

		//left face
		{-0.5f,	-0.5f,	-0.5f},
		{-0.5f,	-0.5f,	+0.5f},
		{-0.5f,	+0.5f,	+0.5f},
		{-0.5f,	+0.5f,	-0.5f},

		//top face
		{-0.5f,	+0.5f,	+0.5f},
		{+0.5f,	+0.5f,	+0.5f},
		{+0.5f,	+0.5f,	-0.5f},
		{-0.5f,	+0.5f,	-0.5f},

		//bottom face
		{-0.5f,	-0.5f,	-0.5f},
		{+0.5f,	-0.5f,	-0.5f},
		{+0.5f,	-0.5f,	+0.5f},
		{-0.5f,	-0.5f,	+0.5f},

		//front face
		{-0.5f,	-0.5f,	+0.5f},
		{+0.5f,	-0.5f,	+0.5f},
		{+0.5f,	+0.5f,	+0.5f},
		{-0.5f,	+0.5f,	+0.5f},

		//back face
		{-0.5f,	-0.5f,	-0.5f},
		{+0.5f,	-0.5f,	-0.5f},
		{+0.5f,	+0.5f,	-0.5f},
		{-0.5f,	+0.5f,	-0.5f},
	};

	glGenBuffers(1, &VBOCubePos);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCubePos);
	glBufferData(
		GL_ARRAY_BUFFER,
		verticesPos.size() * sizeof(glm::vec3),
		verticesPos.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//VBO UV
	glGenBuffers(1, &VBOCubeUv);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCubeUv);
	glBufferData(
		GL_ARRAY_BUFFER,
		DEFAULT_CUBE_UV.size() * sizeof(glm::vec2),
		DEFAULT_CUBE_UV.data(),
		GL_DYNAMIC_DRAW
	);
	// Lit.vert / PBR.vert (and Mesh) expect: 0 = position, 1 = normal, 2 = uv.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//VBO NORMALS
	std::vector<glm::vec3> verticesNorm = {
		//right face
		{+1.0f,	-0.0f,	+0.0f},
		{+1.0f,	-0.0f,	-0.0f},
		{+1.0f,	+0.0f,	-0.0f},
		{+1.0f,	+0.0f,	+0.0f},

		//left face
		{-1.0f,	-0.0f,	-0.0f},
		{-1.0f,	-0.0f,	+0.0f},
		{-1.0f,	+0.0f,	+0.0f},
		{-1.0f,	+0.0f,	-0.0f},

		//top face
		{-0.0f,	+1.0f,	+0.0f},
		{+0.0f,	+1.0f,	+0.0f},
		{+0.0f,	+1.0f,	-0.0f},
		{-0.0f,	+1.0f,	-0.0f},

		//bottom face
		{-0.0f,	-1.0f,	-0.0f},
		{+0.0f,	-1.0f,	-0.0f},
		{+0.0f,	-1.0f,	+0.0f},
		{-0.0f,	-1.0f,	+0.0f},

		//front face
		{-0.0f,	-0.0f,	+1.0f},
		{+0.0f,	-0.0f,	+1.0f},
		{+0.0f,	+0.0f,	+1.0f},
		{-0.0f,	+0.0f,	+1.0f},

		//back face
		{-0.0f,	-0.0f,	-1.0f},
		{+0.0f,	-0.0f,	-1.0f},
		{+0.0f,	+0.0f,	-1.0f},
		{-0.0f,	+0.0f,	-1.0f},
	};

	glGenBuffers(1, &VBOCubeNorm);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCubeNorm);
	glBufferData(
		GL_ARRAY_BUFFER,
		verticesNorm.size() * sizeof(glm::vec3),
		verticesNorm.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//EBO
	std::vector<unsigned int> indices = {
		//right face
		0, 1, 3,
		1, 2, 3,

		//left face
		4, 5, 7,
		5, 6, 7,

		//top face
		8, 9, 11,
		9, 10, 11,

		//bottom face
		12, 13, 15,
		13, 14, 15,

		//front face
		16, 17, 19,
		17, 18, 19,

		//back face
		20, 21, 23,
		21, 22, 23
	};

	glGenBuffers(1, &EBOCube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOCube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		indices.data(),
		GL_STATIC_DRAW
	);
}

void Render::SetCameraValues() {
	glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(currentCamera->GetViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, 64, sizeof(glm::mat4), glm::value_ptr(currentCamera->GetProjectionMatrix())); 
	glBufferSubData(GL_UNIFORM_BUFFER, 128, sizeof(glm::vec3), glm::value_ptr(currentCamera->parent->GetWorldPosition())); 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Render::SetLightValues() {
	std::vector<LightData> lightData = Window::GetInstance().GetActualScene()->GetSceneLightsData();
	glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightData) * 8, lightData.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Render::DrawLineSegment(
	glm::vec3 start, 
	glm::vec3 end, 
	glm::vec3 color
) {
	if (currentCamera == nullptr) return;

	gizmosMaterial->SetColor(color);

	gizmosMaterial->Use();

	gizmosMaterial->shader->SetMatrix4("_model", glm::mat4(1.0f));

	glLineWidth(1);

	glBindVertexArray(VAOLine);

	std::vector<float> vertices = {
		(float)start.x, (float)start.y, (float)start.z,
		(float)end.x, (float)end.y, (float)end.z
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBOLine);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		0,
		vertices.size() * sizeof(float),
		vertices.data()
	);

	glDrawArrays(GL_LINES, 0, 2);

	glBindVertexArray(0);
}

void Render::DrawQuadLine(
	glm::vec2 center, 
	glm::vec2 scale, 
	glm::vec3 color
) {

	glm::vec3 points[] = {
		glm::vec3(center.x - scale.x / 2, center.y - scale.y / 2, 0), //bottom left
		glm::vec3(center.x + scale.x / 2, center.y - scale.y / 2, 0), //bottom right
		glm::vec3(center.x + scale.x / 2, center.y + scale.y / 2, 0), //top right
		glm::vec3(center.x - scale.x / 2, center.y + scale.y / 2, 0), //top left
	};

	DrawLineSegment(points[0], points[1], color);
	DrawLineSegment(points[1], points[2], color);
	DrawLineSegment(points[2], points[3], color);
	DrawLineSegment(points[3], points[0], color);

}

void Render::DrawQuad(
	glm::mat4* modelMatrix,
	AMaterial* material,
	std::vector<glm::vec2>* uv
) {

	if (currentCamera == nullptr) return;

	material->Use();
	material->shader->SetMatrix4("_model", *modelMatrix);

	glBindVertexArray(VAOQuad);

	if (uv == nullptr || uv->size() != 4) {
		uv = new std::vector<glm::vec2>({
			{0.0f,	0.0f},
			{1.0f,	0.0f},
			{1.0f,	1.0f},
			{0.0f,	1.0f},
			});
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBOQuadUv);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		0,
		uv->size() * sizeof(float),
		uv->data()
	);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Render::DrawCube(
	glm::mat4* modelMatrix,
	AMaterial* material,
	std::vector<glm::vec2>* uv
) {

	if (!currentCamera) return;

	// The cube is not drawn right away: the shadow pass needs to know every
	// cube of the frame before the first one is shaded. See Flush().
	CubeDrawCommand command;
	command.model = *modelMatrix;
	command.material = material;
	if (uv != nullptr && uv->size() == 24) command.uv = *uv;
	cubeQueue.push_back(command);
}

void Render::InitShadowMap() {
	shadowShader = new Shader(
		"Assets/Shaders/Shadow/ShadowDepth.vert",
		"Assets/Shaders/Shadow/ShadowDepth.frag"
	);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
		SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, nullptr
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Everything outside of the shadow map counts as lit
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	const float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	// Depth only: no color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Logger::Error("Render::InitShadowMap -> The shadow framebuffer is not complete");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Render::RenderShadowPass(glm::mat4& lightSpaceMatrix) {
	LightComponent* light = Window::GetInstance().GetActualScene()->directionalLight;
	if (light == nullptr) return false;

	glm::vec3 direction = light->GetLightData().direction;
	if (glm::length(direction) < 0.0001f) return false;
	direction = glm::normalize(direction);

	// World bounds of every cube, so the light frustum covers the whole scene
	glm::vec3 minBounds(FLT_MAX);
	glm::vec3 maxBounds(-FLT_MAX);
	for (const CubeDrawCommand& command : cubeQueue) {
		for (int corner = 0; corner < 8; corner++) {
			glm::vec4 local(
				(corner & 1) ? 0.5f : -0.5f,
				(corner & 2) ? 0.5f : -0.5f,
				(corner & 4) ? 0.5f : -0.5f,
				1.0f);
			glm::vec3 world = glm::vec3(command.model * local);
			minBounds = glm::min(minBounds, world);
			maxBounds = glm::max(maxBounds, world);
		}
	}
	glm::vec3 center = (minBounds + maxBounds) * 0.5f;
	float radius = glm::length(maxBounds - minBounds) * 0.5f;
	if (radius < 0.0001f) return false;

	// A directional light has no position: place the "camera" of the light
	// outside of the scene bounds, looking at the center along its direction.
	glm::vec3 up = glm::abs(direction.y) > 0.99f ? glm::vec3(0, 0, 1) : glm::vec3(0, 1, 0);
	glm::mat4 lightView = glm::lookAt(center - direction * radius, center, up);
	glm::mat4 lightProjection = glm::ortho(-radius, radius, -radius, radius, 0.0f, 2.0f * radius);
	lightSpaceMatrix = lightProjection * lightView;

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	glClear(GL_DEPTH_BUFFER_BIT);

	shadowShader->Use();
	shadowShader->SetMatrix4("_lightSpaceMatrix", lightSpaceMatrix);

	glBindVertexArray(VAOCube);
	for (const CubeDrawCommand& command : cubeQueue) {
		shadowShader->SetMatrix4("_model", command.model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	return true;
}

void Render::Flush() {
	if (cubeQueue.empty()) return;

	glm::mat4 lightSpaceMatrix(1.0f);
	const bool hasShadows = RenderShadowPass(lightSpaceMatrix);

	glActiveTexture(GL_TEXTURE0 + SHADOW_MAP_UNIT);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glBindVertexArray(VAOCube);
	for (const CubeDrawCommand& command : cubeQueue) {
		command.material->Use();

		Shader* shader = command.material->shader;
		shader->SetMatrix4("_model", command.model);
		shader->SetMatrix3("_normalModel", glm::mat3(glm::transpose(glm::inverse(command.model))));

		// Only the shaders that declare these uniforms (PBR) use them
		shader->SetBool("_receiveShadows", hasShadows);
		shader->SetMatrix4("_lightSpaceMatrix", lightSpaceMatrix);
		shader->SetInt("_shadowMap", SHADOW_MAP_UNIT);

		if (!command.uv.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, VBOCubeUv);
			glBufferSubData(GL_ARRAY_BUFFER, 0, command.uv.size() * sizeof(glm::vec2), command.uv.data());
		}

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		if (!command.uv.empty()) {
			// Leave the default UVs for the next cube
			glBufferSubData(GL_ARRAY_BUFFER, 0, DEFAULT_CUBE_UV.size() * sizeof(glm::vec2), DEFAULT_CUBE_UV.data());
		}
	}
	glBindVertexArray(0);

	// The cubes changed the bound shader behind SetCurrentMaterial's back
	currentMaterial = nullptr;
	cubeQueue.clear();
}

void Render::DrawModel(
	Model* model,
	AMaterial* material,
	glm::mat4* modelMatrix
) {
	if (!currentCamera) return;
	SetCurrentMaterial(material);
	model->Draw(material, modelMatrix);
}

void Render::SetCurrentMaterial(AMaterial* material) {
	if (currentMaterial == nullptr || currentMaterial != material) {
		currentMaterial = material;
		currentMaterial->Use();
	}
}

void Render::SetCurrentCamera(CameraComponent* camera) {
	currentCamera = camera;
}