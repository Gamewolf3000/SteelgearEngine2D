#pragma once
//https://stackoverflow.com/questions/45715219/store-functions-with-different-signatures-in-a-map
#include <vector>
#include <unordered_map>
#include <functional>
#include <any>

template <typename T>
class EventManager
{
public:

	class EventHandle
	{
	private:
		template<typename templateType> friend class EventManager;

		T eventType;
		int identifier;

		EventHandle(T type, int identifier) : eventType(type), identifier(identifier)
		{

		}

	public:
		EventHandle() : eventType(T()), identifier(-1)
		{

		}
	};

	EventManager() {};
	~EventManager() {};

	template<typename... Argtypes>
	EventHandle BindFunctionToEvent(T eventType, void(ptr)(Argtypes...)); // For declared functions

	template<typename... Argtypes>
	EventHandle BindFunctionToEvent(T eventType, std::function<void(Argtypes...)>& func); // For std::functions

	template<typename... Argtypes, typename Func>
	EventHandle BindFunctionToEvent(T eventType, Func& lambda); // For lambdas, require the parameter list to be specified as template parameter BindFunctionToEvent<type1, type2...>

	template<typename... Args>
	void TriggerEvent(T eventToTrigger, Args... parameters);
	void RemoveEvent(EventHandle& handle);

private:

	struct StoredEvent
	{
		std::any func;
		unsigned int id;
	};

	std::unordered_map<T, std::vector<StoredEvent>> events;
	unsigned int idCounter = 0;

};

template<typename T>
template<typename... Argtypes>
inline typename EventManager<T>::EventHandle EventManager<T>::BindFunctionToEvent(T eventType, void(ptr)(Argtypes...))
{
	StoredEvent temp;
	temp.func = std::function<void(Argtypes...)>(ptr);
	temp.id = idCounter++;

	events[eventType].push_back(temp);

	EventHandle toReturn(eventType, temp.id);
	return toReturn;
}

template<typename T>
template<typename... Argtypes>
inline typename EventManager<T>::EventHandle EventManager<T>::BindFunctionToEvent(T eventType, std::function<void(Argtypes...)>& func)
{
	StoredEvent temp;
	temp.func = func;
	temp.id = idCounter++;

	events[eventType].push_back(temp);

	EventHandle toReturn(eventType, temp.id);
	return toReturn;
}

template<typename T>
template<typename... Argtypes, typename Func>
inline typename EventManager<T>::EventHandle EventManager<T>::BindFunctionToEvent(T eventType, Func& lambda)
{
	StoredEvent temp;
	temp.func = std::function<void(Argtypes...)>(lambda);
	temp.id = idCounter++;

	events[eventType].push_back(temp);

	EventHandle toReturn(eventType, temp.id);
	return toReturn;
}

template<typename T>
template<typename ...Args>
inline void EventManager<T>::TriggerEvent(T eventToTrigger, Args ...parameters)
{
	for (auto& eventFunc : events[eventToTrigger])
	{
		//try {
		//	std::any_cast<std::function<void(Args...)>>(eventFunc)(parameters...);
		//}
		//catch (const std::bad_any_cast& e) {
		//	std::cout << e.what() << '\n';
		//}


		std::any_cast<std::function<void(Args...)>>(eventFunc.func)(parameters...);

	}

}

template<typename T>
inline void EventManager<T>::RemoveEvent(EventHandle & handle)
{
	int size = events[handle.eventType].size();
	for (int i = 0; i < size; i++)
	{
		if (events[handle.eventType][i].id == handle.identifier)
		{
			events[handle.eventType][i] = events[handle.eventType].back();
			events[handle.eventType].pop_back();
			return;
		}
	}

}
