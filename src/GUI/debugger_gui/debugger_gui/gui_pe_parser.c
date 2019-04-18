#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include "parser.h"



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass = "Window Debugger Project";

Collect_Struct collect_struct; 
HWND hCombo;



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	collect_struct.image_section_header = NULL; 
	collect_struct.pe_option_header.DataDirectory = NULL;
	collect_struct.pe_option_header64.DataDirectory = NULL;

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
	RECT rc = {101, -2000, 1000, 2000};
	int index_combobox = 0;
	char lpstrFile[MAX_PATH]="";
	int max_len = 256;
	HDC hdc;
	PAINTSTRUCT ps;
	//=====================================
	memset(collect_struct.file_name, 0, 300);
	//=====================================
	switch (iMessage) {
	case WM_CREATE:
		make_ComBoBox(hWnd); //selchange하면 띄우기!

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetViewportOrgEx(hdc, 200, 150, NULL);
		MoveToEx(hdc, 100, -2000, NULL);
		LineTo(hdc, 100, 2000); //rect = 101, -2000, 1000, 2000
		if (collect_struct.what_paint == 0)
		{
			TextOut(hdc, 200, -130, "Great World", strlen("Great World"));	//기준 좌표 dos_header
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
		//무효화 영역은 어떤것을 호출하면 하게하자. 
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
			open_file(hWnd, 256);
			//MessageBox(hWnd, collect_struct.file_name, "hello", MB_OK);
			setting_parser(hWnd);
			return 0;
			//file_name에는 파일의 절대경로 나오니 이를 이제 pe_parser에 넣는다.
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
				else if (index_combobox == 3) //sectopn 한번에 보여주자 
				{
					InvalidateRect(hWnd, &rc, TRUE);
					collect_struct.what_paint = 3;
				}
				//SendMessage(hCombo, CB_GETLBTEXT, index_combobox, (LPARAM)text);
				//SetWindowText(hWnd, text); 
				//명령 실행 여기서 하면 될듯 
				//InvalidateReat해서 부분영역만 무효화 한다음 PAINT해야하는뎀...
				//값 하나 넘겨서 뭘 paint할건지도 해야되네 
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

void change_char()
{
	int i = 0;
	for (i = 0; collect_struct.file_name[i] != '\0'; i++)
	{
		if (collect_struct.file_name[i] == 0x5c)
		{
			collect_struct.file_name[i] = '/'; 
		}
	}
}

//초기화 작업 
int setting_parser(HWND hWnd)
{
	
	if (collect_struct.image_section_header != NULL)
	{
		free(collect_struct.image_section_header); 
		collect_struct.image_section_header = NULL; 
	}
	if (collect_struct.pe_option_header.DataDirectory != NULL)
	{
		free(collect_struct.pe_option_header.DataDirectory); 
		collect_struct.pe_option_header.DataDirectory = NULL; 
	}
	else if (collect_struct.pe_option_header64.DataDirectory != NULL)
	{
		free(collect_struct.pe_option_header64.DataDirectory); 
		collect_struct.pe_option_header64.DataDirectory = NULL; 
	}
	//collect_struct.file_pointer = NULL;
	collect_struct.what_paint = -1;
	//memcpy(collect_struct.file_name, 0, 300); 
	collect_struct.file_offset = 0;
	collect_struct.binary_bit = 0; 
	memset(&collect_struct.dos_header, 0, sizeof(collect_struct.dos_header));
	memset(&collect_struct.coff_header, 0, sizeof(collect_struct.coff_header));
	memset(&collect_struct.pe_option_header, 0, sizeof(collect_struct.pe_option_header));
	memset(&collect_struct.pe_option_header64, 0, sizeof(collect_struct.pe_option_header64));
	
	change_char(); 

	collect_struct.file_pointer = fopen(collect_struct.file_name, "rb");
	if (collect_struct.file_pointer == NULL)
	{
		char test_file[300] = "";
		sprintf(test_file, "%s", collect_struct.file_name);
		MessageBox(hWnd, test_file, "Error Message", MB_OK); 
		return -1;
	}

	//header를 파싱한 다음 section을 파싱한다. 
	int check = PE_Header_Parser();
	if (check == -1)
	{
		MessageBox(hWnd, "Parsing Error!", "Error Message", MB_OK);
		return -1;
	}
	collect_struct.image_section_header = (IMAGE_Section_Header*)malloc(sizeof(IMAGE_Section_Header)* collect_struct.coff_header.NumberOfSections);

	check = PE_Section_Parser(); 
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
void open_file(HWND hWnd, int max_len)
{
	//memset(collect_struct.file_name, 0, 300);
	strcpy(collect_struct.file_name,"");
	OPENFILENAME OFN;
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hWnd;
	OFN.lpstrFile = collect_struct.file_name;
	OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
	OFN.nMaxFile = max_len;
	OFN.lpstrInitialDir = "c:\\";
	if (GetOpenFileName(&OFN) != 0)
	{
		//MessageBox(hWnd, collect_struct.file_name, "dddddllo", MB_OK);
		//MessageBox(hWnd, collect_struct.file_name, "open!", MB_OK);
	}
	//MessageBox(hWnd, OFN.lpstrFile, "hello", MB_OK);
	strcpy(collect_struct.file_name, OFN.lpstrFile);
	return;
}

void print_adapter(PIP_ADAPTER_ADDRESSES aa, char* check_adapter)
{
	//char buf[BUFSIZ];
	memset(check_adapter, 0, BUFSIZ);
	WideCharToMultiByte(CP_ACP, 0, aa->FriendlyName, wcslen(aa->FriendlyName), check_adapter, BUFSIZ, NULL, NULL);
}

void print_addr(PIP_ADAPTER_UNICAST_ADDRESS ua, char* ip_address)
{
	//char buf[BUFSIZ];
	int family = ua->Address.lpSockaddr->sa_family;
	//printf("\t%s ", family == AF_INET ? "IPv4" : "IPv6");
	if (family == AF_INET) //only IPv4
	{
		memset(ip_address, 0, BUFSIZ);
		getnameinfo(ua->Address.lpSockaddr, ua->Address.iSockaddrLength, ip_address, 200, NULL, 0, NI_NUMERICHOST);
		//printf("%s\n", ip_address);
	}
}

BOOL print_ipaddress(int* byte1, int* byte2, int* byte3, int* byte4)
{
	WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d) != 0)
	{
		FALSE; 
	}
	char ip_address[BUFSIZ] = { 0, };
	char check_adapter[BUFSIZ] = { 0, };
	DWORD rv, size;
	PIP_ADAPTER_ADDRESSES adapter_addresses, aa;
	PIP_ADAPTER_UNICAST_ADDRESS ua;

	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &size);
	if (rv != ERROR_BUFFER_OVERFLOW) {
		fprintf(stderr, "GetAdaptersAddresses() failed...");
		return FALSE;
	}
	adapter_addresses = (PIP_ADAPTER_ADDRESSES)malloc(size);

	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapter_addresses, &size);
	if (rv != ERROR_SUCCESS) {
		fprintf(stderr, "GetAdaptersAddresses() failed...");
		free(adapter_addresses);
		return FALSE;
	}

	for (aa = adapter_addresses; aa != NULL; aa = aa->Next) {
		print_adapter(aa, check_adapter);
		if (!strcmp(check_adapter, "Wi-Fi"))
		{
			for (ua = aa->FirstUnicastAddress; ua != NULL; ua = ua->Next) {
				print_addr(ua, ip_address);
			}
		}
		else
			continue;
	}
	//token seperate 
	char* token;
	token = strtok(ip_address, ".");
	*byte1 = atoi(token);
	token = strtok(NULL, ".");
	*byte2 = atoi(token);
	token = strtok(NULL, ".");
	*byte3 = atoi(token);
	token = strtok(NULL, ".");
	*byte4 = atoi(token);
	free(adapter_addresses);
	WSACleanup(); 
	return TRUE;
}
