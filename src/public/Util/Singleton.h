#pragma once

/**
 * @brief This class is used to create singletons of any type
 * @tparam T The type of the singleton
 */
template <typename T>
class Singleton {
private:
	static T* instance;
public:
	static T& GetInstance();
};

template<typename T>
T* Singleton<T>::instance = nullptr;

template <typename T>
T& Singleton<T>::GetInstance() {
	if (instance == nullptr) {
		instance = new T;
	}
	return *instance;
}

