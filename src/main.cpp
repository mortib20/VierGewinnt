#include "VierGewinnt.h"

int main(int argc, char* argv[])
{
	VierGewinnt vierGewinnt;

	while(vierGewinnt.Running)
		vierGewinnt.Run();

	
	return 0;
}