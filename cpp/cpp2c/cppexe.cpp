typedef union u1
{
  int x;
  float y;
  void add(int a)
  {
    x += a;
  }
};

// union u2 : u1
// {

// };

class c1
{
  int a;
  int b;
};

// union u3 : c1
// {

// };

int main()
{
  u1 u = {5.5};

  return 0;
}