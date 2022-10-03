#include "VierGewinnt.h"

class VierGewinnt : virtual public GameTemplate
{
public:
	//VierGewinnt(const char* Title, int WinWidth, int WinHeight) : GameTemplate(Title, WinWidth, WinHeight) {};
	/* Constructor from GameTemplate maybe replace with other one */
	using GameTemplate::GameTemplate;

	/* What happens if SDL_QUIT */
	void EventQuit()
	{
		Running = false;
	}

	void RenderLines()
	{
		SetDrawColor(255, 255, 255);

		int rowSize = Height / 6;
		int columnSize = Width / 7;

		for (int i = 1; i < 6; i++)
			DrawLine(0, (rowSize * i), Width, (rowSize * i));

		for (int i = 1; i < 7; i++)
			DrawLine((columnSize * i), 0, (columnSize * i), Height);
	}


	/* Place to Render */
	void Render()
	{
		/*SetDrawColor(255, 0, 0);
		DrawLine(0, 0, Width, Height);*/
		RenderLines();
	}
};

int main(int argc, char* argv[])
{
	VierGewinnt vierGewinnt("VierGewinnt", 800, 600);

	while(vierGewinnt.Running)
		vierGewinnt.Run();

	
	return 0;
}