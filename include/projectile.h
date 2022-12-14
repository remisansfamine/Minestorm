#pragma once

#include "entity.h"

class Projectile : public Entity
{
	protected:
		float	m_lifeTime;
		float	m_radius = 13.5f;

		Circle	m_collider;
			
		void rotate(float deltaTime) override;
		void move(float deltaTime) override;

	public:
		Projectile(const Referential2D& referential, Color color = WHITE);

		void update(float deltaTime) override;
		void drawDebug() const override;
		void createParticle();
		Circle getCircle() const;
};