#include "parser.h"

void print_parsing(HWND hdc, HWND hWnd, char(*print_string)[100], Scroll_Bar* scroll_bar, Collect_Struct* collect_struct)
{
	//scroll_bar->LastLine; 
	for (int i = scroll_bar->FirstLine; i <= collect_struct->lines; i++)
	{
		scroll_bar->x = scroll_bar->xChar * (1 - scroll_bar->xPos);
		scroll_bar->y = scroll_bar->yChar * (i - scroll_bar->yPos);

	// Note that "55" in the following depends on the
	// maximum size of an abc[] item.

		scroll_bar->abcLength = strlen(print_string[i]);
		scroll_bar->hr = S_OK;
		if ((FAILED(scroll_bar->hr)))
		{
			MessageBox(hWnd, "err", "err", NULL);
		}
		else {
			TextOut(hdc, scroll_bar->x + 200, scroll_bar->y-120, print_string[i], scroll_bar->abcLength);
		}
	}
}
