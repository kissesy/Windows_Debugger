#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

//#include <windows.h>
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include "resource.h"
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include <ws2tcpip.h>
//#include <errno.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass = "Window Debugger Project";
#define ID_COMBOBOX 100


HWND hCombo;
int what_paint = -1; 

void open_file(HWND hWnd, char* file_name, int max_len);
void make_ComBoBox(HWND hWnd);
BOOL print_ipaddress(int* byte1, int* byte2, int* byte3, int* byte4);
void print_addr(PIP_ADAPTER_UNICAST_ADDRESS ua, char* ip_address);
void print_adapter(PIP_ADAPTER_ADDRESSES aa, char* check_adapter);
void text_print(HWND hdc);
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
	
	/*
	if (check_ip == TRUE)
	{
		MessageBox(hDlg, "No connect Internet!", "Message", MB_OK);
	}
	*/
	
	//SendMessage(hWndIPAddress, IPM_CLEARADDRESS, 0, 0); 
	//SendMessage(hWndIPAddress, IPM_GETADDRESS, 0, (LPARAM)&lpAdr);
	
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
	//char str[300];
	//LPARAM lpAdr = MAKEIPADDRESS(212, 56, 198, 92);
	//HWND hWndIPAddress;
	//int i = 0;
	RECT rc = {101, -2000, 1000, 2000};
	int index_combobox = 0;
	char lpstrFile[MAX_PATH]="";
	int max_len = 256;
	char file_name[300] = "";
	HDC hdc;
	PAINTSTRUCT ps;
	char text[300] = {0, }; 
	//=====================================
	switch (iMessage) {
	case WM_CREATE:
		//hdc = GetDC(hWnd);
		make_ComBoBox(hWnd); //selchange하면 띄우기!
		//hWndIPAddress = CreateWindow(WC_IPADDRESS, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP, 300, 300, 120, 20, hWnd, NULL, g_hInst, NULL);
		//SendMessage(hWndIPAddress, IPM_SETADDRESS, 0, lpAdr);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetViewportOrgEx(hdc, 200, 150, NULL);
		MoveToEx(hdc, 100, -2000, NULL);
		LineTo(hdc, 100, 2000); //rect = 101, -2000, 1000, 2000
		if (what_paint == 0)
		{
			TextOut(hdc, 200, -130, "Great World", strlen("Great World"));
			//text_print(hdc);
			what_paint = -1;
		}
		else if (what_paint == 1)
		{
			TextOut(hdc, 200, -120, "Hello World", strlen("Hello World")); 
			//text_print(hdc);
			what_paint = -1;
		}
		else if (what_paint == 2)
		{
			TextOut(hdc, 200, -100, "Good World", strlen("Good World"));
			what_paint = -1;
		}
		//InvalidateRect(hdc, rc, TRUE); 
		//무효화 영역은 어떤것을 호출하면 하게하자. 
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
			open_file(hWnd, file_name, 256);;
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
					what_paint = 0; 
				}

				else if (index_combobox == 1)
				{
					InvalidateRect(hWnd, &rc, TRUE);
					what_paint = 1;
				}
				else if (index_combobox == 2)
				{
					InvalidateRect(hWnd, &rc, TRUE);
					what_paint = 2;
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

void text_print(HWND hdc)
{
	
	what_paint = 0;
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
void open_file(HWND hWnd, char* file_name, int max_len)
{
	OPENFILENAME OFN;
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hWnd;
	OFN.lpstrFile = file_name;
	OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
	OFN.nMaxFile = max_len;
	OFN.lpstrInitialDir = "c:\\";
	if (GetOpenFileName(&OFN) != 0)
	{
		MessageBox(hWnd, file_name, "open!", MB_OK);
	}
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
