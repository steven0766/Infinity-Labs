#include <stdio.h>
#include <stdlib.h>

#define maxFunc(t1, t2) (((t1) > (t2)) ? (t1) : (t2))

typedef void (*v_func)(void*);

typedef struct
{
  v_func* vptr;
  int m_license_plate;
} Public_Transport;

static int s_count = 0;

void Public_Transport_Dtor(Public_Transport* this);
void Public_Transport_Display(Public_Transport* this);

static v_func Public_Transport_VTable[] = {(v_func)Public_Transport_Dtor,
                                           (v_func)Public_Transport_Display};

Public_Transport* Public_Transport_Ctor(Public_Transport* this)
{
  this->vptr = Public_Transport_VTable;
  this->m_license_plate = ++s_count;
  printf("Public_Transport::Ctor() %d\n", this->m_license_plate);

  return this;
}

void Public_Transport_Dtor(Public_Transport* this)
{
  this->vptr = Public_Transport_VTable;
  --s_count;
  printf("Public_Transport::Dtor() %d\n", this->m_license_plate);
}

Public_Transport* Public_Transport_Cctor(Public_Transport* this,
                                         const Public_Transport* other)
{
  this->vptr = Public_Transport_VTable;
  this->m_license_plate = ++s_count;
  printf("Public_Transport::Cctor() %d\n", this->m_license_plate);

  return this;
}

void Public_Transport_Display(Public_Transport* this)
{
  printf("Public_Transport::Display(): %d\n", this->m_license_plate);
}

static void Public_Transport_PrintCount()
{
  printf("s_count: %d\n", s_count);
}

int Public_Transport_GetID(Public_Transport* this)
{
  return this->m_license_plate;
}

typedef struct
{
  Public_Transport m_pt;
  int m_numSeats;
} Minibus;

void Minibus_Dtor(Minibus* this);
void Minibus_Display(Minibus* this);
void Minibus_Wash(Minibus* this);

static v_func Minibus_VTable[] = {(v_func)Minibus_Dtor, (v_func)Minibus_Display,
                                  (v_func)Minibus_Wash};

Minibus* Minibus_Ctor(Minibus* this)
{
  Public_Transport_Ctor(&this->m_pt);
  this->m_pt.vptr = Minibus_VTable;
  this->m_numSeats = 20;
  printf("Minibus::Ctor()\n");

  return this;
}

Minibus* Minibus_Cctor(Minibus* this, const Minibus* other)
{
  Public_Transport_Cctor(&this->m_pt, &other->m_pt);
  this->m_pt.vptr = Minibus_VTable;
  this->m_numSeats = other->m_numSeats;
  printf("Minibus::Cctor()\n");

  return this;
}

void Minibus_Dtor(Minibus* this)
{
  printf("Minibus::dtor()\n");
  this->m_pt.vptr = Minibus_VTable;
  Public_Transport_Dtor(&this->m_pt);
}

void Minibus_Display(Minibus* this)
{
  printf("Minibus::Display() ID: %d", Public_Transport_GetID(&this->m_pt));
  printf(" num seats: %d\n", this->m_numSeats);
}

void Minibus_Wash(Minibus* this)
{
  printf("Minibus::Wash() ID: %d\n", Public_Transport_GetID(&this->m_pt));
}

typedef struct
{
  Public_Transport m_pt;
} Taxi;

void Taxi_Dtor(Taxi* this);
void Taxi_Display(Taxi* this);

static v_func Taxi_VTable[] = {(v_func)Taxi_Dtor, (v_func)Taxi_Display};

Taxi* Taxi_Ctor(Taxi* this)
{
  Public_Transport_Ctor(&this->m_pt);
  this->m_pt.vptr = Taxi_VTable;
  printf("Taxi::Ctor()\n");

  return this;
}

Taxi* Taxi_Cctor(Taxi* this, const Taxi* other)
{
  Public_Transport_Cctor(&this->m_pt, &other->m_pt);
  this->m_pt.vptr = Taxi_VTable;
  printf("Taxi::Cctor()\n");

  return this;
}

void Taxi_Dtor(Taxi* this)
{
  printf("Taxi::Dtor()\n");
  this->m_pt.vptr = Taxi_VTable;
  Public_Transport_Dtor(&this->m_pt);
}

void Taxi_Display(Taxi* s)
{
  printf("Taxi::Display() ID: %d\n", Public_Transport_GetID(&s->m_pt));
}

typedef struct
{
  Taxi m_t;
} Special_Taxi;

void Special_Taxi_Dtor(Special_Taxi* this);
void Special_Taxi_Display(Special_Taxi* this);

static v_func Special_Taxi_VTable[] = {(v_func)Special_Taxi_Dtor,
                                       (v_func)Special_Taxi_Display};

Special_Taxi* Special_Taxi_Ctor(Special_Taxi* this)
{
  Taxi_Ctor(&this->m_t);
  this->m_t.m_pt.vptr = Special_Taxi_VTable;
  printf("Special_Taxi::Ctor()\n");

  return this;
}

Special_Taxi* Special_Taxi_Cctor(Special_Taxi* this, const Special_Taxi* other)
{
  Taxi_Cctor(&this->m_t, &other->m_t);
  this->m_t.m_pt.vptr = Special_Taxi_VTable;
  printf("Special_Taxi::CCtor()\n");

  return this;
}

void Special_Taxi_Dtor(Special_Taxi* this)
{
  printf("Special_Taxi::Dtor()\n");
  this->m_t.m_pt.vptr = Special_Taxi_VTable;
  Taxi_Dtor(&this->m_t);
}

void Special_Taxi_Display(Special_Taxi* this)
{
  printf("Special_Taxi::Display() ID: %d\n",
         Public_Transport_GetID(&this->m_t.m_pt));
}

typedef struct
{
  Public_Transport m_pt;
  Public_Transport* m_pt1;
  Public_Transport* m_pt2;
  Minibus m_m;
  Taxi m_t;
} Public_Convoy;

void Public_Convoy_Dtor(Public_Convoy* this);
void Public_Convoy_Display(Public_Convoy* this);

static v_func Public_Convoy_VTable[] = {(v_func)Public_Convoy_Dtor,
                                        (v_func)Public_Convoy_Display};

Public_Convoy* Public_Convoy_Ctor(Public_Convoy* this)
{
  Public_Transport_Ctor(&this->m_pt);
  this->m_pt.vptr = Public_Convoy_VTable;
  this->m_pt1 = (Public_Transport*)malloc(sizeof(Minibus));
  Minibus_Ctor((Minibus*)this->m_pt1);
  this->m_pt2 = (Public_Transport*)malloc(sizeof(Taxi));
  Taxi_Ctor((Taxi*)this->m_pt2);
  Minibus_Ctor(&this->m_m);
  Taxi_Ctor(&this->m_t);

  return this;
}

void Public_Convoy_Dtor(Public_Convoy* this)
{
  this->m_pt.vptr = Public_Convoy_VTable;
  Minibus_Dtor((Minibus*)this->m_pt1);
  free(this->m_pt1);
  Taxi_Dtor((Taxi*)this->m_pt2);
  free(this->m_pt2);
  Taxi_Dtor(&this->m_t);
  Minibus_Dtor(&this->m_m);
  Public_Transport_Dtor(&this->m_pt);
}

Public_Convoy* Public_Convoy_Cctor(Public_Convoy* this,
                                   const Public_Convoy* other)
{
  Public_Transport_Cctor(&this->m_pt, &other->m_pt);
  this->m_pt.vptr = Public_Convoy_VTable;
  this->m_pt1 = (Public_Transport*)malloc(sizeof(Minibus));
  Minibus_Cctor((Minibus*)this->m_pt1, (Minibus*)other->m_pt1);
  this->m_pt2 = (Public_Transport*)malloc(sizeof(Taxi));
  Taxi_Cctor((Taxi*)this->m_pt2, (Taxi*)other->m_pt2);
  Minibus_Cctor(&this->m_m, &other->m_m);
  Taxi_Cctor(&this->m_t, &other->m_t);

  return this;
}

void Public_Convoy_Display(Public_Convoy* this)
{
  this->m_pt1->vptr[1](this->m_pt1);
  this->m_pt2->vptr[1](this->m_pt2);
  this->m_m.m_pt.vptr[1](&this->m_m.m_pt);
  this->m_t.m_pt.vptr[1](&this->m_t.m_pt);
}

void PrintInfoi(Public_Transport* a)
{
  a->vptr[1](a);
}
void PrintInfoii()
{
  Public_Transport_PrintCount();
}

void PrintInfoiii(Public_Transport* ret, int i)
{
  Minibus mb;
  Minibus_Ctor(&mb);
  printf("PrintInfo(int i)\n");
  mb.m_pt.vptr[1](&mb);
  Public_Transport_Cctor(ret, &mb.m_pt);
  Minibus_Dtor(&mb);
}

void TaxiDisplay(Taxi s)
{
  Taxi_Display(&s);
}

int main(int argc, char** argv, char** envp)
{
  /* Minibus m;
PrintInfo(m);
PrintInfo(3).Display(); */
  Minibus m;
  Minibus_Ctor(&m);

  PrintInfoi(&m.m_pt);

  Public_Transport tmp;
  PrintInfoiii(&tmp, 3);
  Public_Transport_Display(&tmp);
  Public_Transport_Dtor(&tmp);

  /*   Public_Transport* array[] = {new Minibus(), new Taxi(), new Minibus()};
   */
  Public_Transport* arr[3];
  arr[0] = (Public_Transport*)malloc(sizeof(Minibus));
  Minibus_Ctor((Minibus*)arr[0]);
  arr[1] = (Public_Transport*)malloc(sizeof(Taxi));
  Taxi_Ctor((Taxi*)arr[1]);
  arr[2] = (Public_Transport*)malloc(sizeof(Minibus));
  Minibus_Ctor((Minibus*)arr[2]);

  /*  for (int i = 0; i < 3; ++i)
    array[i]->Display();
  for (int i = 0; i < 3; ++i)
    delete array[i];*/
  for (int i = 0; i < 3; ++i)
  {
    arr[i]->vptr[1](arr[i]);
  }

  for (int i = 0; i < 3; ++i)
  {
    arr[i]->vptr[0](arr[i]);
    free(arr[i]);
  }

  /*  Public_Transport arr2[] = {Minibus(), Taxi(), Public_Transport()};
   */
  Public_Transport arr2[3];

  Minibus m1;
  Minibus_Ctor(&m1);
  Public_Transport_Cctor(&arr2[0], &m1.m_pt);

  Taxi t1;
  Taxi_Ctor(&t1);
  Public_Transport_Cctor(&arr2[1], &t1.m_pt);

  Public_Transport_Ctor(&arr2[2]);

  Taxi_Dtor(&t1);
  Minibus_Dtor(&m1);

  /*  for (int i = 0; i < 3; ++i)
  {
    arr2[i].Display();
  }*/
  for (int i = 0; i < 3; ++i)
  {
    arr2[i].vptr[1](&arr2[i]);
  }

  /*  Public_Transport::PrintCount();
  Minibus m2;
  m2.PrintCount();*/
  Public_Transport_PrintCount();

  Minibus m2;
  Minibus_Ctor(&m2);

  Public_Transport_PrintCount();

  /* Minibus arr3[4];
  Taxi* arr4 = new Taxi[4];
  delete[] arr4;  */
  Minibus arr3[4];
  for (int i = 0; i < 4; ++i)
  {
    Minibus_Ctor(&arr3[i]);
  }

  Taxi* arr4 = (Taxi*)malloc(sizeof(Taxi) * 4);
  for (int i = 0; i < 4; ++i)
  {
    Taxi_Ctor(&arr4[i]);
  }
  for (int i = 3; i >= 0; --i)
  {
    arr4[i].m_pt.vptr[0](&arr4[i]);
  }
  free(arr4);

  /* std::cout << maxFunc<int>(1, 2) << std::endl;
  Special_Taxi st;
  TaxiDisplay(st); */
  printf("%d\n", maxFunc(1, 2));
  Special_Taxi st;
  Special_Taxi_Ctor(&st);
  Taxi t_tmp;
  Taxi_Cctor(&t_tmp, &st.m_t);
  Taxi_Display(&t_tmp);
  Taxi_Dtor(&t_tmp);

  /*Public_Convoy* ts1 = new Public_Convoy();
  Public_Convoy* ts2 = new Public_Convoy(*ts1);
    ts1->Display();
  ts2->Display();*/

  Public_Convoy* ts1 = (Public_Convoy*)malloc(sizeof(Public_Convoy));
  Public_Convoy_Ctor(ts1);
  Public_Convoy* ts2 = (Public_Convoy*)malloc(sizeof(Public_Convoy));
  Public_Convoy_Cctor(ts2, ts1);
  ts1->m_pt.vptr[1](ts1);
  ts2->m_pt.vptr[1](ts2);

  /*  delete ts1;*/
  Public_Convoy_Dtor(ts1);
  free(ts1);

  /*  ts2->Display();
  delete ts2;*/
  ts2->m_pt.vptr[1](ts2);
  Public_Convoy_Dtor(ts2);
  free(ts2);

  /*end of program, dtor all other objects*/
  Special_Taxi_Dtor(&st);
  for (int i = 3; i >= 0; --i)
  {
    Minibus_Dtor(&arr3[i]);
  }
  Minibus_Dtor(&m2);
  for (int i = 2; i >= 0; --i)
  {
    Public_Transport_Dtor(&arr2[i]);
  }
  Minibus_Dtor(&m);

  return 0;
}
