// ConsoleApplication1.c.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
#include <easyx.h>
#include <conio.h>
#include <time.h>

#define NODE_WIDTH 40//网格每一格的宽度设为40像素

typedef struct {
    int x;
    int y;
}node;

enum direction
{
    eUp,
    eDown,
    eLeft,
    eRight,
};

/*{
    //竖向线段
    //第n条线段，起始点（n*NODE_WIDTH, 0),终止点（n*NODE_WIDTH, 600)
    for (int x = 0; x <= 800; x += NODE_WIDTH)//x范围【0，800】
    {
        line(x, 0, x, 600);//line函数在绘图中通过线段依次将点连接起来
        //extern void line(int left, int top, 
        //int right, int bottom, int mode);
        //(left, top)和（right, bottom)指定直线的两个端点坐标
        //mode决定划线的模式：0清除模式，1正常模式，2取反模式
    }
    //横向线段
    for (int y = 0; y <= 600; y += NODE_WIDTH)
    {
        line(0, y, 800, y);
    }
}*/

void paintGrid(void)
{
    for (int x = 0; x <= 800; x += NODE_WIDTH)//x范围【0，800】
        line(x, 0, x, 600);

    for (int y = 0; y <= 600; y += NODE_WIDTH)
        line(0, y, 800, y);
}
void paintSnake(node* snake, int n)//需要蛇节点数组首元素指针和蛇节点个数两个参数
{
    int left, top, right, bottom;
    for (int i = 0; i < n; i++)
    {
        //左上角：【网格 x坐标 * 网格宽度， 网格y坐标 * 网格宽度】
        left = snake[i].x * NODE_WIDTH;
        top = snake[i].y * NODE_WIDTH;
        //右下角：【（网格 x坐标+1） * 网格宽度， （网格y坐标+1） * 网格宽度】
        right = (snake[i].x + 1) * NODE_WIDTH;
        bottom = (snake[i].y + 1) * NODE_WIDTH;
        //通过左上角与右下角坐标绘制矩阵
        solidrectangle(left, top, right, bottom);//画无边框的填充矩形
    }
}
node snakeMove(node* snake, int length, int direction)
{
    //记录尾节点
    node tail = snake[length - 1];

    //从尾结点开始，前一个节点覆盖后一个节点
    for (int i = length - 1; i > 0; i--)
        snake[i] = snake[i - 1];

    //根据方向，确定下一个头节点
    node newHead;
    newHead = snake[0];
    if (direction == eUp)
        newHead.y--;
    else if (direction == eDown)
        newHead.y++;
    else if (direction == eLeft)
        newHead.x--;
    else
        newHead.x++;

    //更新头节点
    snake[0] = newHead;

    return tail;
}

void changeDirection(enum direction* pD)
{
    //检查输入缓冲区中是否有数据
    if (_kbhit() != 0)//同kbhit()，检查当前是否有键盘输入
        //若有则返回一个非零值，否则返回0
    {
        char c = _getch();//编程中所用的函数，为不回显函数
        //当用户按下某个字符时，函数自动读取，无需按回车

        switch (c)
        {
        case 'w' :
            //向上移动
            if(*pD != eDown)//蛇不能后退，如果新的方向与原来方向相同，则按键无效
            *pD = eUp;
            break;
        case 's':
            //向下移动
            if (*pD != eUp)
            *pD = eDown;
            break;
        case 'a':
            //向左移动
            if (*pD != eRight)
            *pD = eLeft;
            break;
        case 'd':
            //向右移动
            if (*pD != eLeft)
            *pD = eRight;
            break;      
        }
    }
}
node createFood(node* snake, int length)
{
    node food;
    while (1)
    {
        food.x = rand() % (800 / NODE_WIDTH);
        food.y = rand() % (600 / NODE_WIDTH);

        int i;
        for (i = 0; i < length; i++)
            if (snake[i].x == food.x && snake[i].y == food.y)
                break;
        if (i < length)
            continue;
        else
            break;
    }
    return food;
}
void paintFood(node food)
{
    int left, top, right, bottom;
    left = food.x * NODE_WIDTH;
    top = food.y * NODE_WIDTH;
    right = (food.x +1)* NODE_WIDTH;
    bottom = (food.y +1) * NODE_WIDTH;
    setfillcolor(YELLOW);
    solidrectangle(left, top, right, bottom);
    setfillcolor(WHITE);
}
//判断游戏是否结束
bool isGameOver(node* snake, int length)
{
    if (snake[0].x < 0 || snake[0].x > 800 / NODE_WIDTH)
        return true;

    if (snake[0].y < 0 || snake[0].y > 600 / NODE_WIDTH)
        return true;

    for(int i = 1; i < length; i++)
    {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            return true;
    }
    return false;
}
void reset(node* snake, int* plength, enum direction* d)
{
    snake[0] = node{ 5, 7 };
    snake[1] = node{ 4, 7 };
    snake[2] = node{ 3, 7 };
    snake[3] = node{ 2, 7 };
    snake[4] = node{ 1, 7 };
    *plength = 5;
    *d = eRight;
}
int main()
{
    initgraph(800, 600);//创建一个800*600的窗体，原点在左上角
    //设置背景色
    setbkcolor(RGB(164, 225, 202));
    //使用背景色清空窗体
    cleardevice();

    //蛇节点坐标
    node snake[100] = { {5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7} };
    //蛇节点长度
    int length = 5;
    enum direction d = eRight;

    srand(unsigned int(time(NULL)));
    node food = createFood(snake, length);

    while (1)
    {
        //清空整个窗体
        cleardevice();
        paintGrid();//绘制网格
        paintSnake(snake, length);//绘制蛇节点
        paintFood(food);//绘制食物

        Sleep(500);//休眠500ms

        //获取键盘输入并将方向存储到d中
        changeDirection(&d);
        //根据变量d的方向移动蛇节点
        node lastTail = snakeMove(snake, length, d);
        if (snake[0].x == food.x && snake[0].y == food.y)
        {
            if (length < 100)//限制snake节点最大长度
            {
                snake[length] = lastTail;
                length++;
            }
            food = createFood(snake, length);//重新生成新的食物
        }

        if (isGameOver(snake, length) == true)
        {
            reset(snake, &length, &d);
            food = createFood(snake, length);
        }
    }

    getchar();
    closegraph();
    return 0;
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
