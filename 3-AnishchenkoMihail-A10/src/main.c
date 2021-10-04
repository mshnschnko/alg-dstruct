#define  _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

void MemoryLeaks(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}

int main(void)
{
	MemoryLeaks();
	int num = 0;
	printf("\t\tWhat do you want work with?\n1.List\n2.Array\n0.EXIT\n\n");
	scanf_s("%i", &num);
	while (num != 0)
	{
		switch (num)
		{
		case 1:
			system("cls");
			MenuList();
			break;
		case 2:
			system("cls");
			MenuArray();
			break;
		case 0:
			break;
		default:
			printf("\nplease choose the option again: ");
			break;
		}
		system("cls");
		printf("\t\tWhat do you want work with?\n1.List\n2.Array\n0.EXIT\n\n");
		scanf_s("%i", &num);
	}
	printf("\n\tGOODBYE!");
	return 0;
}