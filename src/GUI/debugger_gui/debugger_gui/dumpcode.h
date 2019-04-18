#pragma once
#ifndef __H_DUMP_
#define __H_DUMP_

#include <stdio.h>
#include <string.h>
#include <ctype.h>

void printchar(unsigned char c)
{
	if (isprint(c))
		printf("%c", c);		// �ش��ϴ� ���� ���ڷ� ǥ��
	else
		printf(".");			// ��� �Ұ����� ���ڴ� �׳�
	// .���� ǥ��
}

// �޸� ���� �ּҿ� ����� ũ�⸦ ���ڷ� ����
void dumpcode(unsigned char* buff, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		// 16����Ʈ ������ �ּ� ���
		if (i % 16 == 0)
			printf("0x%08x  ", &buff[i]);

		// hex �� ���
		printf("%02x ", buff[i]);

		// �ش� 16�������� ���� ���ڷ� ���
		if (i % 16 - 15 == 0)
		{
			int j;
			printf("  ");
			for (j = i - 15; j <= i; j++)
				printchar(buff[j]);
			printf("\n");
		}
	}

	// ������ ������ 16����Ʈ ������ ��� ���� ����
	if (i % 16 != 0)
	{
		int j;
		int spaces = (len - i + 16 - i % 16) * 3 + 2;
		for (j = 0; j < spaces; j++)
			printf(" ");		// ������ ������ŭ space�� �̵���
		// ��,
		for (j = i - i % 16; j < len; j++)
			printchar(buff[j]);	// ���� ���� ���� ���
	}
	printf("\n");
}

void dumpdepth(unsigned short int* data, int num)
{
	int i;

	for (i = 0; i < num; i++)
	{
		printf("0x%X ", data[i]);

		if (i % 16 == 15)
			printf("\n");
	}
	printf("\n");
}

#endif 
