#include<iostream>
#include<conio.h>
#include<string>
#include<Windows.h>
#include<ctime>
#include<fstream>

using namespace std;

//global variables
const int rows = 25;
const int cols = 60;
char grid[rows][cols];
char chicken = '&';
char spaceShip = '$';
char shipFire = '*';
char egg = '0';
int spaceShipPosX = rows - 1;
int spaceShipPosY = cols / 2;
bool isRight = true;
bool isGameRunning = true;
string playerName = " ";
int _scores = 0;
int level = 1;
int lives = 3;

//function prototypes
void saveScore(string, int);
void readScore();
void displayScore();
void gotoxy(int x, int y);											
void intro();
void menu();
void masterFunc(char);
void initiGrid();
void shipController(char);
void fire();
void moveFire();
void displayGrid();
int moveChickens();
bool moveRight(int);
bool moveLeft(int);
void moveDown();
void eggDrop();
void eggMove();
void checkCollisions();
void collisionDetected();
void levelUp();
void gameCompleted();
void gameOver();
void startGame();
void ShowConsoleCursor(bool);

//main func
int main()
{
	srand(time(0));
	intro();
}

//save score (definition)
void saveScore(string str, int score)
{
	ofstream fout;
	fout.open("scores.txt", ios::app);
	if (!fout) {
		cerr << "Error opening file!" << endl;
		exit(0);
	}
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == ' ')
			str[i] = '-';
	}
	fout << str << " " << score << endl;
	fout.close();
}

void displayScore()
{
	string line;
    ifstream fout("scores.txt");
	if (fout.is_open())
	{
		while (getline(fout, line))
		{
			for (int i = 0; line[i] != '\0'; i++)
			{
				if (line[i] == '-')
					line[i] = ' ';
			}
			cout << line << endl;
		}
	}
	else
		cout << "file opening error";
    fout.close();
}

void readScore()
{
	string name, line;
	int score;
	ifstream fout("scores.txt");
	if (fout.is_open())
	{
		while (getline(fout,line))
		{
			fout >> name >> score;
			cout << name << " " << score;
			cout << endl;
		}
	}
	else
		cout << "file opening error";
	fout.close();
	_getch();
}

//intro func (definition)
void intro()
{
	cout << "\n\n\n\n\n\n\t\t\t\t Welcome to Chicken Invaders" << endl;
	cout << "\t\t\t\t\t\tLoading";
	for (int i = 0; i < 5; i++)
	{
		cout << ".";
		Sleep(50);
	}
	cout << "Completed";
	Sleep(50);
	system("cls");
	cout << "\n\n\t\t\t\tEnter player name : ";
	getline(cin, playerName);
	ShowConsoleCursor(false);
	menu();
}

//menu func (definition)
void menu()
{
	char userInput;
	bool flag = true;
	cout << endl;
	cout << " 1. Start Game" << endl;
	cout << " 2. Score Board" << endl;
	cout << " 3. Want to Learn" << endl;
	cout << " 4. Exit " << endl;
	cout << "\n Enter your desired option: ";
	cin >> userInput;
	while (flag)
	{
		if (cin.fail() || userInput != '1' && userInput != '2' && userInput != '3' &&  userInput != '4')
		{
			cout << "Enter the correct option(1, 2, 3 or 4) : ";
			cin >> userInput;
		}
		else
			flag = false;
	}
	system("cls");
	masterFunc(userInput);
}

//controls func (definition)
void masterFunc(char userInput)
{
	switch (userInput)
	{
	case '1':
		startGame();
		break;
	case '2':
		cout << "Coming soon ";
		break;
	case '3':
		cout << "Coming soon ";
		break;
	case '4':
		cout << "Coming soon ";
		break;

	}
}

void startGame() {
	system("cls");
	initiGrid();
	while (isGameRunning)
	{
		checkCollisions();
		displayGrid();
		moveFire();
		eggDrop();
		eggMove();
		if (moveChickens() == 0)
		{
			if (level < 3)
				levelUp();
			else
				gameCompleted();
		}

		if (_kbhit())
		{
			char ch = _getch();
			switch (ch)
			{
			case 'A':
			case 'a':
			case 'D':
			case 'd':
			case 'W':
			case 'w':
			case 'S':
			case 's':
				shipController(ch);
				break;

			case ' ':
				fire();
				break;

			case 'O':
			case 'o':
				break;
			}

		}
		gotoxy(0, 0);
	}
}

//initiGrid func (definition)
void initiGrid()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i][j] = ' ';
		}
	}
	
	switch (level)
	{
	case 1:
		for (int i = 0; i < level; i++)
		{
			for (int j = 0; j < 40; j += 4)
				grid[i][j] = chicken;
		}
		break;
	case 2:
		for (int i = 0; i < level + 1; i += 2)
		{
			for (int j = 0; j < 40; j += 4)
				grid[i][j] = chicken;
		}
		break;
	case 3:
		for (int i = 0; i < level + 2; i += 2)
		{
			for (int j = 0; j < 40; j += 4)
				grid[i][j] = chicken;
		}
	}
	grid[spaceShipPosX][spaceShipPosY] = spaceShip;
}

//displayGrid func (definition)
void displayGrid()
{
	for (int i = 0; i < rows; i++)
	{
		//cout << "||";
		for (int j = 0; j < cols; j++)
		{
			cout << grid[i][j];
		}
		cout << "|";
		cout << endl;
	} 
	for (int i = 0; i < cols; i++)
		cout << "_";
	cout << '|' << endl << endl;
	cout << " Player name :" << ::playerName << endl;
	cout << " Scores : " << _scores << "\tLevel : " << level << "\tLives : " << lives;
}

//ship controller func (definition)
void shipController(char ch)
{
	grid[spaceShipPosX][spaceShipPosY] = ' ';
	switch (ch)
	{
	default:
		break;
	case 'A':
	case 'a':
		if (spaceShipPosY > 0)
			spaceShipPosY--;
		break;
	case 'D':
	case 'd':
		if (spaceShipPosY < cols - 1)
			spaceShipPosY++;
		break;
	case 'W':
	case 'w':
		if (spaceShipPosX > 0)
			spaceShipPosX--;
		break;
	case 'S':
	case 's':
		if (spaceShipPosX < rows - 1)
			spaceShipPosX++;
		break;
	}
	grid[spaceShipPosX][spaceShipPosY] = spaceShip;
}

//move chickens func (definition)
int moveChickens()
{
	int hasChicken = 0;
	for (int i = 0; i < rows; i++)
	{
		if (isRight)
		{
			if (grid[i][cols - 1] == chicken)
			{
				isRight = false;
				moveDown();
				return 1;
			}
			if(moveRight(i))
				hasChicken++;
		}
		if (!isRight)
		{
			if (grid[i][0] == chicken)
			{
				isRight = true;
				moveDown();
				return 1;
			}
			if(moveLeft(i))
				hasChicken++;
		}
	}
	return hasChicken;
}

//move Right func (definition)
bool moveRight(int i)
{
	bool hasChicken = false;
	for (int j = cols - 2; j >= 0; j--)
	{
		if (grid[i][j] == chicken)
		{
			hasChicken = true;
			grid[i][j + 1] = grid[i][j];
			grid[i][j] = ' ';
		}
	}
	return hasChicken;
}

//move Left func (definition)
bool moveLeft(int i)
{
	bool hasChicken = false;
	for (int j = 0; j < cols; j++)
	{
		if (grid[i][j] == chicken)
		{
			hasChicken = true;
			grid[i][j - 1] = grid[i][j];
			grid[i][j] = ' ';
		}
	}
	return hasChicken;
}

//move down func (definition)
void moveDown()
{
	//check chicken if reach the last row
	if (grid[rows - 1][0] == chicken || grid[rows - 1][cols - 1] == chicken)
	{
		gameOver();
		return;
	}
	for (int i = rows - 1; i > 0; i--)
	{
		for (int j = 0; j < cols; j++)
		{
			//updating row
			if (grid[i - 1][j] == chicken)
			{
				grid[i][j] = grid[i - 1][j];
				grid[i - 1][j] = ' ';
			}
		}
	}
}

//egg drop func (definition)
void eggDrop()
{
	int divisors[] = {0, 73, 57, 43};
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == chicken)
			{
				if (rand() % divisors[level] == 0)
					grid[++i][j] = egg;
			}
		}
	}
}

//egg move func (definition)
void eggMove()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == egg)
			{
				grid[i][j] = ' ';

				if (++i == rows)
					continue;

				if (grid[i][j] == spaceShip) {
					lives--;
					continue;
				}

				while (i < rows && grid[i][j] != ' ')
				{
					i++;
				}
				if (i < rows)
					grid[i][j] = egg;
			}
		}
	}
}

//fire func (definition)
void fire()
{
	grid[spaceShipPosX - 1][spaceShipPosY] = shipFire;
}

//move fire func {definition)
void moveFire()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == shipFire)
			{
				grid[i][j] = ' ';
				if (grid[i - 1][j] == egg)
					i--;
				if (i != 0)
				{
					if (grid[i - 1][j] == chicken)
						_scores += 10;
					grid[i-1][j] = shipFire;
				}
			}
		}
	}
}

//chicken collision func (definition)
void checkCollisions()
{
	//chicken collision
	if (spaceShipPosY > 0 && grid[spaceShipPosX][spaceShipPosY - 1] == chicken)
	{
		Sleep(1000);
		grid[spaceShipPosX][spaceShipPosY - 1] = ' ';
		collisionDetected();
	}
	else if (spaceShipPosY < (cols-1) && grid[spaceShipPosX][spaceShipPosY + 1] == chicken)
	{
		Sleep(1000);
		grid[spaceShipPosX][spaceShipPosY + 1] = ' ';
		collisionDetected();
	}
}

//check collision func (definition)
void collisionDetected()
{
	lives--;
	if (lives == 0)
		gameOver();
}

//level up func (definition)
void levelUp()
{
	system("cls");
	cout << "\n\n\n\t\t\t\t\tLEVEL UP!" << endl;
	cout << "\t\t\t\t\tWorld " << level << " has been completed" << endl;
	cout << "\t\t\t\t\tWorld " << level + 1 <<" is loading";
	for (int i = 0; i < 6; i++)
	{
		cout << ".";
		Sleep(500);
	}
	system("cls");
	level++;
	initiGrid();
}

//game completed func (definition)
void gameCompleted()
{
	system("cls");
	cout << "\n\n\n\t\t\t\t\t-*-*-*-THE END-*-*-*-" << endl;
	cout << "\t\t\t\t\tYou have completed the whole World!" << endl;
	cout << "\t\t\t\t\tYour score is : " << _scores << endl;
	_getch();
}

//game over func (definition)
void gameOver()
{
	system("cls");
	isGameRunning = false;
	cout << "\n\n\n\t\t\t\t\tGAME OVER!" << endl;
	cout << "\t\t\t\t\tYou have lost all your lives!" << endl;
	cout << "\t\t\t\t\tYour score is : " << _scores << endl;
	_getch();
}

//gotoxy func (definition)
void gotoxy(int x, int y)
{
	COORD c = { (short)x, (short)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
