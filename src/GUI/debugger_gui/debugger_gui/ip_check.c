#include "parser.h"

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