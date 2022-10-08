#pragma once
#include"SDL.h"

/* SetField Set Enum with Player Empty  */

class VierGewinnt
{
private:
	static const int Width = 800;
	static const int Height = 600;
	static const int ColumnLength = 7;
	static const int RowLength = 6;
	static const int ColumnSize = Width / ColumnLength;
	static const int RowSize = Height / RowLength;

	int CurrentPlayer = 1;

	int MouseX = 0;
	int MouseY = 0;
	int MouseDown = false;

	int GameField[ColumnLength][RowLength] = { 0 };

	SDL_Window* Window = SDL_CreateWindow("VierGewinnt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height + 1, 0);
	SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_PRESENTVSYNC);

	int GetColumn() {
		for (int c = 0; c < ColumnLength; c++)
			if (MouseX >= ColumnSize * c && MouseX <= ColumnSize * (c + 1))
				return c;
	}

	int GetRow() {
		int Column = GetColumn();

		for (int r = RowLength - 1; r > 0; r--)
			if (GameField[Column][r] == 0)
				return r;
	}

	void SetField() {
		int Column = GetColumn();
		int Row = GetRow();

		if (GameField[Column][Row] == 1 || GameField[Column][Row] == 2)
			return;

		if (CurrentPlayer == 1) {
			GameField[Column][Row] = 1;
			CurrentPlayer = 2;
		}
		else {
			GameField[Column][Row] = 2;
			CurrentPlayer = 1;
		}
		SDL_Log("[%i][%i] set", Column, Row);
	}

	void CheckLeftClick() {
		uint32_t buttons;

		SDL_PumpEvents();

		buttons = SDL_GetMouseState(&MouseX, &MouseY);

		if (!MouseDown && (buttons & SDL_BUTTON(1)) != 0) {
			MouseDown = true;
			SDL_Log("Mouse Click at x = %i y = %i", MouseX, MouseY);
			SetField();
			RenderFields();
			RenderPresent();
			CheckWinner(1);
			CheckWinner(2);
		}

		if (buttons == 0)
			MouseDown = false;
	}

	void CheckWinner(int player)
	{
		bool Winner = false;

		// Row Check
		for (auto c = 0; c < ColumnLength; c++)
			for (int r = 0; r < RowLength; r++)
				if (GameField[c][r] == player && GameField[c + 1][r] == player && GameField[c + 2][r] == player && GameField[c + 3][r] == player)
					Winner = true;

		// Column Check
		for (auto c = 0; c < ColumnLength; c++)
			for (int r = 0; r < RowLength; r++)
				if (GameField[c][r] == player && GameField[c][r + 1] == player && GameField[c][r + 2] == player && GameField[c][r + 3] == player)
					Winner = true;

		// Right to Left Bottom Check
		for (auto c = 0; c < ColumnLength; c++)
			for (int r = 0; r < RowLength; r++)
				if (GameField[c][r] == player && GameField[c + 1][r + 1] == player && GameField[c + 2][r + 2] == player && GameField[c + 3][r + 3] == player)
					Winner = true;

		// Left To Right Top Check
		for (auto c = 0; c < ColumnLength; c++)
			for (int r = 0; r < RowLength; r++)
				if (GameField[c][r] == player && GameField[c - 1][r + 1] == player && GameField[c - 2][r + 2] == player && GameField[c - 3][r + 3] == player)
					Winner = true;

		if (Winner) {
			if (player == 1)
				SDL_ShowSimpleMessageBox(0, "Winner!", "Player 1 won the game!", Window);
			if (player == 2)
				SDL_ShowSimpleMessageBox(0, "Winner!", "Player 2 won the game!", Window);
			EndGame();
		}
	}

	void CheckQuit() {
		SDL_Event event;

		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				Running = false;
	}

	void Input() {
		CheckQuit();
		CheckLeftClick();
	}

	void EndGame() {
		Running = false;
	}

	void RenderFields() {
		for (int c = 0; c < ColumnLength; c++) {
			for (int r = 0; r < RowSize; r++) {
				if (GameField[c][r] == 0)
					DrawColor(0, 0, 0);

				if (GameField[c][r] == 1)
					DrawColor(0, 255, 0);

				if (GameField[c][r] == 2)
					DrawColor(0, 0, 255);

				FillRect(RowSize - 1, ColumnSize - 1, (ColumnSize * c) + 1, (RowSize * r) + 1);
			}
		}
	}

	void RenderLines() {
		DrawColor(255, 255, 255);

		for (int c = 0; c <= ColumnLength; c++)
			DrawLine(ColumnSize * c, 0, ColumnSize * c, Height);

		for (int r = 0; r <= RowLength; r++)
			DrawLine(0, RowSize * r, Width, RowSize * r);
	}

	void RenderPresent() {
		SDL_RenderPresent(Renderer);
	}

	void DrawLine(int x1, int y1, int x2, int y2) {
		SDL_RenderDrawLine(Renderer, x1, y1, x2, y2);
	}

	void FillRect(int h, int w, int x, int y) {
		SDL_Rect Rect;
		Rect.h = h;
		Rect.w = w;
		Rect.x = x;
		Rect.y = y;

		SDL_RenderFillRect(Renderer, &Rect);
	}

	void DrawColor(uint8_t r, uint8_t g, uint8_t b) {
		SDL_SetRenderDrawColor(Renderer, r, g, b, SDL_ALPHA_OPAQUE);
	}
public:
	bool Running = true;

	void Run() {
		Input();
		RenderLines();
		RenderPresent();
	}
};