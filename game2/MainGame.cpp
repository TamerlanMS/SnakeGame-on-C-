#include <iostream>
#include <string>       // ��� ����� std::string
#include <time.h>
#include <conio.h>      // ��� _getch(), _kbhit()
#include "Win10.h"
#include <ctime>
#include <iomanip> 
#include <fstream>
using namespace std;
#define RAND(min,max)  (rand()%((max)-(min)+1)+(min))
#define SIZE 5

// ��������� ��� �������� ���������� ������, ����������
//  struct Coord - ���������������� ��� ������
struct Coord
{
    int row, col; // ���������� ��� ���������
};

//
// ������� ���� - ������ ����� ���� string
// ����� ����� ���� (Canvas)
// �������: & - ������� �����; 1 - ����������;
//          # - �����; 1,2,3,4 - ������;
//          * - �����;
//          ? - �����, ������� �� ��������� �������;
//


string Pole[]   // <==> char pole[15][35+1];
{
  "####################################",// +'\0'
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "##                           &o   ##",
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "####################################"
};

// ������ � ������ �������� ����
int PoleHeight;   // = 15
int PoleWidth;    // = 35

// �������� ���������� ������ ���� �� �����
int PoleRow = 3; // �� ������
int PoleCol = 8; // �� �������
Coord tail[13 * 37];
int cnttail = 1;
int Score = 0;
int lives;
void Menu();
int SettingsMenu();
void GameOver();
void About();
// ��������� �������
void StartGame();     // �-��� ��� ������� ����� ����
//GetCoordHero();       // ������� ��� ��������� ���������� �������� ������
//GetCoordEnemys();     // ������� ��� ��������� ��������� �����������
int DrawPole(int key);      // ������� ������ �������� ���� �� �����
int Menu(string* Items, int cntItems,
    int Row, int Col, int Select);
int SettingsMen(string* Items, int cntItems,
    int Row, int Col, int Select);
void Spawn();
int SdvigHvosta(Coord* tail, int cnttail, Coord Head);
void PrintHvost(Coord* tail, int cnttail);
void Win();
// ������ �������� ���� ���������
// ������ �����
string MainMenu[]
{
  "Start New Game",
  "Settings",
  "About",
  "Exit"
};
// ���������� ������� ������� MainMenu
const int cntMainMenu =
sizeof(MainMenu) / sizeof(MainMenu[0]);

string Settings[]
{
  "Easy (2 Lives, 1x ScorePoints, Speed: low)",
  "Medium (1 Lives, 2x ScorePoints, Speed: medium)",
  "Hard (Extra lives for the weak, 3x ScorePoints, Speed: Flash) ",
};
// ���������� ������� ������� MainMenu
const int cntSetting =
sizeof(Settings) / sizeof(Settings[0]);
int dif = 0;


int main()
{
    //system("chcp 1251 > nul"); //setlocale(LC_ALL, "RUSSIAN");
    //system("chcp 866 > nul"); //setlocale(LC_ALL, "C");
    // ���������� ������ �������� ����:
    //   ������ ����� ���� / ������ ����� ������
    Menu();

    cin.get(); cin.get();
} // main()

int SettingsMenu() { // ������� �������� ��� ������ ���������
    int  Select = 0;
    int  menuRow = 3, menuCol = 12;

    do {
        // ������� ���� ����
        if (Select == -1) { Select = 0; }
        Select = SettingsMen(Settings, cntSetting,
            menuRow, menuCol, Select);

        switch (Select)
        {
        case 0: // "Easy"
            dif = 0;
            CLS;
            Menu();
            break;
        case 1: // "Medium"
            dif = 1;
            CLS;
            Menu();
            break;
        case 2: // "Hard"
            dif = 2;
            CLS;
            Menu();
            break;
            // switch(Select)
        case -1: // ESC
            CLS;
            Menu();
            break;
        }
    } while (Select != 0);
    return dif;
}

void Menu() {
    PoleHeight = sizeof(Pole) / sizeof(Pole[0]);
    PoleWidth = Pole[0].size(); // ������ ������ ����

    bool isExit = false; // ������ ���������� ������
    int  Select = 0;
    int  menuRow = 3, menuCol = 12;

    do {
        // ������� ���� ����
        if (Select == -1) { Select = 0; }
        Select = Menu(MainMenu, cntMainMenu,
            menuRow, menuCol, Select);

        switch (Select)
        {
        case 0: // "Start New Game"
            CLS;
            StartGame();     // �-��� ��� ������� ����� ����
            break;
        case 1: // "Settings"
            CLS;
            SettingsMenu();      // ���������
            break;
        case 2: // "About"
            CLS;
            About();         // �� �������
            break;
        case 3: // "Exit"
        case -1: // ESC
        {
            // ���� ������� ������ �� ����� �� ���������
            SETLOCAL_RU; // ��� ������ ��������� 1251
            SetPos(menuRow + cntMainMenu + 1, menuCol);
            cout << "������������ �� �� ������ �����"
                " �� ���������(y/n): ";
            SETLOCAL_EN; // ��� ������ ������������� 866
            char key = _getche();
            if (key == 'y' || key == 'Y') {
                isExit = true; // ������ �� ���������� ������ ���������
                SetPos(menuRow + cntMainMenu + 2, menuCol);
                cout << "Good bye...";
            }
            else {
                CLS; // ������� ������
            }
        }
        break;
        } // switch(Select)

    } while (isExit == false);

}


// SearchPlayer(Pole, poleHeight, '*');
Coord SearchPlayer(string* pole, int height, char sym)
{
    Coord result{ -1,-1 };// ���������� ���� Coord ��� �������� ���������
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < pole[i].size(); j++) {
            if (pole[i][j] == sym) {
                result.row = i; // ������ ������
                result.col = j; // ������ �������
                return result; // ���������� ���������� � �����������
            }
        }
    }
    //result.row = result.col = -1;
    return result;
}

// ������� ������ ����� ����
void StartGame()
{
    Spawn();

    // �������� ������� ������� ������
    Coord pl = SearchPlayer(Pole, PoleHeight, '&'); // '&'

    //Coord tail = SearchPlayer(Pole, PoleHeight, 'o');
    tail[0].row = pl.row;
    tail[0].col = pl.col + 1;
    tail[1].row = pl.row;
    tail[1].col = pl.col + 1;

    int key = 0;
    //Coord en = SearchEnemy(Pole, PoleHeight);
    int frams = 0; // ������� ������
    double speed; // ��������
    const int max = 481;
    int last_step = 0; // ��������� �������� ��� �����������
    int t = 0, ScorePoint;
    bool gameover = false;
    switch (dif) {
    case 0:
        lives = 2;
        ScorePoint = 10;
        speed = 100;
        break;
    case 1:
        lives = 1;
        ScorePoint = 20;
        speed = 75;
        break;
    case 2:
        lives = 0;
        ScorePoint = 30;
        speed = 50;
        break;
    }
    do // ���� ���� ���� ����, ��������� ���� �� ���������� ����
    {
        if (_kbhit()) // 1) ��������� ������� ������
        {
            key = _getch(); // 2) �������� ��� ������� �������
        }
        // ����������� ������� ������� - ������
        if (int(frams) != last_step)
        {
            last_step = int(frams);
            // 3) �������� ������� ������, ���� �� ���������
            Coord old = pl; // ���������� ������ ����������
            switch (key)
            {
            case _KEY::UP:    // �����
                if (Pole[pl.row - 1][pl.col] == '#' || Pole[pl.row - 1][pl.col] == 'o') {
                    lives--;
                    key = 0;
                }
                else {
                    if (Pole[pl.row - 1][pl.col] == '0') {
                        Spawn();
                        Score += ScorePoint;
                        cnttail++;
                    }
                    pl.row--;
                    Sleep(100);
                }
                break;
            case _KEY::DOWN:  // ����
                if (Pole[pl.row + 1][pl.col] == '#' || Pole[pl.row + 1][pl.col] == 'o') {
                    lives--;
                    key = 0;
                }
                else {
                    if (Pole[pl.row + 1][pl.col] == '0') {
                        Spawn();
                        Score += ScorePoint;
                        cnttail++;
                    }
                    pl.row++;
                    Sleep(100);
                }
                break;
                //SdvigHvosta(key, cnttail);
                break;
            case _KEY::LEFT:  // �����
                if (Pole[pl.row][pl.col - 1] == '#' || Pole[pl.row][pl.col - 1] == 'o') {
                    lives--;
                    key = 0;
                }
                else {
                    if (Pole[pl.row][pl.col - 1] == '0') {
                        Spawn();
                        Score += ScorePoint;
                        cnttail++;
                    }
                    pl.col--;
                }
                break;
            case _KEY::RIGHT: // ������
                if (Pole[pl.row][pl.col + 1] == '#' || Pole[pl.row][pl.col + 1] == 'o') {
                    lives--;
                    key = 0;
                }
                else {
                    if (Pole[pl.row][pl.col + 1] == '0') {
                        Spawn();
                        Score += ScorePoint;
                        cnttail++;
                    }
                    pl.col++;
                }
                break;
                // switch (key)
            }
            if (old.row != pl.row || old.col != pl.col) {
                SdvigHvosta(tail, cnttail, old);
            }
            // ������������ ������
            Pole[old.row][old.col] = ' '; // ������� �� ������ �������
            Pole[pl.row][pl.col] = '&'; // ����� � ����� �������
            PrintHvost(tail, cnttail);

            srand(time(NULL));
            t++;
            SetPos(5, PoleWidth + 15);
            cout << "Time: " << t / 10;
            SetPos(7, PoleWidth + 14);
            cout << "Score: " << Score;
            SetPos(9, PoleWidth + 14);
            cout << "Lives: " << lives;

        } //
        //} // if( _kbhit() )

        // 4) �������� �����������
        //Pole[oldEnemy.row][oldEnemy.col] = point; // ������� �� ������ �������
        //Pole[enemy.row][enemy.col] = '1'; // ����� � ����� �������

        // 5) ������������� ���� �� ������
        DrawPole(key); // ����� ���� �� �����
        // 6) �������� �� ����������: ������ ��� ���������
        Sleep(speed); // �������� ���������
        frams++; // ������� ������

        if (lives < 0) { // ������� ���������
            GameOver();
            cin.get(); cin.get();
            gameover = false;
        }
        if (cnttail == max) { // ������� ������
            Win();
            cin.get(); cin.get();
            gameover = false;
        }
    } while (gameover == false);
} // StartGame()

void Win() {
    CLS;
    SetPos(13, PoleWidth + 14);
    cout << "    You WIN!";
    SetPos(14, PoleWidth + 12);
    cout << "Your Total SCORE: " << Score;
}

void GameOver() {
    CLS;
    SetPos(13, PoleWidth + 14);
    cout << "  GAME OVER!";
    SetPos(14, PoleWidth + 12);
    cout << "Your Total SCORE: " << Score;

}

void Spawn() { // ����� ������
    int apple = 0;
    do {
        int X_Apple = RAND(1, PoleHeight - 2);
        int Y_Apple = RAND(2, PoleWidth - 3);
        if (Pole[X_Apple][Y_Apple] != 'O' && Pole[X_Apple][Y_Apple] != 'o') {
            Pole[X_Apple][Y_Apple] = '0';
            apple = 1;
        }
    } while (apple != 1);
}

void PrintHvost(Coord* tail, int cnttail) { // ������� ���������� ������
    for (int i = 0; i < cnttail; i++) {
        Pole[tail[i].row][tail[i].col] = 'o';
    }
    Pole[tail[cnttail].row][tail[cnttail].col] = ' ';
}

void About() {
    SetPos(5, 12);
    cout << "Developed by Mussabekov Temirlan in 2022";
    SetPos(7, 12);
    cout << "Press ESC for back in menu";
    int key = 0;
    key = _getch();
    switch (key)
    {
    case -1: // ESC
        CLS;
        Menu();
        break;
    }
}

int SdvigHvosta(Coord* tail, int cnttail, Coord Head) { // ������� ������������ ������ �� �������

    for (int i = cnttail; i != 0; i--) {
        tail[i] = tail[i - 1];
    }
    tail[0] = Head;

    return 0;
}
// ������� ������ �������� ���� �� �����
int DrawPole(int key) {
    for (int i = 0; i < PoleHeight; i++)
    {

        // ��������� ������ ������� ��� i-�� ������ ����
        SetPos(PoleRow + i, PoleCol);

        for (int j = 0; j < PoleWidth; j++)
        {
            if (Pole[i][j] == '#') { // ������ �����
                SetColor(COLOR::black, COLOR::green);
                cout << char(177);
            }
            else if (Pole[i][j] == '0') { // ������
                SetColor(COLOR::black, COLOR::light_red);
                cout << '0';
            }
            else if (Pole[i][j] == '&') { // ������� ����� ����
                SetColor(COLOR::black, COLOR::yellow);
                cout << 'O';
            }
            else if (Pole[i][j] == 'o') { // �����
                SetColor(COLOR::black, COLOR::yellow);
                cout << 'o';
            }
            else { cout << Pole[i][j]; } // ��� ��������� ������� ����

        } // for (j : PoleWidth)
    } // for (i : PoleHeight)
    return 1;
} // void DrawPole()
