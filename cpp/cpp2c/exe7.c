typedef struct
{
  int x;
  int y;
} B;

B* B_Ctor(B* this)
{
  this->x = 0;
  this->y = 0;
}

void B_Dtor(B* this)
{
}

void SetX(B* this, int a)
{
  this->x = a;
}

void SetY(B* this, int b)
{
  this->y = b;
}

int main()
{
  B b;
  B_Ctor(&b);
  SetX(&b, 5);
  SetY(&b, 6);
  B* p = (B*)malloc(sizeof(B));
  SetX(p, 5);
  SetY(p, 6);
  B_Dtor(p);
  free(p);
  B_Dtor(&b);

  return 0;
}