#pragma once

#include <vector>

#include <raylib.h>

class Player;

class HUD
{
	private:
		int m_screenWidth = 0;
		int m_screenHeight = 0;

		Texture2D m_bgTexture;
		Texture2D m_fgTexture;

		void drawPlayer1(float fontSize);
		void drawPlayer2(float fontSize);
		void drawPlayer3(float fontSize);
		void drawPlayer4(float fontSize);

	public:
		std::vector<Player>* m_player = nullptr;

		HUD();
		~HUD();

		void drawHUD();
		void drawMainMenu();
		void drawBackground();
		void drawForeground();
		void drawGameOver();
		void drawPauseMenu();

};