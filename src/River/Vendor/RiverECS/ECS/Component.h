#pragma once

#include <string>


namespace River::ECS {

	
	// Represents a component
	using ComponentId = uint32_t;

	/**
	 *	@brief The maximum value for ComponentId
	 */
	const ComponentId MAX_COMPONENT_ID = std::numeric_limits<ComponentId>::max();

	/**
	 * @brief	All user-defined components must inherit from this struct
	*/
	struct Component {
		/**
		 * @brief	Unique ID for each component in the same type
		*/
		ComponentId id;


	};



	/** Asserts whether or not the given type is a valid component type */
	#define RV_ECS_ASSERT_COMPONENT_TYPE(x) static_assert(std::is_base_of<Component, x>::value, "Component types must inherit from ECS::Component")


	using ComponentTypeId = int32_t;
	

	/*class ComponentTypeRegistry;

	struct ComponentTypeId {
	public:
		static const ComponentTypeId UNKNOWN;
	private:

		template <typename C>
		ComponentTypeId() : name(typeid(C).name()) {
			RV_ECS_ASSERT_COMPONENT_TYPE(C);
		}

		ComponentTypeId(const std::string& name) : name(name) {}

		const std::string name;
		friend class ComponentTypeRegistry;
	};

	const ComponentTypeId ComponentTypeId::UNKNOWN("UNKNOWN");*/

}


