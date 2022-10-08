#pragma once
#ifdef linux
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

/* Rect Class */
class Rect {
private:
	SDL_Rect rect;
public:
	/* Constructor */
	Rect(int X, int Y, int Width, int Height)
	{
		rect.x = X;
		rect.y = Y;
		rect.w = Width;
		rect.h = Height;
	}

	/* Return Rect */
	SDL_Rect* GetRect()
	{
		return &rect;
	}
};

/* Window Class */
class Window {
protected:
	const int Width;
	const int Height;

	SDL_Window* window;
public:
	/* Constructor */
	Window(const char* Title, int Width, int Height, int Flags = 0) : Width(Width), Height(Height) 
	{
		window = SDL_CreateWindow(Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, Flags);
	};

	/* Deconstructor */
	~Window()
	{
		SDL_DestroyWindow(window);
	};

	/* Return Window */
	SDL_Window* GetWindow()
	{
		return window;
	};
};

/* Renderer Class */
class Renderer {
	SDL_Renderer* renderer;
public:
	/* De/Constructor */
	Renderer(SDL_Window* window, uint32_t flags = 0)
	{
		renderer = SDL_CreateRenderer(window, -1, flags);
	};

	~Renderer()
	{
		SDL_DestroyRenderer(renderer);
	}

	/* Return Renderer */
	SDL_Renderer* GetRenderer()
	{
		return renderer;
	}

	/* Virtual */
	virtual void Render() {
		SetDrawColor(255, 0, 0);
		DrawLine(0, 0, 10, 10);
	}

	/* Color */
	void SetDrawColor(uint8_t r, uint8_t g, uint8_t b)
	{
		SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	}

	/* Drawing */
	void DrawLine(int x1, int y1, int x2, int y2)
	{
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}

	void DrawRect(Rect rect)
	{
		SDL_RenderDrawRect(renderer, rect.GetRect());
	}

	/* Present */
	void Present()
	{
		SetDrawColor(0, 0, 0);
		SDL_RenderClear(renderer);
		Render();
		SDL_RenderPresent(renderer);
	}
};

class Events {
public:
	/* Events */
	virtual void EventQuit()
	{
		SDL_Quit();
	}

	void PollEvent()
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
			if(event.type == SDL_QUIT)
				EventQuit();
	}
};

class GameTemplate : protected Window, protected Renderer, public Events {
private:

public:
	/* Variables */
	bool Running = true;

	/* Constructor */
	GameTemplate(const char* Title, int WinWidth, int WinHeight) : Window(Title, WinWidth, WinHeight), Renderer(GetWindow(), 0)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
	}

	/* Deconstructor */
	~GameTemplate()
	{
		SDL_Quit();
	}

	/* Run */
	void Run()
	{
		PollEvent();
		Present();
	}
};