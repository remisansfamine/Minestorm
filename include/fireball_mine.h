#pragma once

#include "mine.h"

class FireballMine : public Mine
{
	private:
		void createCollider(float size = 1.f) override;

	public:
		FireballMine(SpawnPoint* sp = nullptr, int size = 2);

		void atDestroy() override;
};