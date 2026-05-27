#include "circle.hpp"
#include "glut_utils.h"
#include "group.hpp"
#include "line.hpp"
#include "point.hpp"
#include "rectangle.hpp"
#include "square.hpp"
#include <cstdlib>
#include <iostream>
#include <stdio.h>

using namespace hrd42;
using namespace ilrd;

static Group* g_group = 0;
static int test_num = 0;
static Shape* shape[4] = {0};
static char test_func = 'q';
static int g_draw = 4;

static void InitObjects();
static void FreeObjects();
static void TestSeperateOrGroup();
static int MenuFunction(int options);
static int MotionFunction(int x, int y);

static int KeyboardFunction(unsigned char key, int x, int y);
static int TimerFunction();

int main(int argc, char** argv)
{
  InitObjects();
  DrawInit(argc, argv, 1000, 1000, TestSeperateOrGroup);
  DrawSetKeyboardFunc(KeyboardFunction);
  DrawSetTimerFunc(TimerFunction, 100);
  DrawCreateMenu();
  DrawSetMenuFunc(MenuFunction);
  DrawSetMotionFunc(MotionFunction);
  DrawMainLoop();
  FreeObjects();

  return 0;
}

static void InitObjects()
{
  shape[0] = new Square(Point(200, 200), 30, 0, COLOR_BLUE);
  shape[1] = new Rectangle(Point(500, 200), 40, 10, 0, COLOR_CYAN);
  shape[2] = new Circle(Point(150, 150), 15, 0, COLOR_FILL);
  shape[3] = new Line(Point(500, 300), 170, 0, COLOR_RED);
  g_group = new Group(*shape[0]);
  for (int i = 1; i < 4; ++i)
  {
    g_group->AddShape(shape[i]);
  }
}
static void FreeObjects()
{
  for (int i = 0; i < 4; ++i)
  {
    delete shape[i];
  }
  delete g_group;
}

static void TestSeperateOrGroup()
{
  if (0 == test_num)
  {
    for (int i = 0; i < 4; ++i)
    {
      shape[i]->Draw();
    }
  }
  if (1 == test_num)
  {
    g_group->Draw();
  }
}

static int KeyboardFunction(unsigned char key, int x, int y)
{
  (void)x;
  (void)y;
  if (key == 0x1b)
    return -1;

  if ('z' == key)
  {
    test_func = 'z';
    return 1;
  }

  if ('x' == key)
  {
    test_func = 'x';
    return 1;
  }

  if ('c' == key)
  {
    test_func = 'c';
    return 1;
  }

  if (key == 'w')
  {
    if (0 == test_num)
    {
      for (int i = 0; i < 4; ++i)
      {
        shape[i]->Move(0, -10);
      }
    }
    if (1 == test_num)
    {
      g_group->Move(0, -10);
    }
    return 1;
  }
  if (key == 's')
  {
    if (0 == test_num)
    {
      for (int i = 0; i < 4; ++i)
      {
        shape[i]->Move(0, 10);
      }
    }
    if (1 == test_num)
    {
      g_group->Move(0, 10);
    }
    return 1;
  }
  if (key == 'd')
  {
    if (0 == test_num)
    {
      for (int i = 0; i < 4; ++i)
      {
        shape[i]->Move(10, 0);
      }
    }
    if (1 == test_num)
    {
      g_group->Move(10, 0);
    }
    return 1;
  }
  if (key == 'a')
  {
    if (0 == test_num)
    {
      for (int i = 0; i < 4; ++i)
      {
        shape[i]->Move(-10, 0);
      }
    }
    if (1 == test_num)
    {
      g_group->Move(-10, 0);
    }
    return 1;
  }

  if ('0' == key)
  {
    test_num = 0;
    return 1;
  }

  if ('1' == key)
  {
    test_num = 1;
    return 1;
  }

  return 0;
}

static int TimerFunction()
{
  if (0 == test_num)
  {
    int i = 1;
    if ('z' == test_func)
    {
      for (i = 1; i < 4; ++i)
      {
        shape[i]->Revolve(Point(400, 400), 0.3);
      }
    }
    if ('x' == test_func)
    {
      for (i = 1; i < 4; ++i)
      {
        shape[i]->Rotate(0.3);
      }
    }
  }
  if (1 == test_num)
  {
    if ('z' == test_func)
    {
      g_group->Revolve(Point(400, 400), 0.3);
    }
    if ('x' == test_func)
    {
      g_group->Rotate(0.3);
    }
  }
  return 1;
}

static int MenuFunction(int option)
{
  // circle
  if (1 == option)
  {
    g_draw = 1;
  }
  // rectangle
  if (2 == option)
  {
    g_draw = 2;
  }
  // line
  if (3 == option)
  {
    g_draw = 3;
  }
  // ESC
  if (4 == option)
  {
    std::cout << "exit" << std::endl;
    exit(0);
  }

  return 1;
}

static int MotionFunction(int x, int y)
{
  printf("Mouse: %d,%d\n", x, y);
  if (1 == g_draw)
  {
    g_group->AddShape(new Circle(Point(x, y), 50, 0, COLOR_GREEN));
  }
  if (2 == g_draw)
  {
    g_group->AddShape(new Rectangle(Point(x, y), 40, 10, 0, COLOR_CYAN));
  }
  if (3 == g_draw)
  {
    g_group->AddShape(new Line(Point(x, y), 170, 0, COLOR_RED));
  }

  return 0;
}