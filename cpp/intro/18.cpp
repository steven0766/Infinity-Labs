#include <iostream>

void IncByOne(int& n)
{
  ++n;
}

int main()
{
  // int reference
  int i = 8;
  int& ip = i;
  ip = 9;
  std::cout << "int: " << i << std::endl;

  // double reference
  double d = 3.14;
  double& rd = d;
  rd = 2.71;
  std::cout << "double: " << d << std::endl;

  // char reference
  char c = 'a';
  char& rc = c;
  rc = 'z';
  std::cout << "char: " << c << std::endl;

  // reference to array element
  int arr[5] = {1, 2, 3, 4, 5};
  int& ra = arr[2];
  ra = 99;
  std::cout << "array[2]: " << arr[2] << std::endl;

  // reference to pointer
  int* p = &i;
  int*& rp = p;
  std::cout << "pointer ref: " << *rp << std::endl;

  // function with & type
  int num = 3;
  IncByOne(num);
  std::cout << "num: " << num << std::endl;

  // size of reference type
  int a = 4;
  int& ra2 = a;
  std::cout << "size of reference type: " << sizeof(ra2)
            << std::endl; // sizeof reference type is 4

  // address of reference type
  std::cout << "address of a is: " << &a << std::endl;
  std::cout << "address of reference to a is: &ra2" << &ra2
            << std::endl; // reference type is just another variable that points
                          // to address of another variable

  return 0;
}