#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include "parser.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass = "Window Debugger Project";
//Collect_Struct collect_struct; 
HWND hCombo;
//=====================================================
/*LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass = "Class";
*/

TEXTMETRIC tm;
SCROLLINFO si;

HWND hStat;
HWND hMonthCal;

/*
// These variables are required to display text. 
static int xClient;     // width of client area 
static int yClient;     // height of client area 
static int xClientMax;  // maximum width of client area 

static int xChar;       // horizontal scrolling unit 
static int yChar;       // vertical scrolling unit 
static int xUpper;      // average width of uppercase letters 

static int xPos;        // current horizontal scrolling position 
static int yPos;        // current vertical scrolling position 

int i;                  // loop counter 
int x, y;               // horizontal and vertical coordinates

int FirstLine;          // first line in the invalidated area 
int LastLine;           // last line in the invalidated area 
HRESULT hr;
int abcLength = 0;  // length of an abc[] item

//int lines = 40;

// Create an array of lines to display. 
static const int LINES = 90; //이는 나중에 값으로 바꾸자. (각 헤더마다 필요한 라인의 수)
*/
static const int LINES = 90;
//=========================================================

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	LPNMHDR lpNmHdr;
	HWND hWndMonthCal = GetDlgItem(hDlg, IDC_MONTHCALENDAR1);//id로부터 핸들값을 얻어옴
	//WSAData d;
	LPARAM lpAdr;
	int byte1, byte2, byte3, byte4; 
	int result_value = print_ipaddress(&byte1, &byte2, &byte3, &byte4); 
	if (result_value == FALSE)
	{
		lpAdr = MAKEIPADDRESS(0, 0, 0, 0);
	}
	else {
		lpAdr = MAKEIPADDRESS(byte1, byte2, byte3, byte4);
	}
	
	HWND hWndIPAddress;
	hWndIPAddress=GetDlgItem(hDlg, IDC_IPADDRESS1);
	BOOL check_ip = SendMessage(hWndIPAddress, IPM_ISBLANK, 0, 0); 

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg, "Help");
		//SendMessage(hWndMonthCal, MCM_SETCOLOR, MCSC_TITLEBK, RGB(205, 50, 5));
		//MonthCal_SetColor(hWndMonthCal, MCSC_TITLEBK, RGB(205, 50, 5));
		return TRUE;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDCANCEL:
		case IDOK:
			EndDialog(hDlg, 0);
			return TRUE;
		case IDC_BUTTON1:
			SendMessage(hWndIPAddress, IPM_SETADDRESS, 0, lpAdr);
			MessageBox(hDlg, "IP Renewal Success!", "System Message", MB_OK);
			return 0;
		}
	/*case WM_NOTIFY:
		lpNmHdr = (LPNMHDR)lParam;
		if (lpNmHdr->code == MCN_SELECT) //날짜가 선택될 때 
		{
			GetSelectedDate(hMonthCal, hStat, hDlg);
		}
		break;
		*/
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//=====================================
	static Collect_Struct collect_struct;
	static Scroll_Bar scroll_bar;
	int return_function_value = 0;

	RECT rc = { 101, -2000, 1000, 2000 };
	int index_combobox = 0;
	char lpstrFile[MAX_PATH] = "";
	int max_len = 256;
	HDC hdc;
	PAINTSTRUCT ps;

	int i = -120; 
	char test[300] = { 0, };
	static char print_string[100][100] = {0, };
 
	//=====================================
	//memset(collect_struct.file_name, 0, 300);
	//=====================================
	switch (iMessage) {
	case WM_CREATE:
		collect_struct.image_section_header = NULL;
		collect_struct.pe_option_header.DataDirectory = NULL;
		collect_struct.pe_option_header64.DataDirectory = NULL;
		collect_struct.fin_parsing = 0;
		make_ComBoBox(hWnd); //selchange하면 띄우기!
		//MessageBox(hWnd, "asd", "asd", MB_OK);
		hdc = GetDC(hWnd);
		///////////////////////////scroll_bar.LINES = 90; 
		//TextOut(hdc, 200, 100, "Good World", strlen("Good World"));
		// Extract font dimensions from the text metrics. 
		GetTextMetrics(hdc, &tm);
		scroll_bar.xChar = tm.tmAveCharWidth;
		scroll_bar.xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * scroll_bar.xChar / 2;
		scroll_bar.yChar = tm.tmHeight + tm.tmExternalLeading;

		// Free the device context. 
		ReleaseDC(hWnd, hdc);

		// Set an arbitrary maximum width for client area. 
		// (xClientMax is the sum of the widths of 48 average 
		// lowercase letters and 12 uppercase letters.) 
		scroll_bar.xClientMax = 48 * scroll_bar.xChar + 12 * scroll_bar.xUpper;

		return 0;
	case WM_VSCROLL:
		// Get all the vertical scroll bar information
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);
		// Save the position for comparison later on
		scroll_bar.yPos = si.nPos;
		switch (LOWORD(wParam))
		{
			// user clicked the HOME keyboard key
		case SB_TOP:
			si.nPos = si.nMin;
			break;

			// user clicked the END keyboard key
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;

			// user clicked the top arrow
		case SB_LINEUP:
			si.nPos -= 1;
			break;

			// user clicked the bottom arrow
		case SB_LINEDOWN:
			si.nPos += 1;
			break;

			// user clicked the scroll bar shaft above the scroll box
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;

			// user clicked the scroll bar shaft below the scroll box
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;

			// user dragged the scroll box
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

			// user positioned the scroll box
			// This message is the one used by Windows Touch
		case SB_THUMBPOSITION:
			si.nPos = HIWORD(wParam);
			break;

		default:
			break;
		}
		// Set the position and then retrieve it.  Due to adjustments
		//   by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hWnd, SB_VERT, &si);
		// If the position has changed, scroll window and update it
		if (si.nPos != scroll_bar.yPos)
		{
			ScrollWindow(hWnd, 0, scroll_bar.yChar * (scroll_bar.yPos - si.nPos), NULL, NULL);
			UpdateWindow(hWnd);
		}
		InvalidateRect(hWnd, &rc, TRUE);
		break;
	case WM_SIZE:
		// Retrieve the dimensions of the client area. 
		scroll_bar.yClient = HIWORD(lParam);
		scroll_bar.xClient = LOWORD(lParam);

		// Set the vertical scrolling range and page size
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = LINES - 1;
		si.nPage = scroll_bar.yClient / scroll_bar.yChar;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		// Set the horizontal scrolling range and page size. 
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + scroll_bar.xClientMax / scroll_bar.xChar;
		si.nPage = scroll_bar.xClient / scroll_bar.xChar;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
		return 0;
	case WM_PAINT:
		//MessageBox(hWnd,"1", "1", MB_OK);
		hdc = BeginPaint(hWnd, &ps);

		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hWnd, SB_VERT, &si);
		scroll_bar.yPos = si.nPos;
		// Get horizontal scroll bar position
		GetScrollInfo(hWnd, SB_HORZ, &si);
		scroll_bar.xPos = si.nPos;
		// Find painting limits
		scroll_bar.FirstLine = max(0, scroll_bar.yPos + ps.rcPaint.top / scroll_bar.yChar);
		/*
		switch (collect_struct.what_paint)
		{
			case 0:
				//scroll_bar.LINES = collect_struct.lines; 
				break; 
			case 1:
				break; 
			case 2:
				break; 
			case 4:
				break;
		}
		*/
		scroll_bar.LastLine = min(LINES - 1, scroll_bar.yPos + ps.rcPaint.bottom / scroll_bar.yChar);////////////////////////////////////////////////////설정 필요 
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetViewportOrgEx(hdc, 200, 150, NULL);
		MoveToEx(hdc, 100, -500, NULL);
		LineTo(hdc, 100, 500); //rect = 101, -2000, 1000, 2000
		
		//MessageBox(hWnd, "2", "1", MB_OK);
		//계속 이 값이 유지 되어야 함. 
		if (collect_struct.what_paint == 0 && collect_struct.fin_parsing == 1)
		{
			parser_to_string_dos_header(print_string, &collect_struct, hWnd);
			TextOut(hdc, 200, -150, "   pFile         RawData         Description", strlen("   pFile         RawData         Description"));
			print_parsing(hdc, hWnd, print_string, &scroll_bar, &collect_struct); 
		}
		else if (collect_struct.what_paint == 1 && collect_struct.fin_parsing == 1)
		{

		}
		else if (collect_struct.what_paint == 2 && collect_struct.fin_parsing == 1)
		{
			//MessageBox(hWnd, "1", "1", MB_OK); 
			parser_to_string_coff_header(print_string, &collect_struct, hWnd);
			//MessageBox(hWnd, "2", "2", MB_OK);
			TextOut(hdc, 200, -150, "   pFile         RawData         Description", strlen("   pFile         RawData         Description"));
			print_parsing(hdc, hWnd, print_string, &scroll_bar, &collect_struct);
		}
		else if (collect_struct.what_paint == 3 && collect_struct.fin_parsing == 1)
		{
			if (collect_struct.binary_bit == 32)
			{
				parser_to_string_pe32_header(print_string, &collect_struct, hWnd);
				TextOut(hdc, 200, -150, "   pFile         RawData         Description", strlen("   pFile         RawData         Description"));
				print_parsing(hdc, hWnd, print_string, &scroll_bar, &collect_struct);
			}
			else if (collect_struct.binary_bit == 64)
			{
				parser_to_string_pe64_header(print_string, &collect_struct, hWnd);
				TextOut(hdc, 200, -150, "   pFile         RawData         Description", strlen("   pFile         RawData         Description"));
				print_parsing(hdc, hWnd, print_string, &scroll_bar, &collect_struct);
			}
		}

		//InvalidateRect(hdc, rc, TRUE); 
		//무효화 영역은 어떤것을 호출하면 하게하자. 
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
			return_function_value = open_file(hWnd, &collect_struct, 256);
			if (return_function_value == -1)
			{
				MessageBox(hWnd, "Failed File Open!", "Error Message", MB_OK | MB_ICONWARNING);
				return 0; 
			}
			else
			{
				return_function_value = setting_parser(hWnd, &collect_struct);
				if (return_function_value == -1)
				{
					return 0; 
				}
				//TextOut(hWnd, 10, 30, collect_struct.file_name, strlen(collect_struct.file_name));
			}
			return 0;
			//file_name에는 파일의 절대경로 나오니 이를 이제 pe_parser에 넣는다.
		case IDC_MONTHCALENDAR1:
			return 0;
		case ID_EXIT_EXIT40005:
			if (MessageBox(hWnd, "프로그램을 종료하시겠습니까?", "Warning", MB_OKCANCEL | MB_ICONWARNING) == IDOK)
			{
				MessageBox(hWnd, "Program is exited!", "Message", MB_OK);
				PostQuitMessage(0);
				return 0;
			}
			else
				break;
		case ID_HELP_HELP:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
			return 0;
			break;
		case ID_COMBOBOX:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				index_combobox = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
				
				if (index_combobox == 0)
				{
					InvalidateRect(hWnd, &rc, TRUE);
					collect_struct.what_paint = 0;
					collect_struct.lines = 31;
				}
				else if (index_combobox == 2)
				{
					InvalidateRect(hWnd, &rc, TRUE);
					collect_struct.what_paint = 2;
					collect_struct.lines = 8; 
				}
				else if (index_combobox == 3)
				{
					InvalidateRect(hWnd, &rc, TRUE);
					collect_struct.what_paint = 3;
					if (collect_struct.binary_bit == 32)
						collect_struct.lines = 30 + collect_struct.pe_option_header.NumberOfRvaAndSizes;
					else if (collect_struct.binary_bit == 64)
						collect_struct.lines = 29 + collect_struct.pe_option_header64.NumberOfRvaAndSizes;
				}
				else if (index_combobox == 4) //sectopn 한번에 보여주자 
				{
					InvalidateRect(hWnd, &rc, TRUE);
					collect_struct.what_paint = 4;
				}
				break;
			}
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
void GetSelectedDate(HWND hMonthCal, HWND hStat, HWND hDlg) {

	SYSTEMTIME time;
	const int dsize = 20;
	wchar_t buf[20];

	ZeroMemory(&time, sizeof(SYSTEMTIME));
	SendMessage(hMonthCal, MCM_GETCURSEL, 0, (LPARAM)& time);

	size_t cbDest = dsize * sizeof(wchar_t);
	StringCbPrintfW(buf, cbDest, L"%d-%d-%d",time.wYear, time.wMonth, time.wDay);

	//MessageBox(hDlg, buf, wcslen(buf), MB_OK); ////에러!!!!!!!!!!!!!!!!!!
	SetWindowTextW(hDlg, buf);
}

void change_char(Collect_Struct* collect_struct)
{
	int i = 0;
	for (i = 0; collect_struct->file_name[i] != '\0'; i++)
	{
		if (collect_struct->file_name[i] == 0x5c)
		{
			collect_struct->file_name[i] = '/';
		}
	}
}
 
void make_ComBoBox(HWND hWnd)
{
	char* items[] = { "IMAGE_DOS_HEADER","MS-DOS Stub Program", "IMAGE_FILE_HEADER", "IMAGE_OPTIONAL_HEADER" };
	int i = 0;
	hCombo = CreateWindow("combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,10, 10, 250, 200, hWnd, (HMENU)ID_COMBOBOX, g_hInst, NULL);
	for (i = 0; i < 5; i++)
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)items[i]);
	return;
}
//섹션의 개수만큼 콤보박스에 추가시킨다.
void add_ComBoBox(HWND hWnd, Collect_Struct* collect_struct)
{
	int add_number = 0;
	for (add_number = 0; add_number < collect_struct->coff_header.NumberOfSections; add_number++)
	{
		//MessageBox(hWnd, collect_struct->image_section_header[add_number].Name, "msg", MB_OK); 
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)collect_struct->image_section_header[add_number].Name);
	}
	return 0;
}
//open file 
int open_file(HWND hWnd, Collect_Struct* collect_struct, int max_len)
{
	//memset(collect_struct.file_name, 0, 300);
	strcpy(collect_struct->file_name,"");
	OPENFILENAME OFN;
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hWnd;
	OFN.lpstrFile = collect_struct->file_name;
	OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
	OFN.nMaxFile = max_len;
	OFN.lpstrInitialDir = "c:\\";
	if (GetOpenFileName(&OFN) != 0)
	{
		MessageBox(hWnd, "Success File Open!", "System Message", MB_OK);
	}
	else
	{
		return -1; 
	}
	//MessageBox(hWnd, OFN.lpstrFile, "hello", MB_OK);
	strcpy(collect_struct->file_name, OFN.lpstrFile);
	return 0;
}