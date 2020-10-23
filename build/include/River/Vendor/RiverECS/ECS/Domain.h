#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <functional>

#include "ComponentTypeRegistry.h"
#include "ComponentController.h"
#include "Component.h"

#include "SignatureArray/Signature.h"
#include "SignatureArray/SignatureArray.h"


namespace River::ECS {

	struct Entity;

	class Domain {
	public:

		Domain();
		~Domain();


		Entity* createEntity();


		template <typename C>
		C* addEntityComponent(Entity* entity) {
			RV_ECS_ASSERT_COMPONENT_TYPE(C);
			auto componentController = getComponentController<C>();		
			auto component = componentController->createComponent(entity);

			entityComponentsToCreate[entity].push_back(ComponentTypeRegistry::getTypeId<C>());
			return component;
		}



		template <typename C>
		C* getEntityComponent(Entity* entity) {
			RV_ECS_ASSERT_COMPONENT_TYPE(C);
			auto componentController = getComponentController<C>();
			return componentController->getComponent(entity);
		}


		template <typename C>
		void removeEntityComponent(Entity* entity) {
			RV_ECS_ASSERT_COMPONENT_TYPE(C);
			entityComponentsToDelete[entity].push_back(ComponentTypeRegistry::getTypeId<C>());
		}						  






		template <typename C>
		void forEachEntity(std::function<void (Entity*, C*)> callback) {
			auto componentController = getComponentController<C>();
			componentController->forEachEntity(callback);
		}


		template <typename ... C, typename Func>
		void forEachEntity(Func callback) {
			
			// TODO: Store signature across queries (no need to create new each query)
			Signature signature(ComponentTypeRegistry::getNumTypes());
			addComponentTypeToSignature<C...>(signature);
			
			signatures.forMatchingSignatures(signature, [this, &callback](unsigned int signatureIndex) {
				Entity* entity = this->signatureIndexEntityMap.find(signatureIndex)->second;
				callback(entity, getEntityComponent<C>(entity)...);
			});
		}

		/*template <typename T>
		T* getEntityComponent(Entity* entity) {

			LOG("Adding component");

		}*/


		/*template <typename T>
		T getComponent(ComponentId<T> id);*/


		/**
		 * @brief Marks Entity for deletion on next flush
		 * @param  
		*/
		void destroyEntity(Entity*);


		/**
		 * @brief Flushes the Domain, deleting all Entities marked for deletion
		*/
		void clean();



		
	private:
		template <typename C>
		ComponentController<C>* getComponentController() {
			RV_ECS_ASSERT_COMPONENT_TYPE(C);

			auto componentTypeId = ComponentTypeRegistry::getTypeId<C>();
			auto controllerIterator = componentControllers.find(componentTypeId);

			if( controllerIterator != componentControllers.end() )
				return (ComponentController<C>*) controllerIterator->second;

			// Component Type isn't registered yet, so it's registered and then returned
			auto emplaceResult = componentControllers.emplace(componentTypeId, new ComponentController<C>());
			return (ComponentController<C>*) emplaceResult.first->second;
		}


		template <typename C>
		void addComponentTypeToSignature(Signature& signature) {
			RV_ECS_ASSERT_COMPONENT_TYPE(C);

			// The given Component type may not have been registered in the system yet
			// so this is basically to check that, and resize the Signature in
			// case the Component is new.
			auto typeId = ComponentTypeRegistry::getTypeId<C>();
			if( signature.getSize() <= typeId )
				signature.resize(typeId+1);

			signature.set(ComponentTypeRegistry::getTypeId<C>());
		}

		template <typename CFirst, typename CSecond, typename ... CRest>
		void addComponentTypeToSignature(Signature& signature) {
			addComponentTypeToSignature<CFirst>(signature);
			addComponentTypeToSignature<CSecond, CRest...>(signature);
		}


	private:
		// TODO: Change this to unordered_set
		std::vector<Entity*> entities;

		/**
		 * @brief Maps an Entity to its index into the 'entities' vector */
		std::unordered_map<Entity*, unsigned int> entityIndices;

		std::unordered_set<Entity*> newEntities;
		std::unordered_set<Entity*> entitiesToDelete;

		/**
		 * @brief Maps an entity to an index into the signature array */
		std::unordered_map<Entity*, unsigned int> entitySignatureIndexMap;

		/**
		 * @brief Maps an signature index to an entiy */
		std::unordered_map<unsigned int, Entity*> signatureIndexEntityMap; // TODO: use vector instead


		std::unordered_map<Entity*, std::vector<ComponentTypeId>> entityComponentsToCreate;
		std::unordered_map<Entity*, std::vector<ComponentTypeId>> entityComponentsToDelete;
		


		SignatureArray signatures;

		std::unordered_map<ComponentTypeId, IComponentController*> componentControllers;

	};

}






