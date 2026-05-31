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

static v_func v_table[] = {(v_func)Public_Transport_Dtor,
                           (v_func)Public_Transport_Display};

Public_Transport* Public_Transport_Ctor(Public_Transport* this)
{
  this->vptr = v_table;
  this->m_license_plate = ++s_count;
  printf("Public_Transport::Ctor() %d", this->m_license_plate);
}

void Public_Transport_Dtor(Public_Transport* this)
{
  --s_count;
  printf("Public_Transport::Dtor() %d\n", this->m_license_plate);
}

Public_Transport* Public_Transport_Cctor(Public_Transport* this,
                                         const Public_Transport* other)
{
  this->vptr = v_table;
  this->m_license_plate = ++s_count;
  printf("Public_Transport::Cctor() %d\n", this->m_license_plate);
}

void Public_Transport_Display(Public_Transport* this)
{
  printf("Public_TransportPublic_Transport_Display(): %d\n",
         this->m_license_plate);
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
  this->m_numSeats = other->m_numSeats;
  printf("Minibus::Cctor()\n");

  return this;
}

void Minibus_Dtor(Minibus* this)
{
  printf("Minibus::dtor()\n");
  Public_Transport_Dtor(&this->m_pt);
}

void Minibus_Display(Minibus* this)
{
  printf("Minibus::Display() ID: %d\n", Public_Transport_GetID(&this->m_pt));
  printf("num seats: %d\n", this->m_numSeats);
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

static v_func Taxi_VTable[] = {Taxi_Dtor, Taxi_Display};

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
  Public_Transport_Dtor(&this->m_pt);
}

void Taxi_Display(Taxi* this)
{
  printf("Taxi::Display() ID: %d\n", Public_Transport_GetID(&this->m_pt));
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
}

Special_Taxi* Special_Taxi_Cctor(Special_Taxi* this, const Special_Taxi* other)
{
  Taxi_Cctor(&this->m_t, &other->m_t);
  this->m_t.m_pt.vptr = Special_Taxi_VTable;
  printf("Special_Taxi::CCtor()\n");
}

void Special_Taxi_Dtor(Special_Taxi* this)
{
  printf("Special_Taxi::Dtor()\n");
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
Public_Convoy* Public_Convoy_Cctor(Public_Convoy* this,
                                   const Public_Convoy* other);

static v_func Public_Convoy_VTable[] = {(v_func)Public_Convoy_Dtor,
                                        (v_func)Public_Convoy_Cctor};

Public_Convoy* Public_Convoy_Ctor(Public_Convoy* this)
{
  Public_Transport_Ctor(&this->m_pt);
  this->m_pt.vptr = Public_Convoy_VTable;
  this->m_pt1 = (Minibus*)malloc(sizeof(Minibus));
  Minibus_Ctor((Minibus*)this->m_pt1);
  this->m_pt2 = (Taxi*)malloc(sizeof(Taxi));
  Taxi_Ctor((Taxi*)this->m_pt2);
  Minibus_Ctor(&this->m_m);
  Taxi_Ctor(&this->m_t);

  return this;
}

void Public_Convoy_Dtor(Public_Convoy* this)
{
  Public_Transport_Dtor(&this->m_pt);
  Minibus_Dtor((Minibus*)this->m_pt1);
  free(this->m_pt1);
  Taxi_Dtor((Taxi*)this->m_pt2);
  free(this->m_pt2);
  Minibus_Dtor(&this->m_m);
  Taxi_Dtor(&this->m_t);
}

Public_Convoy* Public_Convoy_Cctor(Public_Convoy* this,
                                   const Public_Convoy* other)
{
  Public_Transport_Cctor(&this->m_pt, &other->m_pt);
  this->m_pt.vptr = Public_Convoy_VTable;
  this->m_pt1 = (Minibus*)malloc(sizeof(Minibus));
  Minibus_Cctor((Minibus*)this->m_pt1, (Minibus*)other->m_pt1);
  this->m_pt2 = (Taxi*)malloc(sizeof(Taxi));
  Taxi_Cctor((Taxi*)this->m_pt2, (Taxi*)other->m_pt2);
  Minibus_Cctor(&this->m_m, &other->m_m);
  Taxi_Cctor(&this->m_t, &other->m_t);
}

void PrintInfoi(Public_Transport* a)
{
  a->vptr[1](&a);
}
void PrintInfoii()
{
  Public_Transport_PrintCount();
}

void PrintInfoiii(Public_Transport* ret, int i)
{
  Minibus mb;
  Minibus_Ctor(&mb);
  printf("PrintInfo(int i)");
  mb.m_pt.vptr[1](&mb);
  Public_Transport_Cctor(&ret, &mb.m_pt);
}

void TaxiDisplay(Taxi s)
{
  Taxi_Display(&s);
}
