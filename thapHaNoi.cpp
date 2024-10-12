#include "mylib.h"
#include <fstream>
using namespace std;

#define STACKSIZE 5
const int x = 30;
const int y = 10;
const int w = 13;
const int h = 5;
const int space = 11;
const int xA = x + (w / 2);
const int xB = x + w + space + (w / 2);
const int xC = x + 2 * (space + w) + (w / 2);
int speed = 30;
bool isPaused = false;
int stepCount = 0;

struct Move
{
    int n;
    int from;
    int to;
    int aux;
    int fromX;
    int toX;
    int auxX;
};

struct MoveStack
{
    Move moves[100];
    int top = -1;
};

struct stack
{
    int sp1 = -1;
    int nodes[STACKSIZE];
};

stack cot_A, cot_B, cot_C;
ofstream logFile;

void pushMove(MoveStack &st, int n, int from, int to, int aux,
              int fromX, int toX, int auxX)
{
    st.top++;
    st.moves[st.top] = {n, from, to, aux, fromX, toX, auxX};
}

Move popMove(MoveStack &st)
{
    Move move = st.moves[st.top];
    st.top--;
    return move;
}

int pushStack1(stack &st, int x)
{
    if (st.sp1 == STACKSIZE - 1)
        return 0;
    st.nodes[++(st.sp1)] = x;
    return 1;
}

int popStack1(stack &st, int &x)
{
    if (st.sp1 == -1)
        return 0;
    x = st.nodes[(st.sp1)--];
    return 1;
}

void writeLog(int disk, char chFrom, char chTo)
{
    stepCount++;
    gotoxy(0, 22);
    SetBGColor(9);
    SetColor(15);
    cout << "B" << stepCount << ": Chuyen dia " << disk
         << " tu " << chFrom << " sang " << chTo;
    SetBGColor(0);
    SetColor(15);
    logFile << "B" << stepCount << ": Chuyen dia " << disk
            << " tu " << chFrom << " sang " << chTo << endl;
}

void drawDisk(int size, int x, int y)
{
    gotoxy(x - size, y);
    for (int i = 0; i < size * 2 + 1; i++)
    {
        SetBGColor(11);
        cout << " ";
    }
    SetBGColor(0);
}

void eraseDisk(int size, int x, int y)
{
    gotoxy(x - size, y);
    for (int i = 0; i < size * 2 + 1; i++)
    {
        cout << " ";
    }
}

void drawTowers()
{
    for (int xa = x; xa < x + w; xa++)
    {
        gotoxy(xa, y + h);
        SetBGColor(6);
        cout << " ";
        SetBGColor(0);
    }
    for (int xb = x + w + space; xb < x + w + space + w; xb++)
    {
        gotoxy(xb, y + h);
        SetBGColor(6);
        cout << " ";
        SetBGColor(0);
    }
    for (int xc = x + 2 * (space + w); xc < x + 2 * (space + w) + w; xc++)
    {
        gotoxy(xc, y + h);
        SetBGColor(6);
        cout << " ";
        SetBGColor(0);
    }
    for (int ya = y; ya <= y + h; ya++)
    {
        gotoxy((x + x + w) / 2, ya);
        SetBGColor(6);
        cout << " ";
        SetBGColor(0);
    }
    for (int yb = y; yb <= y + h; yb++)
    {
        gotoxy((x + w + space + x + w + space + w) / 2, yb);
        SetBGColor(6);
        cout << " ";
        SetBGColor(0);
    }
    for (int yc = y; yc <= y + h; yc++)
    {
        gotoxy((x + 2 * (space + w) + x + 2 * (space + w) + w) / 2, yc);
        SetBGColor(6);
        cout << " ";
        SetBGColor(0);
    }
    SetBGColor(5);
    gotoxy((2 * x + w) / 2 - 3, y + h + 2);
    cout << " Cot A ";
    SetBGColor(0);
    gotoxy((2 * (x + w + space) + w) / 2 - 3, y + h + 2);
    SetBGColor(5);
    cout << " Cot B ";
    SetBGColor(0);
    gotoxy((2 * (x + w + space + w + space) + w) / 2 - 3, y + h + 2);
    SetBGColor(5);
    cout << " Cot C ";
    SetBGColor(0);
}

void drawState()
{
    system("cls");
    drawTowers();
    for (int i = 0; i <= cot_A.sp1; i++)
        drawDisk(cot_A.nodes[i], xA, y + h - i - 1);

    for (int i = 0; i <= cot_B.sp1; i++)
        drawDisk(cot_B.nodes[i], xB, y + h - i - 1);

    for (int i = 0; i <= cot_C.sp1; i++)
        drawDisk(cot_C.nodes[i], xC, y + h - i - 1);
}

void handleControls()
{
    ShowCursor(false);
    gotoxy(0, 23);
    cout << "Speed = " << speed << endl;
    cout << "KeyUp:   Speed + 5\nKeyDown: Speed - 5\n";
    if (_kbhit())
    {
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            switch (key)
            {
            case 72:
                speed = min(100, speed + 5);
                gotoxy(0, 23);
                if (speed < 100)
                    cout << "Speed = " << speed << " ";
                else
                    cout << "Speed = " << speed;
                break;
            case 80:
                speed = max(0, speed - 5);
                gotoxy(0, 23);
                if (speed > 0)
                    cout << "Speed = " << speed << " ";
                else
                    cout << "Speed = " << speed;
                break;
            }
        }
        else if (key == 32)
        {
            isPaused = !isPaused;
            gotoxy(0, 26);
            if (isPaused)
                cout << "DANG TAM DUNG - Nhan SPACE de tiep tuc";
            else
                cout << "                                      ";
        }
    }
}

void moveDisk(stack &from, stack &to, int fromX, int toX, char chFrom, char chTo)
{
    ShowCursor(false);
    int disk;
    if (popStack1(from, disk))
    {
        writeLog(disk, chFrom, chTo);
        // di len
        int fromY = y + h - from.sp1 - 2;
        for (int cy = fromY; cy > y; cy--)
        {
            while (isPaused)
            {
                handleControls();
                Sleep(50);
            }
            handleControls();
            eraseDisk(disk, fromX, cy);
            drawDisk(disk, fromX, cy - 1);
            drawTowers();
            Sleep(speed);
        }
        // di ngang
        int step = (toX > fromX) ? 1 : -1;
        for (int cx = fromX; cx != toX; cx += step)
        {
            while (isPaused)
            {
                handleControls();
                Sleep(50);
            }
            handleControls();
            eraseDisk(disk, cx, y);
            drawDisk(disk, cx + step, y);
            drawTowers();
            Sleep(speed);
        }
        // di xuong
        int toY = y + h - to.sp1 - 2;
        for (int cy = y; cy < toY; cy++)
        {
            while (isPaused)
            {
                handleControls();
                Sleep(50);
            }
            handleControls();
            eraseDisk(disk, toX, cy);
            drawDisk(disk, toX, cy + 1);
            drawTowers();
            Sleep(speed);
        }
        pushStack1(to, disk);
    }
}

void hanoiTower(int n, char from, char to, char aux, int fromX, int toX, int auX)
{
    MoveStack moveStack;
    for (int i = n; i >= 1; i--)
    {
        pushStack1(cot_A, i);
    }
    drawState();
    logFile << "Giai bai toan Thap Ha Noi voi " << n << " dia" << endl;
    pushMove(moveStack, n, from, to, aux, fromX, toX, auX);
    while (moveStack.top >= 0)
    {
        Move move = popMove(moveStack);

        if (move.n == 1)
        {
            stack *from_stack, *to_stack;
            if (move.from == 'A')
            {
                from_stack = &cot_A;
            }
            else if (move.from == 'B')
            {
                from_stack = &cot_B;
            }
            else
            {
                from_stack = &cot_C;
            }
            if (move.to == 'A')
            {
                to_stack = &cot_A;
            }
            else if (move.to == 'B')
            {
                to_stack = &cot_B;
            }
            else
            {
                to_stack = &cot_C;
            }
            moveDisk(*from_stack, *to_stack, move.fromX, move.toX,
                     move.from, move.to);
        }
        else
        {
            pushMove(moveStack, move.n - 1, move.aux, move.to, move.from,
                     move.auxX, move.toX, move.fromX);   //tg->dich
            pushMove(moveStack, 1, move.from, move.to, move.aux,
                     move.fromX, move.toX, move.auxX);   //bd->dich
            pushMove(moveStack, move.n - 1, move.from, move.aux, move.to,
                     move.fromX, move.auxX, move.toX);   //bd->tg
        }
    }

    logFile << "Hoan thanh sau " << stepCount << " buoc" << endl;
}
//
int main()
{
    ShowCursor(false);
    int n, choice;
    int fromX = xA, toX = xC, auX = xB;
    char from = 'A', to = 'C', au = 'B';
    
    cout << "Mac dinh\n";
    cout << "1. Goc:        " << from << endl;
    cout << "2. Trung gian: " << au << endl;
    cout << "3. Dich:       " << to << endl;
    do
    {
        cout << "Nhap so dia (1-" << STACKSIZE << "): ";
        cin >> n;
        if (n <= 0 || n > STACKSIZE)
        {
            cout << "So dia khong hop le. Nhap lai\n";
        }
    } while (n <= 0 || n > STACKSIZE);

    logFile.open("thapHaNoi.txt");
    if (!logFile.is_open())
    {
        cout << "Khong the mo file" << endl;
        return 1;
    }
    system("cls");
    hanoiTower(n, from, to, au, fromX, toX, auX);
    logFile.close();
    system("cls");
    drawState();
    drawTowers();
    gotoxy(xB - 3, 22);
    SetBGColor(10);
    SetColor(0);
    cout << "HOAN TAT";
    while (1)
    {
        _getch();
    }
    return 0;
}