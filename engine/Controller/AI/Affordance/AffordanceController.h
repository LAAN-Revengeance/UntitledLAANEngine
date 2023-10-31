#pragma once

#include "Global_Affordances.h"
#include <vector>

class AffordanceController
{
public:
	AffordanceController(GameObject* owner);

	~AffordanceController();

	void Update(double deltaTime);

	template<class T>
	T* AddAffordance();

	template<class T>
	T* GetAffordance();

	template<class T>
	void RemoveAffordance();

	//Lua implementations
	Affordance* GetAffordanceString(const std::string type);
	void RemoveAffordanceString(const std::string type);

private:

	GameObject* _owner;
	std::vector<Affordance*> _affordances;

	friend class SceneLoader;
};

template<class T>
inline T* AffordanceController::AddAffordance() {

	for (auto& affordance : _affordances)
	{
		if (dynamic_cast<T*>(affordance)) {
			return dynamic_cast<T*>(affordance);
		}
	}

	T* nAffordance = new T(_owner);
	_affordances.push_back(nAffordance);
	return nAffordance;
}

template<class T>
inline T* AffordanceController::GetAffordance() {

	for (auto& affordance : _affordances)
	{
		if (dynamic_cast<T*>(affordance)) {
			return dynamic_cast<T*>(affordance);
		}
	}

	return nullptr;

}

template<class T>
inline void AffordanceController::RemoveAffordance() {
	for (auto it = _affordances.begin(); it != _affordances.end(); ) {

		if(dynamic_cast<T*>(*it)) {
			delete *it;
			it = _affordances.erase(it);
		}else {
       
			++it;
		}
	}
}
