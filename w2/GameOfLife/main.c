#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

#define WIDTH 80
#define HEIGHT 25

#define COLUMNS WIDTH
#define ROWS HEIGHT

#define SPEED_MIN 5000000
#define SPEED_MAX 50000
#define SPEED_D 10000

// TODO: Заменить везде ошибки на макросы
#define ERROR_BASE 1
#define ERROR_EXIT 2

#define INFO_HEIGHT 10

// Функции вывода
void printGreeting();
void printFinish();
void printJustLine();
void printPic();
void printModes();
void printHalfInstruction();
void printInstructions();
void printField(int **);
// TODO: modes instructions

// Функции получения данных
int inputMode(int *);
int inputField(int **);
int inputKey(char *key, int *speed); // еще и преобразует данные, что не очень хорошо, но пока так

// Функции работы с матрицами
int allocateMatrix(int **matrix);
void freeMatrix(int **matrix);
//int copyMatrix(int **from, int **to);
// void exit();

// Функции, реализующие логику игры
void makeLifeStep(int **);
int getNeighborsCount(int **matrix, int row, int column);
// process

// Остальные фунции
// void initWindow(void);

// TODO: Вынести в game loop
// TODO: Заменить где надо на H W
// TODO: Игнорить есть не та клавиша, не выходить
int main()
{
    int **current = NULL;
    int mode = 0, error = 0, isExit = 0;
    int speed = SPEED_MIN;
    printGreeting();
    printModes();

    mode = inputMode(&error);
    // Если нет ошибки в инпуте, в выделении памяти для двух матриц
    if (!error && !allocateMatrix(current))
    {

        char k;
        // заполняем игровое поле
        // TODO: добавить разные заполнения в зависимости от мода
   
        while (!error && !isExit)
        {
            error = inputField(current);
            printField(current);
            makeLifeStep(current);
            usleep(speed);
        }
    }

    printFinish();
    return 1;
}

// Одна итерация по циклу жизни
void makeLifeStep(int **current)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            int nc = getNeighborsCount(current, i, j); // получаем количество соседей вокруг ячейки

            if (nc == 3 && current[i][j] == 0) // Each cell with three neighbors becomes populated
            {
                current[i][j] = 1;
            }
            else if (current[i][j] == 1 && nc != 2 &&
                     nc != 3)
            {
                // проверяем условия на смерть ячейки:
                // Each cell with one or no neighbors dies, as if by solitude.
                // Each cell with four or more neighbors dies, as if by overpoulation.
                current[i][j] = 0;
            }
           
            // Если не overpopulation и не solitude (одиночество), то оставляем  TODO: (или 1?) (тут был if)
                
            
        }
    }
}

// Функция получения количества соседей вокруг ячейки
int getNeighborsCount(int **matrix, int row, int column)
{
    int count = 0;
    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = column - 1; j <= column + 1; j++)
        {
            if (i != row || j != column)
            {
                int r = (i + ROWS) % ROWS; // вычисляем координаты соседа
                int c = (j + COLUMNS) % COLUMNS;
                count += matrix[r][c]; // тк 1 означает соседа, а 0 - просто поле,
                // то суммируя значения соседей, мы получим и их количество
            }
        }
    }
    return count;
}

// Считываем режим игры: из файла, ручками вводим данные или генирируем в программе
int inputMode(int *error)
{
    char c;
    int mode;
    if (scanf("%d%c", &mode, &c) != 2 || c != '\n' || (mode != 0 && mode != 1 && mode != 2))
    {
        *error = 1;
    }

    return *error ? 0 : mode;
}
int inputField(int **field)
{
    int error = 0;
    for (int i = 0; i < HEIGHT && !error; i++)
    {
        for (int j = 0; j < WIDTH && !error; j++)
        {
            if (scanf("%d", &field[i][j]) != 1 ||
                (field[i][j] != 0 && field[i][j] != 1))
            {
                error = 1;
            }
        }
    }

    return error;
}

int inputKey(char *key, int *speed)
{
    int error = 0;
    *key = getchar();

    // обрабатываем нажатие на w
    if (*key == 'W' || *key == 'w')
    {
        // проверяем: если будет выход за макс, не меняем
        *speed = *speed + SPEED_D > SPEED_MAX ? *speed : *speed + SPEED_D;
    }
    else if (*key == 'S' || *key == 's')
    {
        // проверяем: если будет выход за min, не меняем
        *speed = *speed - SPEED_D < SPEED_MIN ? *speed : *speed - SPEED_D;
    }
    else if (*key == 'Q' || *key == 'q')
    {
        error = 2;
    }
    else
    {
        error = 1;
    }
    return error;
}

void initWindow()
{
    // nodelay(initscr(), 1);
    // noecho();
}

int allocateMatrix(int **matrix)
{
    int error = 0;
    // Если матрица не пустая - надо сначала очистить старое, а потом выделить новую память (иначе - утечка)
    // Если матрица пустая - достаточно просто выделить память
    if (matrix != NULL)
    {
        freeMatrix(matrix);
    }
    matrix = (int **)malloc(ROWS * sizeof(int));
    if (matrix == NULL)
    { // Выделение может произойти с ошибкой, тогда будет NULL
        error = 1;
    }
    else
    {
        for (int i = 0; i < ROWS; i++)
        {
            matrix[i] = (int *)malloc(COLUMNS * sizeof(int));
            if (matrix[i] == NULL)
            { // Выделение может произойти с ошибкой, тогда будет NULL
                error = 1;
            }
        }
    }
    return error;
}

// Освобождаем память
// Массив каждой строчки освобождаем
// Потом освобождаем массив строчек
// Зануляем указатель для однозначиности (чтобы точно понимать, что там пусто)
void freeMatrix(int **matrix)
{
    for (int i = 0; i < ROWS; i++)
    {
        free(matrix[i]);
    }

    free(matrix);
    matrix = NULL;
}

int copyMatrix(int **from, int **to)
{
    int error = 0;
    if (from != NULL && to != NULL)
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                to[i][j] = from[i][j];
            }
        }
    }
    else
    {
        error = 1;
    }

    return error;
}

// Печатает режимы игры
void printModes()
{
    printHalfInstruction();
    printf("Which game mode do you choose?");
    printHalfInstruction();
    printf("0 - File initialization");
    printHalfInstruction();
    printf("1 - Yours initialization");
    printHalfInstruction();
    printf("2 - Auto initialization");
    printHalfInstruction();
    printf("Type mode number + 'enter'");
    printHalfInstruction();
}

// Просто печатает линию из символов
void printJustLine()
{
    for (int j = 0; j < WIDTH; j++)
    {
        printf("░");
    }
    printf("\n");
}

// Печатает отступы на половину ширины (для выравнивания последующих фраз)
void printHalfInstruction()
{
    printf("\n");
    for (int i = 0; i < 29; i++)
    {
        printf(" ");
    }
}

// Печатает стартовые инструкции игры
void printInstructions()
{
    printHalfInstruction();
    printf("     GAME OF LIFE");
    printHalfInstruction();
    printHalfInstruction();
    printf("─  ─  ─  ─  ─  ─  ─  ─");
    printHalfInstruction();
    printf(" Use '─' to speed up. ");
    printHalfInstruction();
    printf("Use 'S' to speed do─n.");
    printHalfInstruction();
    printf("   Use 'Q' to quit.   ");
    printHalfInstruction();
    printf("─  ─  ─  ─  ─  ─  ─  ─");
    printHalfInstruction();
    printHalfInstruction();
    printf("Press 'enter' to start");
}

// Функция, отвечающая за вывод стартового экрана
void printGreeting()
{
    printJustLine();
    for (int i = 0; i < INFO_HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == INFO_HEIGHT / 2 - 2)
            {
                printInstructions();
                i += 3;
            }
            else
            {
                printf(" ");
            }
        }
    }
    printf("\n");
    printJustLine();
}

void printField(int **field)
{
   // move(0, 0);
   // clear();

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (field[i][j] == 0)
            {
               // printf("%c", '·');
                printf("%c", (char)250);
            }
            else
            {
                //printf("%c", '■');
                printf("%c", (char)254);
            }
        }
        printf("\n");
    }
    printf("\n");
    //refresh();
}

// Печатает картинку
void printPic()
{
    printHalfInstruction();
    printf("     GAME FINISHED");
    printHalfInstruction();
    printHalfInstruction();
    printf(" ──────▄▀▄─────▄▀▄");
    printHalfInstruction();
    printf(" ─────▄█░░▀▀▀▀▀░░█▄");
    printHalfInstruction();
    printf(" ─▄▄──█░░░░░░░░░░░█──▄▄");
    printHalfInstruction();
    printf(" █▄▄█─█░░▀░░┬░░▀░░█─█▄▄█");
    printHalfInstruction();
}

// Функция, отвечающая за вывод
void printFinish()
{
    printJustLine();
    for (int i = 0; i < INFO_HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == INFO_HEIGHT / 2 - 2)
            {
                printPic();
                i += 3;
            }
            else
            {
                printf(" ");
            }
        }
    }
    printf("\n");
    printJustLine();
}

// Функция отвечающая за закрытие программы
void exitGame()
{
    // TODO: написать тип выход из игры - таймаут - выход
    exit(0);
}
