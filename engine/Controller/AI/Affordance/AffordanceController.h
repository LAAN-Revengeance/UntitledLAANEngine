#pragma once

#include "Affordance.h"
#include <vector>

class AffordanceController
{
public:
	AffordanceController(GameObject* owner);

	~AffordanceController();

	void Update(double delteTime);

	template<class T>
	T* AddAffordance();

	template<class T>
	T* GetAffordance();

	template<class T>
	void RemoveAffordance();

private:

	GameObject* _owner;
	std::vector<Affordance*> _affordances;
};

template<class T>
inline T* AffordanceController::AddAffordance() {

	for (auto& affordance : _affordances)
	{
		if (dynamic_cast<T*>(affordance)) {
			return dynamic_cast<T>(affordance);
		}
	}
	_affordances.push_back(new T())
}

template<class T>
inline T* AffordanceController::GetAffordance() {

	for (auto& affordance : _affordances)
	{
		if (dynamic_cast<T*>(affordance)) {
			return dynamic_cast<T>(affordance);
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
       
			++it
		}
	}
}
