typedef struct Base
{
  int x;
  int y;
} Base;

void Base_ctor(Base* this, int inX, int inY)
{
  this->x = inX;
  this->y = inY;
}

void Base_dtor(Base* this)
{
}

void Base_copy_ctor(Base* this, const Base* other)
{
  this->x = other->x;
  this->y = other->y;
}

Base* Base_assign(Base* this, const Base* other)
{
  if (this != other)
  {
    this->x = other->x;
    this->y = other->y;
  }
  return this;
}

typedef struct Derived
{
  Base base;
  int z;
} Derived;

void Derived_ctor(Derived* this, int inX, int inY, int inZ)
{
  Base_ctor(&this->base, inX, inY);
  this->z = inZ;
}

void Derived_dtor(Derived* this)
{
  Base_dtor(&this->base);
}

void Derived_copy_ctor(Derived* this, const Derived* other)
{
  Base_copy_ctor(&this->base, &other->base);
  this->z = other->z;
}

Derived* Derived_assign(Derived* this, const Derived* other)
{
  if (this != other)
  {
    Base_assign(&this->base, &other->base);
    this->z = other->z;
  }
  return this;
}