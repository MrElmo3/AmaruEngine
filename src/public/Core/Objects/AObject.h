#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Core/Components/IComponent.h"

class ASceneController;

class AObject {
	friend class ASceneController;
public:
	std::string name;

public:
	bool isEnabled = true;
	
	/**
	 * Local position
	 */
	glm::vec3 position = {0, 0, 0};

	/**
	 * Local rotation
	 */
	glm::quat rotationQuat = { 1, 0, 0, 0};

	/**
	 * Local scale
	 */
	glm::vec3 scale = { 1, 1, 1 };

protected:
	/**
	 * World position
	 */
	glm::vec3 worldPosition = { 0, 0, 0 };

	/**
	 * World rotation
	 */
	glm::quat worldRotation = { 1, 0, 0, 0 };

	/**
	 * World scale
	 */
	glm::vec3 worldScale = { 1, 1, 1 };

public:
	
	/**
	 * The components of the object.
	 */
	std::vector<IComponent*> components;

	ASceneController* scene = nullptr;
	AObject* parent = nullptr;
	std::vector<AObject*> children;
	
	AObject(
		const std::string &_name, 
		AObject* _parent = nullptr, 
		ASceneController* _scene = nullptr
	);
	virtual ~AObject();

protected:
	virtual void Awake();
	virtual void Start();
	virtual	void FixedUpdate();
	virtual void Update(double deltaTime);
	virtual void LateUpdate();
	virtual void End();

public:
	IComponent* AddComponent(IComponent* component);

	/**
	 * @brief Gets a component of the object.
	 * @tparam T The type of the component.
	 * @return The component of the object.
	 */
	template<typename T>
	std::enable_if_t<std::is_base_of_v<IComponent, T>, T*>
	GetComponent() {
		for (IComponent* component : components) {
			if (T* t = dynamic_cast<T*>(component)) {
				return t;
			}
		}
		return nullptr;
	}

	void AddChild(AObject* child);

	/**
	 * @brief Rotates the object in the local space.
	 * @param rotation The quaternion to apply.
	 */
	void Rotate(glm::quat rotation);

	/**
	 * @brief Rotates the object in the local space.
	 * @param rotation The euler angles to apply.
	 */
	void RotateEuler(glm::vec3 rotation);

	[[nodiscard]] glm::vec3 Forward() const;
	[[nodiscard]] glm::vec3 Right() const;
	[[nodiscard]] glm::vec3 Up() const;
	
	// Getters and setters
	void SetRotation(glm::quat rotation);
	void SetWorldPosition(glm::vec3 position);
	void SetWorldRotation(glm::quat rotation);
	void SetWorldScale(glm::vec3 scale);
	
	[[nodiscard]] glm::vec3 GetWorldPosition()	const { return worldPosition; }
	[[nodiscard]] glm::quat GetWorldRotation()	const { return glm::normalize(worldRotation); }
	[[nodiscard]] glm::vec3 GetWorldScale()		const { return worldScale; }

	// Static functions

	/**
	 * @brief Finds an object in the scene by its name.
	 * @param name The name of the object.
	 * @return The object with the name.
	 */
	static AObject* FindObjectByName(const std::string& name);
};
