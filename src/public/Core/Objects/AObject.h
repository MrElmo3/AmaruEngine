#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Core/Global.h>

class IComponent;
class ASceneController;

class AObject {
friend class ASceneController;
public:
	std::string name;

public:
	bool isEnabled = true;

protected:
	/**
	 * Local position
	 */
	glm::vec3 position = {0, 0, 0};

	/**
	 * Local rotation
	 */
	glm::quat rotation = { 1, 0, 0, 0};

	/**
	 * Local scale
	 */
	glm::vec3 scale = { 1, 1, 1 };

	/**
	 * World position
	 */
	glm::vec3 worldPosition = { 0, 0, 0 };

	/**
	 * World rotation
	 */
	glm::quat worldRotation = { 1, 0, 0, 0 };

	
	/// @brief World scale
	glm::vec3 worldScale = { 1, 1, 1 };

	
	/// @brief World transform matrix of the object.
	glm::mat4 worldTransform;

	/// @brief Flag used when the object changes its position, rotation or scale.
	bool isDirty = true;

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

private:
	/// @brief Marks this object, its components and its children as dirty;
	void MarkDirty();

	/// @brief Updates the World values if is dirty.
	void UpdateWorldValues();

	/// @brief Updated the matrix transform if is dirty.
	void UpdateTransform();	

public:

	/// @brief Adds a component of a certain type to the object.
	/// @tparam T The type of the component.
	/// @return The component added.
	template<typename T>
	std::enable_if_t<std::is_base_of_v<IComponent, T>, T*>
	AddComponent(){
		T* component = new T(this);
		components.push_back(component);
		return component;
	}

	/// @brief Gets a component of the object.
	/// @tparam T The type of the component that inherits from IComponent.
	///@return The first component found of the object.
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

	/// @brief Searchs all of the components for a specific type of component T.
	/// @tparam T The type of the component that inherits from IComponent.
	/// @return A vector of all of the components recolected.
	template<typename T>
	std::enable_if_t<std::is_base_of_v<IComponent, T>, std::vector<T*>>
	GetComponentsOfType() {
		std::vector<T*> returnComponents = std::vector<T*>();
		for (IComponent* component : components) {
			if (T* t = dynamic_cast<T*>(component)) {
				returnComponents.push_back(t);
			}
		}
		return returnComponents;
	}

	/// @brief Searchs recursievly all of the children for a sp[ecific type of component T.
	/// @tparam T The type of the component that inherits from IComponent.
	/// @return A vector of all of the components recolected.
	template<typename T>
	std::enable_if_t<std::is_base_of_v<IComponent, T>, std::vector<T*>>
	GetAllComponentsInChildren() {
		std::vector<T*> returnComponents = GetComponentsOfType<T>();

		for (auto& child : children) {
			std::vector<T*> childComponents = GetAllComponentsInChildren<T>();
			returnComponents.insert(returnComponents.end(), childComponents.begin(), childComponents.end());
		}
		return returnComponents;
	}

	void AddChild(AObject* child);

	/// @brief Translates the object in the local space.
	/// @param positionDelta The amount to translate the object.
	void Translate(glm::vec3 positionDelta);

	
	/// @brief Rotates the object in the local space.
	/// @param rotation The quaternion to apply.
	void Rotate(glm::quat rotationDelta);

	
	/// @brief Rotates the object in the local space.
	/// @param rotation The euler angles to apply.
	void RotateEuler(glm::vec3 rotationDelta);

	glm::vec3 Forward() const { return GetWorldRotation() * Global::FORWARD; }
	glm::vec3 Right() 	const { return GetWorldRotation() * Global::RIGHT; }
	glm::vec3 Up() 		const { return GetWorldRotation() * Global::UP; }
	
	// Getters and setters
	void SetPosition(glm::vec3 position);
	void SetRotation(glm::quat rotation);
	void SetScale(glm::vec3 size);

	void SetWorldPosition(glm::vec3 position);
	void SetWorldRotation(glm::quat rotation);
	void SetWorldScale(glm::vec3 scale);
	
	glm::vec3 GetPosition()		const { return position; }
	glm::quat GetRotation()		const { return glm::normalize(rotation); }
	glm::vec3 GetScale()		const { return scale; }

	glm::vec3 GetWorldPosition()	const { return worldPosition; }
	glm::quat GetWorldRotation()	const { return glm::normalize(worldRotation); }
	glm::vec3 GetWorldScale()		const { return worldScale; }

	glm::mat4 GetTransformMatrix() 	const { return worldTransform; }

	bool IsDirty() { return isDirty; }

public:
	// Static functions

	/**
	 * @brief Finds an object in the scene by its name.
	 * @param name The name of the object.
	 * @return The object with the name.
	 */
	static AObject* FindObjectByName(const std::string& name);
};
