#include "circle.hpp"
#include "glut_utils.h"
#include "group.hpp"
#include "line.hpp"
#include "point.hpp"
#include "rectangle.hpp"
#include "square.hpp"
#include <stdio.h>

using namespace hrd42;
using namespace ilrd;

static Group* g_group[9] = {0};
static int group_idx = 0;
static int g_x = 0;
static int g_y = 0;
static int g_is_jump;

static void DrawFunction();
static int KeyboardFunction(unsigned char key, int x, int y);
static int TimerFunction();

int main(int argc, char** argv)
{
  Square* square_left = new Square(Point(200, 200), 30, 0, COLOR_BLUE);
  Square* square_right = square_left->Clone();
  Circle* eye_left = new Circle(Point(150, 150), 15, 0, COLOR_GREEN);
  Circle* eye_right = eye_left->Clone();
  Rectangle* eyebrow_left =
      new Rectangle(Point(500, 200), 40, 10, 0, COLOR_CYAN);
  Rectangle* eyebrow_right = eyebrow_left->Clone();
  Line* smile = new Line(Point(500, 300), 170, 0, COLOR_RED);
  Circle* face = new Circle(Point(750, 750), 120, 0, COLOR_YELLOW);
  g_group[0] = new Group(*square_left);
  g_group[1] = new Group(*square_right);
  g_group[2] = new Group(*eye_left);
  g_group[3] = new Group(*eye_right);
  g_group[4] = new Group(*eyebrow_left);
  g_group[5] = new Group(*eyebrow_right);
  g_group[6] = new Group(*smile);
  g_group[7] = new Group(*face);
  g_group[8] = new Group(*square_left);
  g_group[8]->AddShape(square_right);
  g_group[8]->AddShape(eye_left);
  g_group[8]->AddShape(eye_right);
  g_group[8]->AddShape(eyebrow_left);
  g_group[8]->AddShape(eyebrow_right);
  g_group[8]->AddShape(smile);
  g_group[8]->AddShape(face);

  DrawInit(argc, argv, 1000, 1000, DrawFunction);
  DrawSetKeyboardFunc(KeyboardFunction);
  DrawSetTimerFunc(TimerFunction, 100);
  DrawMainLoop();

  for (int i = 0; i < 8; ++i)
  {
    delete g_group[i];
  }

  delete square_left;
  delete square_right;
  delete eye_left;
  delete eye_right;
  delete eyebrow_left;
  delete eyebrow_right;
  delete smile;
  delete face;

  return 0;
}

static void DrawFunction()
{
  for (int i = 0; i < 8; ++i)
  {
    g_group[i]->Draw();
  }
}

static int KeyboardFunction(unsigned char key, int x, int y)
{
  (void)x;
  (void)y;
  if (key == 0x1b)
    return -1;

  if (key == 'r')
  {
    g_group[group_idx]->Rotate(0.1);
    return 1;
  }

  if ('t' == key)
  {
    g_group[group_idx]->Revolve(Point(400, 400), 0.1);
    return 1;
  }

  if (key == 'c')
  {
    g_group[group_idx]->SetColor(COLOR_MAGENTA);
    return 1;
  }

  if (key == 'w')
  {
    g_group[group_idx]->Move(0, -10);
    return 1;
  }

  if (key == 's')
  {
    g_group[group_idx]->Move(0, 10);
    return 1;
  }

  if (key == 'a')
  {
    g_group[group_idx]->Move(-10, 0);

    return 1;
  }

  if (key == 'd')
  {
    g_group[group_idx]->Move(10, 0);
    return 1;
  }

  if ('0' == key)
  {
    group_idx = 0;
    return 1;
  }

  if ('1' == key)
  {
    group_idx = 1;
    return 1;
  }

  if ('2' == key)
  {
    group_idx = 2;
    return 1;
  }

  if ('3' == key)
  {
    group_idx = 3;
    return 1;
  }

  if ('4' == key)
  {
    group_idx = 4;
    return 1;
  }

  if ('5' == key)
  {
    group_idx = 5;
    return 1;
  }

  if ('6' == key)
  {
    group_idx = 6;
    return 1;
  }

  if ('7' == key)
  {
    group_idx = 7;
    return 1;
  }

  if ('8' == key)
  {
    group_idx = 8;
    return 1;
  }

  if (32 == key)
  {
    g_is_jump = 1;
    g_y = -10;
    return 1;
  }

  return 0;
}

static int TimerFunction()
{
  if (1 == g_is_jump)
  {
    g_group[group_idx]->Move(g_x, g_y);
    if (10 == g_y)
    {
      g_is_jump = 0;
    }
    ++g_y;
  }
  return 1;
}