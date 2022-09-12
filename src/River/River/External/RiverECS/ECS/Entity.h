#pragma once

#include "Domain.h"

namespace River::ECS {

	struct Entity {

		friend class Domain;

	public:

		Domain& getDomain() const {
			return domain;
		}


		// TODO: Document
		template <typename C>
		C* addComponent() {
			RV_ECS_ASSERT_COMPONENT_TYPE(C);
			return domain.addEntityComponent<C>(this);
		}


		// TODO: Document
		template <typename C>
		C* getComponent() {
			RV_ECS_ASSERT_COMPONENT_TYPE(C);
			return domain.getEntityComponent<C>(this);
		}


		// TODO: Document
		template <typename C>
		void removeComponent() {
			RV_ECS_ASSERT_COMPONENT_TYPE(C);
			domain.removeEntityComponent<C>(this);
		}

		
		void destroy() {
			domain.destroyEntity(this);
		}


	private:
		
		Entity(Domain& domain) : domain(domain) { }

		// Prevents entity from being deleted by anyone else than Domain
		~Entity() { }

		Entity(const Entity& other) = delete;
		// TODO: Delete move contructor, and = operator constructors as well

	private:

		Domain& domain;

	};

}