#include <iostream>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <fstream>

using namespace std;

enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

void SetColor(ConsoleColor text, ConsoleColor background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

static class Dice {

public:
	static int getDice(int countDice) {
		if (countDice == 1) {
			return rand() % 6 + 1;
		}
		else if (countDice == 2) {
			int dice = (rand() % 6 + 1) + (rand() % 6 + 1);
			if (dice <= 7)
				return dice * 2;
		}
		else if (countDice == 3) {
			int dice = (rand() % 6 + 1) + (rand() % 6 + 1) + (rand() % 6 + 1);
			if (dice <= 7)
				return dice * 3;
		}
		return 0;
	}
};

class Players {

public:
	int getScore() { return this->score; }
	void setScore(int score) { this->score = score; }

	void IincrimentCount() { this->count++; }
	//int getCount() { return this->count; }
	//void setCount(int count) { this->count = count; }
	void writeFile() {
		ofstream out("save.txt", std::ios::app);
		out << "Счёт: " << score
			<< " Кол-во ходов: " << count << endl;
		out.close();
		count = 0;
	}
	virtual void RollADie() = 0;

protected: 
	int score;

private:
	int count = 0;
};

class Player : public Players {

public:
	virtual void RollADie() {
		IincrimentCount();
		system("cls");
		int countDice = 1;
		int key = 0;
		do {
			system("cls");
			key = (key + 4) % 4;
			cout << "Сколько костей кинуть\n";
			if (countDice == 1) draw("Одну кость\n");
			else cout << "Одну кость\n";
			if (countDice == 2) draw("Две кости\n");
			else cout << "Две кости\n";
			if (countDice == 3) draw("Три кости\n");
			else cout << "Три кости\n";
			if (countDice == 4) draw("Выход\n");
			else cout << "Выход\n";

			key = _getch();
			if (key == 0 || key == 224)
				key = _getch();
			if (key == 80 && countDice < 4) countDice++;
			if (key == 72 && countDice > 1) countDice--;
		} while (key != 13);

		system("cls");
		if (countDice == 4) exit(0);
		int rand = Dice::getDice(countDice);
		score += rand;
		SetColor(White, LightGreen);
		cout << "Вам выпало " << rand << endl;
		cout << "Ваш счёт " << score << endl;
		SetColor(White, Black);
	}

private:
	void draw(string str) {
		SetColor(Black, White);
		cout << str;
		SetColor(White, Black);
	}
};

class Enemy : public Players {

public:
	virtual void RollADie() {
		IincrimentCount();
		int Rand = rand() % 33 - 16 + score;
		if (score >= 45) Rand = Dice::getDice(1);
		else if (Rand >= 45) Rand = Dice::getDice(1);
		else if (Rand <= 33) Rand = Dice::getDice(3);
		else Rand = Dice::getDice(2);
		score += Rand;
		SetColor(White, LightRed);
		cout << "Врагу выпало " << Rand << endl;
		cout << "Вражеский счёт " << score << endl;
		SetColor(White, Black);
	}

private:
	int Max(int a, int b) { return a > b ? a : b; }
};

class Game {

private:
	bool isGameStopped;
	Player player;
	Enemy enemy;

public:
	Game() {
		isGameStopped = false;
		player = *new Player();
		enemy = *new Enemy();
	}
	void start() {
		while (!isGameStopped) {
			player.RollADie();
			enemy.RollADie();
			system("pause");
			if (player.getScore() >= 50 && enemy.getScore() >= 50) {
				if (player.getScore() > enemy.getScore())
					youWin();
				else youLose();
				return;
			}
			if (player.getScore() >= 50) {
				youWin();
				return;
			}
			if (enemy.getScore() >= 50) {
				youLose();
				return;
			}
		}
	}

private:
	void youWin() {
		isGameStopped = true;
		system("cls");
		SetColor(White, Green);
		cout << "Ты выйграл!\n\n";
		system("pause");
		SetColor(White, Black);
		restart();
	}
	void youLose() {
		isGameStopped = true;
		system("cls");
		SetColor(White, Red);
		cout << "Ты пройграл!\n\n";
		system("pause");
		SetColor(White, Black);
		restart();
	}
	void restart() {
		if (isGameStopped) {
			isGameStopped = false;
			player.writeFile();
			player.setScore(0);
			enemy.setScore(0);
			
			start();
		}
	}
};

int main() {
	setlocale(LC_ALL, "rus");
	srand(time(NULL));
	SetColor(White, Black);
	cout << "Для управлением игрой исопльзуйте только строчки вверх, вниз и Enter\n\n";
	system("pause");
	Game game = *new Game();
	game.start();
	return 0;
}