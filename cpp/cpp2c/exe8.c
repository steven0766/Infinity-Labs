void Foo()
{
  /* B array1[100]*/
  B array1[100];
  int i;
  for (i = 0; i < 100; i++)
    B_ctor(&array1[i]);
  /* B* array2 = new B[100]*/
  B* array2 = (B*)malloc(sizeof(B) * 100);
  for (i = 0; i < 100; i++)
    B_ctor(&array2[i]);
  /*delete[] array2*/
  for (i = 99; i >= 0; i--)
    B_dtor(&array2[i]);
  free(array2);

  /*before exit foo*/
  for (i = 99; i >= 0; i--)
    B_dtor(&array1[i]);
}