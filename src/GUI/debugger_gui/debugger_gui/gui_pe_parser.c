#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include "parser.h"



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass = "Window Debugger Project";

//Collect_Struct collect_struct; 
HWND hCombo;



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
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
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
		return TRUE;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			EndDialog(hDlg, 0);
			return TRUE;
		case IDC_BUTTON1:
			SendMessage(hWndIPAddress, IPM_SETADDRESS, 0, lpAdr);
			MessageBox(hDlg, "IP Renewal Success!", "System Message", MB_OK);
		}
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//=====================================
	static Collect_Struct collect_struct;
	int return_function_value = 0;

	RECT rc = { 101, -2000, 1000, 2000 };
	int index_combobox = 0;
	char lpstrFile[MAX_PATH] = "";
	int max_len = 256;
	HDC hdc;
	PAINTSTRUCT ps;

	char test[300] = { 0, };
	//=====================================
	//memset(collect_struct.file_name, 0, 300);
	//=====================================
	switch (iMessage) {
	case WM_CREATE:
		collect_struct.image_section_header = NULL;
		collect_struct.pe_option_header.DataDirectory = NULL;
		collect_struct.pe_option_header64.DataDirectory = NULL;
		make_ComBoBox(hWnd); //selchange�ϸ� ����!

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetViewportOrgEx(hdc, 200, 150, NULL);
		MoveToEx(hdc, 100, -2000, NULL);
		LineTo(hdc, 100, 2000); //rect = 101, -2000, 1000, 2000
		if (collect_struct.what_paint == 0)
		{
			TextOut(hdc, 200, -130, "Great World", strlen("Great World"));	//���� ��ǥ dos_header
			sprintf(test, "%x", collect_struct.dos_header.e_lfanew);
			TextOut(hdc, 200, -120, test, strlen(test));
			collect_struct.what_paint = -1;
		}
		else if (collect_struct.what_paint == 1) //coff header 
		{
			TextOut(hdc, 200, -120, "Hello World", strlen("Hello World")); 
			collect_struct.what_paint = -1;
		}
		else if (collect_struct.what_paint == 2) //optional header 
		{
			TextOut(hdc, 200, -100, "Good World", strlen("Good World"));
			collect_struct.what_paint = -1;
		}
		else if (collect_struct.what_paint == 3) //section header 1 ~ n 
		{
			TextOut(hdc, 200, -80, "Nice World", strlen("Nice World"));
			collect_struct.what_paint = -1;
		}
		//InvalidateRect(hdc, rc, TRUE); 
		//��ȿȭ ������ ����� ȣ���ϸ� �ϰ�����. 
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
			}
			return 0;
			//file_name���� ������ ������ ������ �̸� ���� pe_parser�� �ִ´�.
		case ID_EXIT_EXIT40005:
			if (MessageBox(hWnd, "���α׷��� �����Ͻðڽ��ϱ�?", "Warning", MB_OKCANCEL | MB_ICONWARNING) == IDOK)
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
				}

				else if (index_combobox == 1)
				{
					InvalidateRect(hWnd, &rc, TRUE);
					collect_struct.what_paint = 1;
				}
				else if (index_combobox == 2)
				{
					InvalidateRect(hWnd, &rc, TRUE);
					collect_struct.what_paint = 2;
				}
				else if (index_combobox == 3) //sectopn �ѹ��� �������� 
				{
					InvalidateRect(hWnd, &rc, TRUE);
					collect_struct.what_paint = 3;
				}
				//SendMessage(hCombo, CB_GETLBTEXT, index_combobox, (LPARAM)text);
				//SetWindowText(hWnd, text); 
				//��� ���� ���⼭ �ϸ� �ɵ� 
				//InvalidateReat�ؼ� �κп����� ��ȿȭ �Ѵ��� PAINT�ؾ��ϴµ�...
				//�� �ϳ� �Ѱܼ� �� paint�Ұ����� �ؾߵǳ� 
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

//�ʱ�ȭ �۾� 
int setting_parser(HWND hWnd, Collect_Struct* collect_struct)
{
	
	if (collect_struct->image_section_header != NULL)
	{
		free(collect_struct->image_section_header);
		collect_struct->image_section_header = NULL;
	}
	if (collect_struct->pe_option_header.DataDirectory != NULL)
	{
		free(collect_struct->pe_option_header.DataDirectory);
		collect_struct->pe_option_header.DataDirectory = NULL;
	}
	else if (collect_struct->pe_option_header64.DataDirectory != NULL)
	{
		free(collect_struct->pe_option_header64.DataDirectory);
		collect_struct->pe_option_header64.DataDirectory = NULL;
	}
	//collect_struct.file_pointer = NULL;
	collect_struct->what_paint = -1;
	//memcpy(collect_struct.file_name, 0, 300); 
	collect_struct->file_offset = 0;
	collect_struct->binary_bit = 0;
	memset(&collect_struct->dos_header, 0, sizeof(collect_struct->dos_header));
	memset(&collect_struct->coff_header, 0, sizeof(collect_struct->coff_header));
	memset(&collect_struct->pe_option_header, 0, sizeof(collect_struct->pe_option_header));
	memset(&collect_struct->pe_option_header64, 0, sizeof(collect_struct->pe_option_header64));
	
	change_char(collect_struct); 

	collect_struct->file_pointer = fopen(collect_struct->file_name, "rb");
	if (collect_struct->file_pointer == NULL)
	{
		char test_file[300] = "";
		sprintf(test_file, "%s", collect_struct->file_name);
		MessageBox(hWnd, test_file, "Error Message", MB_OK); 
		return -1;
	}

	//header�� �Ľ��� ���� section�� �Ľ��Ѵ�. 
	int check = PE_Header_Parser(collect_struct);
	if (check == -1)
	{
		MessageBox(hWnd, "Parsing Error!", "Error Message", MB_OK);
		return -1;
	}
	collect_struct->image_section_header = (IMAGE_Section_Header*)malloc(sizeof(IMAGE_Section_Header)* collect_struct->coff_header.NumberOfSections);

	check = PE_Section_Parser(collect_struct); 
	if (check == -1)
	{
		MessageBox(hWnd, "Section Parsing Error!", "Error Message", MB_OK);
		return -1;
	}
	MessageBox(hWnd, "Parsing Success!", "Success Message", MB_OK);
	return 0;
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