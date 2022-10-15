#include <stdio.h>
#include <time.h> 
#include <stdlib.h> 
//#include <strings.h> 
#include <windows.h> 
//тут есть _getch();
#include <conio.h> 

#define V 21
#define H 75
#define N 100


struct snake
{
	int x, y; // определяем где змейка стоит в начале
	int movX, movY;
	char imagen; //где хранятся изображения змеи, тело
};

struct  fruit
{
	int x, y;
};

struct snake snk[N]; // максимальный размер
struct fruit frt;

void setColor(int ForgC);

void begin(int* size, char map[V][H]);
void intro(char map[V][H]);
void intro_data(char map[V][H], int* size);
void loop(char map[V][H], int size);
void input(char map[V][H], int* size, int* dead);
void update(char map[V][H], int size);
void intro_data2(char map[V][H], int size);

void gotoxy(int x, int y);

void show(char map[V][H]);

void gameRules();
void playGame();


int main()
{
	int choice;
	while (1)
	{
		setColor(4);
		printf("\n\n*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\n\n");
		setColor(4);
		printf("|                    Hello to my Snake!                           |");
		setColor(4);
		printf("\n\n*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\n\n");
		printf("\n1. Game Rules");
		printf("\n2. Play Game");
		printf("\n3. Quit\n");

		scanf_s("%d", &choice);
		switch (choice)
		{

		case 1: gameRules();
			break;

		case 2: playGame();
			break;

		case 3: exit(0);
			break;

		default: printf("Invalid choice");

		}
	}
	return 0;
}

void gameRules()
{
	setColor(4);
	printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	setColor(4);
	printf("|                      Game Rules                          |");
	setColor(4);
	printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

	printf("\n\tPress -> W : Up Movement");
	printf("\n\tPress -> S : Down Movement\n");
	printf("\n\tPress -> A : Left Movement");
	printf("\n\tPress -> D : Right Movement\n");
}

void playGame()
{
	//задаем цвет, очищаем экран консоли
	setColor(4);
	system("cls");
	int size;
	char map[V][H];
	begin(&size, map);
	show(map);
	loop(map, size);
	//это команда которая ждет чтобы мы что то ввели с клавиатуры. так игра не сразу закроется
	system("pause");
	return 0;
}

// создаем изначальное поле
void begin(int* size, char map[V][H])
{
	int i;
	// змеиная голова. генерируем ее в самом центре поля
	snk[0].x = 32;
	snk[0].y = 11; // V

	// змеиное тело по дефолту длины 4
	*size = 4;

	//рандомно заполняем массив
	srand(time(NULL));

	frt.x = rand() % (H - 2) + 1; //автоматическое генерирование фрукта
	frt.y = rand() % (V - 2) + 1;

	for (i = 0; i < *size; i++)
	{
		snk[i].movX = 1;
		snk[i].movY = 0;

	} // змейка двигается только по оси X

	intro(map);
	intro_data(map, size);
}

// Show everything
void show(char map[V][H])
{
	int i, j;

	for (i = 0; i < V; i++)
	{

		for (j = 0; j < H; j++)
		{

			printf("%c", map[i][j]);
		}
		printf("\n");
	}
}

// рисуем всю карту
void intro(char map[V][H])
{
	int i, j;

	for (i = 0; i < V; i++)
	{
		for (j = 0; j < H; j++)
		{
			if (i == 0 || i == V - 1)
			{
				map[i][j] = '~';
			}
			else if (j == 0 || j == H - 1)
			{
				map[i][j] = '|';
			}
			else
			{
				map[i][j] = ' ';
			}
		}
	}
}

// Introduce every data inIT
void intro_data(char map[V][H], int* size)
{
	int i;

	for (i = 1; i < *size; i++)
	{
		snk[i].x = snk[i - 1].x - 1;
		snk[i].y = snk[i - 1].y;
		snk[i].imagen = 'o'; // тело
	}
	snk[0].imagen = 'O'; // голова

	// запускаем змею и делаем ее состоящей из о

	for (i = 0; i < *size; i++)
	{

		map[snk[i].y][snk[i].x] = snk[i].imagen;
	}

	map[frt.y][frt.x] = 'M'; // fruit
}

//игра идет пока значение смерть равно 0
void loop(char map[V][H], int size)
{

	int dead;

	dead = 0;

	do
	{
		gotoxy(0, 0);
		show(map);
		input(map, &size, &dead);
		update(map, size); // automatic

	} while (dead == 0);
}

void input(char map[V][H], int* size, int* dead)
{
	int i;
	char key;

	// Только два способа умереть, столкновение с картой или телом, каждая часть змеи представляет собой различную структуру

	// CHECK GAME CONDITIONS

	if (snk[0].x == 0 || snk[0].x == H - 1 || snk[0].y == 0 || snk[0].y == V - 1)
	{ // 0 -- это голова змеи, мы будем оценивать только при столкновении головы.

		//тогда смерть
		*dead = 1;
	}

	for (i = 1; i < *size && *dead == 0; i++)
	{
		//врезались сами в себя
		if (snk[0].x == snk[i].x && snk[0].y == snk[i].y)
		{
			*dead = 1;
		}
	}

	// CHECK FRUIT, IF HEAD GET CLOSE, EAT IT!

	if (snk[0].x == frt.x && snk[0].y == frt.y)
	{
		*size += 1;
		snk[*size - 1].imagen = 'o';

		// Regenerated fruit in a random position
		frt.x = rand() % (H - 2) + 1;
		frt.y = rand() % (V - 2) + 1;
	}

	// IF DEAD IS ZERO WE CAN KEEP GOING

	if (*dead == 0)
	{
		//проверяем консоль на предмет нажатия клавиши
		if (_kbhit() == 1)
		{
			//считываем 1 символ с клавиатуры
			key = _getch();

			//прописываем логику движения змеи
			if (key == 's' && snk[0].movY != -1)
			{
				snk[0].movX = 0;
				snk[0].movY = 1;
			}

			if (key == 'w' && snk[0].movY != 1)
			{
				snk[0].movX = 0;
				snk[0].movY = -1;
			}

			if (key == 'a' && snk[0].movX != 1)
			{
				snk[0].movX = -1;
				snk[0].movY = 0;
			}

			if (key == 'd' && snk[0].movX != -1)
			{
				snk[0].movX = 1;
				snk[0].movY = 0;
			}
		}
	}
}

void update(char map[V][H], int size)
{
	// Everytime this function is called, the map is overwriting

	intro(map);
	intro_data2(map, size);
}

void intro_data2(char map[V][H], int size)
{
	int i;
	// Body following the head

	for (i = size - 1; i > 0; i--)
	{ // 0 is the head. so we going decresing until extremities

		snk[i].x = snk[i - 1].x;
		snk[i].y = snk[i - 1].y;
	}

	snk[0].x += snk[0].movX; // plus movemenrts
	snk[0].y += snk[0].movY;

	// закрашиваем нужные поля змейкой

	for (i = 0; i < size; i++)
	{
		map[snk[i].y][snk[i].x] = snk[i].imagen;
	}
	//делаем где надо фрукты
	map[frt.y][frt.x] = 'M';
}

void gotoxy(int x, int y)
{ // tbh idk what the hell this function is but this works fine (thought)
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int ForgC)
{
	WORD wColor;

	//We will need this handle to get the current background attribute
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	//We use csbi for the wAttributes word.
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//Mask out all but the background attribute, and add in the forgournd color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
}