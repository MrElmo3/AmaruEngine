#include "Util/Singleton.h"
#include "Core/Window.h"
#include "Core/Render/Render.h"

template<typename T>
T* Singleton<T>::instance = nullptr;

template <typename T>
T& Singleton<T>::GetInstance() {
	if (instance == nullptr) {
		instance = new T;
	}
	return *instance;
}
