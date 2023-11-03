#pragma once

#include "Global_Affordances.h"
#include <vector>

/**
*	@Class AffordanceController
*	@brief Used to control an objects available affordance
*
*	@author Andres Comeros-Ochtman, Lochy Edward
*	@version 1.0
*	@date 15/10/2023
*/
class AffordanceController
{
public:
	/**
	 * @brief Constructor. Requires an owner to function properly
	 * @param owner object that owns this affordance controller
	*/
	AffordanceController(GameObject* owner);
	
	/**
	 * @brief destuctor
	*/
	~AffordanceController();

	/**
	 * @brief Updates all the affordances added to this object
	 * @param deltaTime time since last frame
	*/
	void Update(double deltaTime);
	
	/**
	 * @brief Add an affordance type to this object
	 * @tparam T affordance type
	 * @return the affordacnce added to this object
	*/
	template<class T>
	T* AddAffordance();

	/**
	 * @brief Get an afforance from this object
	 * @tparam T the affordance type retrieving
	 * @return affordance of type T retrieved
	*/
	template<class T>
	T* GetAffordance();

	/**
	 * @brief Remove an affordance from this object
	 * @tparam T the affordance type to remove
	*/
	template<class T>
	void RemoveAffordance();

	/**
	 * @brief Returns an affordance based on its name
	 * @param type the name of the affordace being retrieved
	 * @return affordance with name type, will return a
	 * blank affordance if affordance does not exist
	*/
	Affordance* GetAffordanceString(const std::string type);

	/**
	 * @brief Remove an affordance from this controller based on its name
	 * @param type name of affordance to remove
	*/
	void RemoveAffordanceString(const std::string type);

private:

	///The ower of this affordance controller
	GameObject* _owner;

	///List of all affordances the owner has
	std::vector<Affordance*> _affordances;

	///Empty affordance to return of no affordance is available
	static AffordanceBlank _blankAffordance;


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

	T* nAffordance = new T(_owner);
	_affordances.push_back(nAffordance);
	return nAffordance;
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
