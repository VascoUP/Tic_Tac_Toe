#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <ctime>
#include <cstdio>

using namespace std;

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

struct jogada
{
	int l;
	int c;
	char letra;
};

void clrscr(void)
{
	COORD coordScreen = { 0, 0 }; // upper left corner
	DWORD cCharsWritten;
	DWORD dwConSize;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hCon, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hCon, &csbi);
	FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hCon, coordScreen);
}

void setcolor(unsigned int color, unsigned int background_color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == BLACK)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | background_color * 16 + color);
}

int cor_peca(char peca)
{
	int cor;
	if (peca == 'X')
		cor = RED;
	else
		cor = LIGHTBLUE;
	return cor;
}

void tabuleiro(vector < vector <char>> jogo, char peca_jogador, char peca_computador)
{
	for (size_t i = 0; i < jogo.size(); i++)
	{
		for (size_t c = 0; c < jogo[i].size(); c++)
		{
			if (jogo[i][c] == peca_computador)
			{
				setcolor(cor_peca(peca_computador), BLACK);
				cout << " " << jogo[i][c];
				setcolor(WHITE, BLACK);
			}
			else if (jogo[i][c] == peca_jogador)
			{
				setcolor(cor_peca(peca_jogador), BLACK);
				cout << " " << jogo[i][c];
				setcolor(WHITE, BLACK);
			}
			else
				cout << " " << jogo[i][c];

			if (c != jogo[i].size() - 1)
			{
				cout << " |";
			}
		}
		cout << endl;
		if (i != jogo.size() - 1)
			cout << "-----------\n";
	}
}

bool coluna(vector < vector <char>> jogo, const int i)
{
	bool acaba = false;
	char a = jogo[0][i], b= jogo[1][i], c = jogo[2][i];
	if (a == b && a == c && a != ' ')
		acaba = true;

	return acaba;
}

bool linha(vector < vector <char>> jogo, const int i)
{
	bool acaba = false;
	char a = jogo[i][0], b = jogo[i][1], c = jogo[i][2];
	if (a == b && a == c && a != ' ')
		acaba = true;

	return acaba;
}

bool diagonal(vector < vector <char>> jogo, const int i)
{
	bool acaba = false;
	if (i == 0)
	{
		char a = jogo[i][i], b = jogo[i + 1][i + 1], c = jogo[i + 2][i + 2];
		if (a == b && a == c && a != ' ')
			acaba = true;
	}
	else if (i == 2)
	{
		char a = jogo[i][i - 2], b = jogo[i - 1][i - 1], c = jogo[i - 2][i];
		if (a == b && a == c && a != ' ')
			acaba = true;
	}

	return acaba;
}

bool tab_cheio(vector < vector <char>> jogo)
{
	bool cheio = true;
	for (size_t i = 0; i < jogo.size(); i++)
	{
		if (jogo[i][0] == ' ' || jogo[i][1] == ' ' || jogo[i][2] == ' ')
			cheio = false;
	}
	return cheio;
}

bool acabou(vector < vector <char>> jogo)
{
	bool fim = false;
	for (size_t l = 0; l < jogo.size(); l++)
	{
		if (coluna(jogo, l))
		{
			fim = true;
			break;
		}
		else if (linha(jogo, l))
		{
			fim = true;
			break;
		}
		else if (diagonal(jogo, l))
		{
			fim = true;
			break;
		}
	}
	
	return fim;
}

bool possivel_peca(vector <vector <char>> jogo, jogada Instrucao)
{
	bool valido = true;
	if (jogo[Instrucao.l][Instrucao.c] != ' ')
		valido = false;
	return valido;
}

bool pode_ganhar(vector <vector <char>> jogo, jogada Instrucao)
{
	bool ganha = false;

	jogo[Instrucao.l][Instrucao.c] = Instrucao.letra;

	if (acabou(jogo))
		ganha = true;

	jogo[Instrucao.l][Instrucao.c] = ' ';

	return ganha;
}

jogada converte_pos(int pos, char peca_jogador)
{
	jogada Inst;
	for (int i = 0; i < 3; i++)
	{
		if (pos <= 3)
		{
			Inst.l = i;
			Inst.c = pos - 1;
			Inst.letra = peca_jogador;
			break;
		}
		pos = pos - 3;
	}
	return Inst;
}

void pede_jogada(vector <vector <char>> &jogo, char peca_jogador, char peca_computador)
{
	int numero;
	cout << "Em que posicao deseja colocar a sua peca? ";
	cin >> numero;
	jogada Inst = converte_pos(numero, peca_jogador);
	while (!possivel_peca(jogo, Inst))
	{
		cout << "Em que posicao deseja colocar a sua peca? ";
		cin >> numero;
		if (numero > 0 && numero < 10)
			Inst = converte_pos(numero, peca_jogador);
	}
	clrscr();

	jogo[Inst.l][Inst.c] = Inst.letra;
	tabuleiro(jogo, peca_jogador, peca_computador);
}

int computador(vector <vector <char>> jogo, char peca_jogador, char peca_computador)
{
	for (unsigned int move = 1; move < 10; move++)
	{
		jogada Inst = converte_pos(move, peca_computador);
		if (possivel_peca(jogo, Inst))
		{	
			if (pode_ganhar(jogo, Inst))
				return move;
		}
	}
	for (unsigned int pos = 1; pos < 10; pos++)
	{
		jogada Inst = converte_pos(pos, peca_jogador);
		if (possivel_peca(jogo, Inst))
		{
			if (pode_ganhar(jogo, Inst))
				return pos;
		}
	}
	int posicao = rand() % 9 + 1;
	jogada Instru = converte_pos(posicao, peca_computador);
	while (!possivel_peca(jogo, Instru))
	{
		posicao = rand() % 9 + 1;
		Instru = converte_pos(posicao, peca_computador);
	}

	return posicao;
}

void joga_computador(vector < vector <char>> &jogo, char peca_jogador, char peca_computador)
{
	int posicao = computador(jogo, peca_jogador, peca_computador);
	jogada Inst = converte_pos(posicao, peca_computador);
	jogo[Inst.l][Inst.c] = Inst.letra;
	clrscr();
	tabuleiro(jogo, peca_jogador, peca_computador);
}

void contra_computador(vector <vector <char>> &jogo)
{
	char peca, peca_comp;
	clrscr();
	cout << "Deseja jogar com o 'X' ou o 'O'? ";
	cin >> peca;

	while (peca != 'X' && peca != 'O')
	{
		cout << "Peca invalida.";
		system("PAUSE");
		clrscr();
		cout << "Deseja jogar com o 'X' ou o 'O'? ";
		cin >> peca;
	}

	clrscr();

	if (peca == 'X')
		peca_comp = 'O';
	else
		peca_comp = 'X';

	tabuleiro(jogo, peca, peca_comp);

	while (!acabou(jogo) && !tab_cheio(jogo))
	{
		if (peca == 'X')
		{
			pede_jogada(jogo, peca, peca_comp);
			if (acabou(jogo))
			{
				cout << "Ganhou o jogo. Parabens!\n";
				break;
			}
			else if (tab_cheio(jogo))
			{
				cout << "O jogo acabou empatado.\n";
				break;
			}
			joga_computador(jogo, peca, peca_comp);
			if (acabou(jogo))
			{
				cout << "O computador ganhou o jogo. Que pena...\nNao te preocupes, fica para a proxima!\n";
				break;
			}
		}
		else
		{
			joga_computador(jogo, peca, peca_comp);
			if (acabou(jogo))
			{
				cout << "O computador ganhou o jogo. Que pena...\nFica para a proxima.\n";
				break;
			}
			else if (tab_cheio(jogo))
			{
				cout << "O jogo acabou empatado.\n";
				break;
			}
			pede_jogada(jogo, peca, peca_comp);
			if (acabou(jogo))
			{
				cout << "Ganhou o jogo. Parabens!\n";
				break;
			}
		}
	}
}

void contra_jogador(vector < vector <char>> &jogo)
{
	char peca1 = 'X', peca2 = 'O';
	clrscr();
	tabuleiro(jogo, peca1, peca2);
	while (!acabou(jogo) && !tab_cheio(jogo))
	{
		cout << "E a vez do jogador 1.\n";
		pede_jogada(jogo, peca1, peca2);
		if (acabou(jogo))
		{
			cout << "O jogador 1 ganhou o jogo.\n";
			break;
		}
		else if (tab_cheio(jogo))
		{
			cout << "O jogo acabou empatado.\n";
			break;
		}
		cout << "E a vez do jogador 2.\n";
		pede_jogada(jogo, peca2, peca1);
		if (acabou(jogo))
		{
			cout << "O jogador 2 ganhou o jogo.\n";
			break;
		}
	}
}

void ecra()
{
	setcolor(RED, BLACK);
	cout << "------------------\n------------------\n";
	cout << "-- JOGO DO GALO --";
	cout << "\n------------------\n------------------\n";
	setcolor(BLACK, BLACK);
	system("PAUSE");
	clrscr();
}

int main()
{
	srand(time(NULL));
	int vs;
	ecra();

	clrscr();
	setcolor(WHITE, BLACK);

	vector < vector <char> > jogo = { 3, vector <char> (3, ' ' ) };

	cout << "Pode jogar contra\n1 - Computador.\n2 - Um amigo.\nInsira opcao: ";
	cin >> vs;

	if (vs == 1)
		contra_computador(jogo);
	else if (vs == 2)
		contra_jogador(jogo);
	else
		cout << "Opcao nao dada. Volte a repetir.";

	return 0;
}