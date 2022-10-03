#pragma once
#ifdef linux
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include <iostream>
#include <thread>
#include <chrono>
#include <array>
using std::array;

enum class FieldType {
	EMPTY = 0,
	PLAYER1 = 1,
	PLAYER2 = 2
};

class Rect {
public:
	SDL_Rect rect;
	Rect(int x, int y, int w, int h) {
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
	}
};

class vierGewinnt {
private:
	/* Constants */
	const char* TITLE = "VierGewinnt";
	const static int WIDTH = 800;
	const static int HEIGHT = 600;
	const static int COLUMNCOUNT = 7;
	const static int ROWCOUNT = 6;

	/* Variables */
	bool quit = false;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Cursor* handCursor;

	int rowSize;
	int columnSize;

	bool mouseDown = 0;

	// 0 = Player 1 & 1 = Player 2
	bool currentPlayer = false;

	array<array<FieldType, COLUMNCOUNT>, ROWCOUNT> fields; // int field[ROW][COL];

	/* Private Functions */
	void HandleInput();
	void HandleEvents();

	/* Checking */
	void CheckEvent(SDL_Event& event);
	bool CheckWinner(FieldType player);

	/* Rendering */
	void RenderLines();
	void RenderPlayers();
	void RenderPresent();

	/* Setting */
	void SetDrawColor(Uint8 r, Uint8 g, Uint8 b);
	void SetCursor(SDL_Cursor* cursor);

	/* Drawing */
	void DrawLine(int x1, int y1, int x2, int y2);
	void FillRect(Rect rect);

	/* Get */
	int GetColumn(int x);
	int GetRow(int y);

	/* Set */
	void SetField(int c, int r, FieldType type);

	void ShowFields();
public:
	/* Variables */
	bool running = true;

	/* Public Functions */
	VierGewinnt();
	void Loop();
	void Clean();
};

/* Private Functions*/
/* Input Handler */
inline void VierGewinnt::HandleInput()
{
	int x, y, column, row;
	Uint32 buttons;

	SDL_PumpEvents();

	buttons = SDL_GetMouseState(&x, &y);

	//SDL_Log("Mouse cursor is at %d, %d", x, y);
	if (((buttons & SDL_BUTTON_LMASK) != 0) && (!mouseDown)) {
		mouseDown = true;
		column = GetColumn(x);
		row = GetRow(y);
		SDL_Log("%i : %i", row, column);

		for (auto i = fields.size() - 1; i >= 0; i--)
		{
			if (fields[i][column] == FieldType::EMPTY)
			{
				if (currentPlayer == false)
				{
					currentPlayer = true;
					fields[i][column] = FieldType::PLAYER1;
				}
				else {
					currentPlayer = false;
					fields[i][column] = FieldType::PLAYER2;
				}
				break;
			}

			if (i <= 0)
				break;
		}

		bool player1 = CheckWinner(FieldType::PLAYER1);
		bool player2 = CheckWinner(FieldType::PLAYER2);

		if (player1 == true) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Winner", "Player1 won", window);
			running = false;
		}

		if (player2 == true) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Winner", "Player2 won", window);
			running = false;
		}

		//fields[row][column] = FieldType::PLAYER1;

		ShowFields();
	}
	else if (buttons == 0) {
		mouseDown = false;
	}
}

/* Event Handler */
inline void VierGewinnt::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		CheckEvent(event);
	}
}

/* Events Checker */
inline void VierGewinnt::CheckEvent(SDL_Event& event)
{
	/* Quit the program */
	if (event.type == SDL_QUIT)
		running = false;
}

inline bool VierGewinnt::CheckWinner(FieldType player)
{
	bool winner = false;

	// Row Check
	for (auto i = 0; i < fields.size(); i++)
		for (int b = 0; b < fields[0].size() - 3; b++)
			if (fields[i][b] == player && fields[i][b + 1] == player && fields[i][b + 2] == player && fields[i][b + 3] == player)
				winner = true;

	// Column Check
	for (auto i = 0; i < fields.size() - 3; i++)
		for (int b = 0; b < fields[0].size(); b++)
			if (fields[i][b] == player && fields[i + 1][b] == player && fields[i + 2][b] == player && fields[i + 3][b] == player)
				winner = true;

	// Right to Left Bottom Check
	for (auto i = 0; i < fields.size() - 3; i++)
		for (int b = 0; b < fields[0].size() - 3; b++)
			if (fields[i][b] == player && fields[i + 1][b + 1] == player && fields[i + 2][b + 2] == player && fields[i + 3][b + 3] == player)
				winner = true;

	// Left To Right Top Check
	for (auto i = 3; i < fields.size(); i++)
		for (int b = 3; b < fields[0].size(); b++)
			if (fields[i][b] == player && fields[i - 1][b - 1] == player && fields[i - 2][b - 2] == player && fields[i - 3][b - 3] == player)
				winner = true;

	return winner;
}

/* Rendering the Rows */
inline void VierGewinnt::RenderLines()
{
	SetDrawColor(255, 255, 255);

	for (int i = 0; i <= ROWCOUNT; i++)
		DrawLine(0, (rowSize * i), WIDTH, (rowSize * i));

	for (int i = 0; i <= COLUMNCOUNT; i++)
		DrawLine((columnSize * i), 0, (columnSize * i), HEIGHT);
}

inline void VierGewinnt::RenderPlayers()
{
	int width, height;

	height = 0;
	for (auto row : fields) {
		width = 0;
		for (auto col : row) {
			if (col == FieldType::PLAYER1)
				SetDrawColor(255, 32, 32);
			if (col == FieldType::PLAYER2)
				SetDrawColor(23, 122, 242);
			if (col == FieldType::EMPTY)
				SetDrawColor(0, 0, 0);

			Rect rect(width + 1, height + 1, columnSize - 1, rowSize - 1);
			FillRect(rect);
			/*SDL_Rect rect;
			rect.x = col + 1;
			rect.y = row + 1;
			rect.w = columnSize - 1;
			rect.h = rowSize - 1;*/

			//SDL_RenderFillRect(renderer, &rect);
			/*DrawLine(col, row, col + columnSize, row + rowSize);
			DrawLine(col + columnSize, row, col, row + rowSize);*/
			width += columnSize;
		}
		height += rowSize;
	}
}

inline void VierGewinnt::RenderPresent()
{
	SDL_RenderPresent(renderer);
}

inline void VierGewinnt::SetDrawColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

inline void VierGewinnt::SetCursor(SDL_Cursor* cursor)
{
	SDL_SetCursor(cursor);
}

inline void VierGewinnt::DrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

inline void VierGewinnt::FillRect(Rect rect)
{
	SDL_RenderFillRect(renderer, &rect.rect);
}

inline int VierGewinnt::GetColumn(int x)
{
	int curColumn = 0, prevColumn;
	int column = -1;

	for (int i = 0; i <= COLUMNCOUNT; i++) {
		prevColumn = curColumn;
		curColumn = columnSize * i;

		if ((x >= prevColumn) && (x <= curColumn)) {
			column = i;
			break;
		}
	}

	return column - 1;
}

inline int VierGewinnt::GetRow(int y)
{
	int curRow = 0, prevRow;
	int row = -1;

	for (int i = 0; i <= ROWCOUNT; i++) {
		prevRow = curRow;
		curRow = rowSize * i;

		if ((y >= prevRow) && (y <= curRow)) {
			row = i;
			break;
		}
	}

	return row - 1;
}

inline void VierGewinnt::SetField(int c, int r, FieldType type)
{
	fields[c][r] = type;
}

/* Public Functions */
/* Constructor */
VierGewinnt::VierGewinnt()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT + 1, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
	handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

	rowSize = HEIGHT / ROWCOUNT;
	columnSize = WIDTH / COLUMNCOUNT;

	// Set Array fields EMPTY
	for (auto& column : fields)
		for (auto& row : column)
			row = FieldType::EMPTY;

	SetCursor(handCursor);
}

/* Game Loop */
inline void VierGewinnt::Loop()
{
	HandleInput();
	HandleEvents();

	RenderLines();
	RenderPlayers();
	RenderPresent();
}

inline void VierGewinnt::ShowFields()
{
	int i = 0;

	std::cout << " 0123456\n";
	for (const auto &col : fields)
	{
		std::cout << i++;
		for (const auto &row : col)
		{
			std::cout << (int)row;
		}
		std::cout << '\n';
	}
}

inline void VierGewinnt::Clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}