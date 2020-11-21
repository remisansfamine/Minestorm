#include "player.h"

#include "entity_manager.h"

#include "bullet.h"
#include "mine.h"

#include "game.h"

#include <algorithm>
#include <vector>

void Player::createCollider(float size)
{
	ConvexPolygon firstTriangle;
	firstTriangle.pts =
	{
		{ 0.f, -70.f * size },
		{ -50.f * size, 77.f * size },
		{ 0.f, 66.f * size },
	};

	ConvexPolygon secondTriangle;
	secondTriangle.pts =
	{
		{ 0.f, 66.f * size },
		{ 50.f * size, 77.f * size },
		{ 0.f, -70.f * size },
	};

	m_collider.polygon = { firstTriangle, secondTriangle };

	m_shootCooldown = m_fireRate;
}

Player::Player(int index, const Input& input, const Referential2D& referential,
	Color color)
	: m_index(index), m_input(input), Entity(referential)
{
	m_size = 0.5f;

	m_srcRect = { 0, 0, 256, 256 };

	m_rotationSpeed = 2.5f;

	m_translationSpeed = 10000.f;

	m_color = color;

	createCollider(0.15f * m_size + 0.15f);

	entityManager->m_player.push_back(*this);
}

void Player::update(float deltaTime)
{
	m_shootCooldown -= deltaTime;

	if (IsKeyPressed(m_input.m_randomPos1) || IsKeyPressed(m_input.m_randomPos2))
		randomTeleport();

	if (IsKeyDown(m_input.m_shoot) && entityManager && m_shootCooldown <= 0)
	{
		m_shootCooldown = m_fireRate;
		Bullet(m_referential, m_color, this);
	}

	int yAxis = IsKeyDown(m_input.m_right) - IsKeyDown(m_input.m_left);

	if (yAxis)
		rotate(yAxis * deltaTime);

	move(deltaTime);

	checkCollision();
}

void Player::checkCollision()
{
	ConcavePolygon playerGlobal = m_referential.concaveToGlobal(m_collider);
	playerGlobal.getAABB();

	for (Mine* mine : entityManager->m_mine)
	{
		if (!mine || mine->m_shouldBeDestroyed)
			continue;

		ConcavePolygon mineGlobal =mine->m_referential.concaveToGlobal(mine->m_collider);

		mineGlobal.getAABB();

		if (intersect(playerGlobal, mineGlobal))
		{
			hurt();

			mine->atDestroy();

			return;
		}
	}
}

void Player::randomTeleport()
{
	bool isValid = true;
	float x, y;
	do
	{
		isValid = true;

		x = (float)rand() / (float)RAND_MAX * screenBorder.halfWidth * 2.f;
		y = (float)rand() / (float)RAND_MAX * screenBorder.halfHeight * 2.f;

		Referential2D newRef = m_referential;
		newRef.m_origin = Vector2D(x, y);

		ConcavePolygon playerGlobal = newRef.concaveToGlobal(m_collider);
		playerGlobal.getAABB();

		for (Mine* mine : entityManager->m_mine)
		{
			ConcavePolygon mineGlobal = mine->m_referential.concaveToGlobal(mine->m_collider);
			mineGlobal.getAABB();

			if (intersect(playerGlobal.getAABB(), mineGlobal.getAABB()))
				isValid = false;
		}
	} while (!isValid);

	m_referential.m_origin = Vector2D(x, y);
}

void Player::move(float deltaTime)
{
	if (IsKeyDown(m_input.m_forward))
		m_acceleration = -m_referential.m_j / m_mass * m_translationSpeed;
	else m_acceleration = Vector2D(0, 0);

	m_speed += m_acceleration * deltaTime;

	// Clamp speed magnitude and y speed
	{
		Vector2D localSpeed = m_referential.vectorGlobalToLocal(m_speed);

		float magnitude = min(localSpeed.magnitude(), m_maxSpeed) * pow(m_friction, deltaTime);

		// TODO: Change sign of y
		localSpeed.y = min(0, localSpeed.y); // Should put max because y is opposed

		m_speed = m_referential.vectorLocalToGlobal(localSpeed.normalized() * magnitude);
	}

	// Rotate the speed vector to let the player drift
	m_speed.rotate((1 - m_kDrift) * angle(m_speed, -m_referential.m_j) * M_PI / 180.f);

	m_referential.m_origin += m_speed * deltaTime;

	stayInScreen();
}

void Player::rotate(float value)
{
	m_referential.rotate(value * m_rotationSpeed);
}

void Player::drawDebug() const
{
	Entity::drawDebug();

	ConcavePolygon polygonGlobal = m_referential.concaveToGlobal(m_collider);

	Rect concaveAABB = polygonGlobal.getAABB();

	concaveAABB.drawDebug();

	for (const ConvexPolygon& polygon : polygonGlobal.polygon)
		polygon.drawDebug();
}

void Player::hurt()
{
	if (--m_life <= 0)
	{
		m_isAlive = false;
		if (std::count_if(entityManager->m_player.begin(),
						  entityManager->m_player.end(),
						  [](const Player& p) 
						  {return p.m_isAlive; }) == 0)
			Game::m_gameState = GameState::GAMEOVER;
	}
}