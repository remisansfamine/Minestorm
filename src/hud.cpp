#include "hud.h"

#include "utils.h"
#include <string>
#include <numeric>

#include "player.h"

HUD::HUD()
{
	m_bgTexture = LoadTexture("assets/minestorm_background.png");
	m_fgTexture = LoadTexture("assets/minestorm_forground.png");

	Image screenData = GetScreenData();

	m_screenWidth = screenData.width;
	m_screenHeight = screenData.height;
}

HUD::~HUD()
{
	UnloadTexture(m_bgTexture);
	UnloadTexture(m_fgTexture);
}

void HUD::drawBackground()
{
	DrawTexture(m_bgTexture, 0, 0, WHITE);
}

void HUD::drawMainMenu()
{
	float fontSize = 30.f;

	DrawAlignedText("Do you want to play ?", 0.5f, m_screenWidth * 0.5f, m_screenHeight / 4.25f, fontSize, WHITE);

	fontSize = 17.5f;
	DrawAlignedText("Press 'F':                        Solo Mode", 0.5f, m_screenWidth * 0.5f, m_screenHeight / 3.5f, fontSize, LIME);
	DrawAlignedText("Press 'K':             Two players Mode", 0.5f, m_screenWidth * 0.5f, m_screenHeight / 3.25f, fontSize, BLUE);
	DrawAlignedText("Press 'DOWN':    Three players Mode", 0.5f, m_screenWidth * 0.5f, m_screenHeight / 3.f, fontSize, ORANGE);
	DrawAlignedText("Press 'Keypad 5': Four players Mode", 0.5f, m_screenWidth * 0.5f, m_screenHeight / 2.75f, fontSize, PINK);
	DrawAlignedText("Press 'Esc':                   Exit", 0.5f, m_screenWidth * 0.5f, m_screenHeight / 2.5f, fontSize, RED);
}

void HUD::drawPauseMenu()
{
	float fontSize = 30.f;

	DrawAlignedText("PAUSE ||", 0.5f, m_screenWidth * 0.5f, m_screenHeight / 4.25f, fontSize, WHITE);

	fontSize = 17.5f;
	DrawAlignedText("Press 'SPACE':                             Resume", 0.5f, m_screenWidth * 0.5f, m_screenHeight / 3.25f, fontSize, BLUE);
	DrawAlignedText("Press 'Esc' or 'Return':   Back to main menu", 0.5f, m_screenWidth * 0.5f, m_screenHeight / 3.f, fontSize, RED);

}

void HUD::drawForeground()
{
	DrawTexture(m_fgTexture, 0, 0, WHITE);
}

void HUD::drawGameOver()
{
	// Getting the sum of each player score

	int score = std::accumulate(m_player->begin(),
								m_player->end(),
								0, [](int& i, Player& p) { return i + p.m_score; });

	float fontSize = 30.f;
	DrawAlignedText("GAME OVER", 0.5f, m_screenWidth * 0.5f, m_screenHeight / 3.75f, fontSize, WHITE);


	// And display it
	fontSize = 25.f;
	std::string string = stringConcatenateInt("Total Score:", score);
	DrawAlignedText(string.c_str(), 0.5f, m_screenWidth * 0.5f, m_screenHeight / 3.25f, fontSize, GREEN);

	fontSize = 17.5f;
	DrawAlignedText("Press 'Esc' or 'SPACE:      Back to menu", 0.5f, m_screenWidth * 0.5f, m_screenHeight / 2.75f, fontSize, BLUE);

}

void HUD::drawPlayer1(float fontSize)
{
	if (m_player->at(0).m_destroyed)
		return;

	DrawRectangle((m_screenWidth - 150.f) * 0.2f, (m_screenHeight - 80.f) * 0.125f, 50, 20, WHITE);

	for (int i = 0; i < m_player->at(0).m_life; i++)
		DrawCircle(m_screenWidth * 0.2f + 5 * (i * 3 - 4), m_screenHeight * 0.125f, 5, m_player->at(0).m_color);

	// Display score
	{
		std::string infos = stringConcatenateInt("Score:", m_player->at(0).m_score);
		infos += "\nRotate: 'D' and 'G'\n"
			"Move: 'R'\n"
			"Shoot: 'F'\n"
			"Teleport: 'E' or 'T'\n";
		DrawAlignedText(infos.c_str(), 0.2f, m_screenWidth * 0.2f, m_screenHeight / 7.f, fontSize, WHITE);
	}
}

void HUD::drawPlayer2(float fontSize)
{
	if (m_player->at(1).m_destroyed)
		return;

	DrawRectangle((m_screenWidth - 37.5f) * 0.8f, (m_screenHeight - 80.f) * 0.125f, 50, 20, WHITE);

	for (int i = 0; i < m_player->at(1).m_life; i++)
		DrawCircle((m_screenWidth * 0.8f + 5 * (i * 3 - 4)), m_screenHeight * 0.125f, 5, m_player->at(1).m_color);

	// Display keybinds
	{
		std::string infos = stringConcatenateInt("Score:", m_player->at(1).m_score);
		infos += "\nRotate: 'J' and 'L'\n"
			"Move: 'I'\n"
			"Shoot: 'K'\n"
			"Teleport: 'U' or 'O'\n";
		DrawAlignedText(infos.c_str(), 0.8f, m_screenWidth * 0.8f, m_screenHeight / 7.f, fontSize, WHITE);
	}
}

void HUD::drawPlayer3(float fontSize)
{
	if (m_player->at(2).m_destroyed)
		return;

	DrawRectangle((m_screenWidth - 150.f) * 0.2f, m_screenHeight * 0.81f - 10.f, 50, 20, WHITE);

	for (int i = 0; i < m_player->at(2).m_life; i++)
		DrawCircle(m_screenWidth * 0.2f + 5 * (i * 3 - 4), m_screenHeight * 0.81f, 5, m_player->at(2).m_color);

	// Display score
	{
		std::string infos = stringConcatenateInt("Score:", m_player->at(2).m_score);
		infos += "\nRotate: 'RIGHT' and 'LEFT'\n"
			"Move: 'UP'\n"
			"Shoot: 'DOWN'\n"
			"Teleport: 'Ctrl' or 'K0'\n";
		DrawAlignedText(infos.c_str(), 0.2f, m_screenWidth * 0.2f, m_screenHeight / 1.5f, fontSize, WHITE);
	}
}

void HUD::drawPlayer4(float fontSize)
{
	if (m_player->at(3).m_destroyed)
		return;

	DrawRectangle((m_screenWidth - 37.5f) * 0.8f, m_screenHeight * 0.81f - 10.f, 50, 20, WHITE);

	for (int i = 0; i < m_player->at(3).m_life; i++)
		DrawCircle((m_screenWidth * 0.8f + 5 * (i * 3 - 4)), m_screenHeight * 0.81f, 5, m_player->at(3).m_color);

	// Display keybinds
	{
		std::string infos = stringConcatenateInt("Score:", m_player->at(3).m_score);
		infos += "\nRotate: 'K4' and 'K6'\n"
			"Move: 'K8'\n"
			"Shoot: 'K5'\n"
			"Teleport: 'K7' or 'K9'\n";
		DrawAlignedText(infos.c_str(), 0.8f, m_screenWidth * 0.8f, m_screenHeight / 1.5f, fontSize, WHITE);
	}
}

void HUD::drawHUD()
{
	float fontSize = 30.f;

	// Display Score
	{
		int score = std::accumulate(m_player->begin(),
			m_player->end(),
			0, [](int& i, Player& p) { return i + p.m_score; });

		std::string string = stringConcatenateInt("Score:", score);
		const char* scoreChar = string.c_str();
		DrawText(scoreChar, (m_screenWidth - MeasureText(scoreChar, fontSize)) * 0.5f, 715, fontSize, WHITE);
	}

	fontSize = 10.f;

	// Display global keybinds
	{
		std::string keybindString = "Press 'SPACE'\nor 'P' to pause";
		const char* keybindChar = keybindString.c_str();
		DrawText(keybindChar, (m_screenWidth - MeasureText(keybindChar, fontSize)) * 0.875f, 715, fontSize, YELLOW);
	}

	fontSize = 15.f;

	if (!m_player || m_player->size() == 0)
		return;

	// For each player display its informations (score, keybinds...)

	// Player 1
	drawPlayer1(fontSize);

	if (m_player->size() < 2)
		return;

	// Player 2
	drawPlayer2(fontSize);

	if (m_player->size() < 3)
		return;

	// Player 3
	drawPlayer3(fontSize);

	if (m_player->size() < 4)
		return;

	// Player 4
	drawPlayer4(fontSize);
}