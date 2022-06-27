#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <limits>
#include <type_traits>
#include <functional>

#include "Component.h"
#include "Exception.h"



namespace River::ECS {
	struct Entity;

	// Thrown if an Entity already has a component
	class MultipleComponentException : public Exception {
	public:
		MultipleComponentException(const std::string& componentName) : Exception("Entity already has component '" + componentName + "'") {}
	};

	// Thrown if the all compnent id has been used 
	class MaxComponentsException : public Exception {
	public:
		MaxComponentsException(const std::string& componentName) : Exception("Max number of components (" + std::to_string(MAX_COMPONENT_ID) + ") has been created for component '" + componentName + "'") {}
	};



	class IComponentController {
	public:
		virtual void deleteComponent(Entity* entity) = 0;
		virtual void clean() = 0;
	};


	/**
	 * @brief	Manages the creation, storage and deletion of a specifc component type
	 * @tparam  C	The type of component (must inherit from ECS::Component)
	*/
	template <typename C>
	class ComponentController : public IComponentController {
		RV_ECS_ASSERT_COMPONENT_TYPE(C);
		const static unsigned int SECONDARY_LIST_SIZE = 100;

	public:
		ComponentController() {}


		/**
		 * @brief	Creates a new component for the given Entity
		 *
		 * @return	A temporary pointer to the component. The pointer is invalidated after the compress() or clean() is called
		*/
		C* createComponent(Entity* entity) {
			/*	Created components are stored in the primary list only if the
				primary list already has memory allocated for it. Otherwise, it's
				stored in one of the secondary lists until the the controllers
				'compress' method is called. This is to prevent invalidation of
				pointers to elements in the primary list, in case new components
				are created during iteration of primary components.
			*/
			if( entity == nullptr )
				throw std::invalid_argument("Entity pointer cannot be null");

			if( componentMap.find(entity) != componentMap.end() )
				throw MultipleComponentException(typeid(C).name()); // Throw exception

			C* component = allocateComponent();

			entityMap[component->id] = entity;
			componentMap[entity] = component->id;

			return component;
		}


		/**
		 * @brief	Marks the component for deletion for the given Entity. The components will keep existing,
					until the Controller is cleaned by calling clean().
					Does nothing if the Entity doesn't the particular component.
		*/
		void deleteComponent(Entity* entity) override {
			auto iterator = componentMap.find(entity);
			if( iterator == componentMap.end() ) return;
			if( componentsToDelete.find(iterator->second) == componentsToDelete.end() )
				componentsToDelete.insert(iterator->second);
		}


		/**
		 * @brief	Get a pointer to the Entity's component
		 * @return	Temporary to pointer the Entity's component, or nullptr if the Entity doesn't have this component.
					The pointer is invalidated when the Controller is cleaned or compressed.
		*/
		C* getComponent(Entity* entity) {
			auto iterator = componentMap.find(entity);
			if( iterator == componentMap.end() )
				return nullptr;

			return getComponent(iterator->second);
		}


		/**
		 * @brief	Get a pointer to the Component with the given ComponentId
		 * @param id	The ID of the component to retrieve
		 * @return	Temporary to pointer Component, or nullptr if the the 'id' is not in use.
					The pointer is invalidated when the Controller is cleaned or compressed.
		*/
		C* getComponent(ComponentId id) {
			if( id == NULL_COMPONENT_ID )
				throw new Exception("ComponentId is null");

			// Check if id exists
			auto iterator = indexMap.find(id);
			if( iterator == indexMap.end() )
				return nullptr;

			// Find component primary list
			unsigned int index = iterator->second;
			unsigned int primaryListSize = (unsigned int)components.size();
			if( index < primaryListSize )
				return &components.at(index);

			// Find component in list of new components
			unsigned int adjustedIndex = (index - primaryListSize);
			unsigned int listIndex = adjustedIndex / SECONDARY_LIST_SIZE;
			unsigned int elementIndex = adjustedIndex % SECONDARY_LIST_SIZE;

			return &newComponents.at(listIndex).at(elementIndex);
		}


		/**
		 * @brief	Call the given callback for each Entity which has this Component
		 * @param callback	The callback to call
		*/
		void forMatchingEntities(std::function<void(Entity*, C*)> callback) {
			for( auto& component : components ) {
				auto entity = entityMap.find(component.id)->second;
				callback(entity, &component);
			}
		}


		/**
		 * @brief	Deletes components that are marked for deletion, and move newly created components to sequential data structure
		 *			This invalidated all pointers to Components returned by the controller
		*/
		void clean() override {
			moveNewComponents();
			deleteComponents();
		}


		/*
		 * @return	Current number of components
		*/
		unsigned int getNumComponents() {
			return numComponents;
		}


	private:
		// Component controller should never be moved
		ComponentController(ComponentController&& other) = delete;
		ComponentController(const ComponentController&) = delete;
		ComponentController& operator=(const ComponentController&) = delete;

		/**
		 * @brief	Finds the next available component id
		*/
		ComponentId createComponentId() {
			if( numComponents >= MAX_COMPONENT_ID )
				throw new MaxComponentsException(typeid(C).name());

			while( indexMap.find(nextId) != indexMap.end() ) {
				if( nextId == MAX_COMPONENT_ID )
					nextId = 0;
				else
					nextId++;
			}
			return nextId;
		}


		/**
		 * @brief	Adds a component with the given ID to the list of new components
		 * @return	Pointer to the newly added component
		*/
		C* allocateComponent() {
			ComponentId id = createComponentId();
			unsigned int index = numComponents;

			C* newComponent = nullptr;

			if( components.size() > numComponents ) {
				newComponent = &components[index];
				numComponentsInPrimary++;
				numComponents++;
			} else {
				if( newComponents.size() == 0 || newComponents.back().size() == SECONDARY_LIST_SIZE ) {
					// All lists are full - create new secondary list
					// This is to ensure that pointers to other newly created components aren't
					// invalidated
					newComponents.emplace_back();
					newComponents.back().reserve(SECONDARY_LIST_SIZE);
				}

				auto& list = newComponents.back();
				list.emplace_back();
				newComponent = &list.back();
				numComponents++;
			}


			(*newComponent) = C(); // Reset to default values
			newComponent->id = id;
			indexMap[id] = index;

			return newComponent;
		}


		/**
		 * @brief	Move all new components into the primary list (components), which is consecutive in memory
		 *			This invalidated all pointers to Components returned by the controller
		*/
		void moveNewComponents() {

			// Check if there are any components to move
			if( newComponents.size() == 0 ) return;

			// Resize primary list (we'll never downsize)
			if( components.size() < numComponents )
				components.resize(numComponents);

			for( auto& secondaryList : newComponents ) {
				for( auto& component : secondaryList ) {
					if( numComponents - numComponentsInPrimary <= 0 ) break;
					auto& newDestination = components.at(numComponentsInPrimary);
					newDestination = component;
					numComponentsInPrimary++;
				}
			}

			newComponents.clear();
		}


		void deleteComponents() {
			// we assume that all components have been moved to the primary list
			for( auto& componentId : componentsToDelete ) {
				auto entity = entityMap.find(componentId)->second;
				unsigned int index = indexMap.find(componentId)->second;

				auto& component = components.at(index);

				bool isNotLast = (index + 1) < numComponents;
				if( isNotLast ) {
					// Move last component to the now empty slot
					auto& last = components.at(numComponents-1);
					component = last;
					indexMap.at(last.id) = index;
				}

				// Delete component
				componentMap.erase(entity);
				entityMap.erase(componentId);
				indexMap.erase(componentId);

				numComponentsInPrimary--;
				numComponents--;
			}

			componentsToDelete.clear();
		}



	private:
		ComponentId nextId = 1;

		// Maps entity to component id
		std::unordered_map<Entity*, ComponentId> componentMap;

		// Maps component id to entity
		std::unordered_map<ComponentId, Entity*> entityMap;

		// Maps component id to index in component list
		std::unordered_map<ComponentId, unsigned int> indexMap;

		std::unordered_set<ComponentId> componentsToDelete;

		std::vector<std::vector<C>> newComponents;

		unsigned int numComponents = 0;
		unsigned int numComponentsInPrimary = 0; // Number of components in primary list
		std::vector<C> components;
	};



}
