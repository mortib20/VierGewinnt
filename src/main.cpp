#include "VierGewinnt.h"

class VierGewinnt : public GameTemplate
{
public:
	//VierGewinnt(const char* Title, int WinWidth, int WinHeight) : GameTemplate(Title, WinWidth, WinHeight) {};
	/* Constructor from GameTemplate */
	using GameTemplate::GameTemplate;

	void EventQuit()
	{
		Running = false;
	}

	/* Place to Render */
	void Render()
	{
		SetDrawColor(255, 0, 0);
		DrawLine(0, 0, Width, Height);
	}
};

int main(int argc, char* argv[])
{
	VierGewinnt vierGewinnt("VierGewinnt", 800, 600);

	while(vierGewinnt.Running)
		vierGewinnt.Run();

	
	return 0;
}