#include "bitset.hpp"
#include "test_utils.hpp"
#include <climits>
#include <vector>

using namespace hrd42;
using namespace std;

static void TestOperatorAt();
static void TestOperatorAnd();
static void TestOperatorOr();
static void TestOperatorXor();
static void TestCountBits();
static void TestCopy();

int main()
{
  TestOperatorAt();
  TestOperatorAnd();
  TestOperatorOr();
  TestOperatorXor();
  TestCountBits();
  TestCopy();

  return 0;
}

static void TestOperatorAt()
{
  Tester t("TestOperatorAt");

  BitSet<8> b1;
  for (int i = 0; i < 8; ++i)
  {
    t.AssertTrue(false == b1[i]);
    b1[i] = true;
    t.AssertTrue(true == b1[i]);
  }

  BitSet<65> b2;
  for (int i = 0; i < 65; ++i)
  {
    t.AssertTrue(false == b2[i]);
    b2[i] = true;
    t.AssertTrue(true == b2[i]);
  }
}

static void TestOperatorAnd()
{
  Tester t("TestOperatorAnd");
  // all 0
  BitSet<8> b1;
  BitSet<8> b2;
  b1 &= b2;
  t.AssertTrue(b1 == BitSet<8>());

  // all 1
  BitSet<8> b3;
  BitSet<8> b4;
  for (size_t i = 0; i < 8; ++i)
  {
    b3[i] = true;
  }
  b3 &= b3;
  t.AssertTrue(b3 == b3);
  b3 &= b4;
  t.AssertTrue(b3 == b4);

  // 1010 & 1100 = 1000
  BitSet<8> b5;
  BitSet<8> b6;
  BitSet<8> b7;
  b5[3] = b5[2] = true;
  b6[3] = b6[1] = true;
  b7[3] = true;
  b5 &= b6;
  t.AssertTrue(b7 == b5);

  // more than one block
  BitSet<130> b8;
  BitSet<130> b9;
  b8[65] = true;
  b9[65] = true;
  b9[120] = true;
  b9 &= b8;
  t.AssertTrue(b8 == b9);
}

static void TestOperatorOr()
{
  Tester t("TestOperatorOr");
  // all 0
  BitSet<8> b1;
  BitSet<8> b2;
  b1 |= b2;
  t.AssertTrue(b1 == BitSet<8>());

  // all 1
  BitSet<8> b3;
  BitSet<8> b4;
  for (size_t i = 0; i < 8; ++i)
  {
    b3[i] = true;
  }
  b3 |= b3;
  t.AssertTrue(b3 == b3);
  b3 |= b4;
  t.AssertTrue(b3 != b4);

  // 1010 | 1100 = 1000
  BitSet<8> b5;
  BitSet<8> b6;
  BitSet<8> b7;
  b5[3] = b5[2] = true;
  b6[3] = b6[1] = true;
  b7[3] = b7[2] = b7[1] = true;
  b5 |= b6;
  t.AssertTrue(b7 == b5);

  // more than one block
  BitSet<130> b8;
  BitSet<130> b9;
  BitSet<130> b10;
  b8[65] = true;
  b9[65] = true;
  b9[120] = true;
  b10 = b9;
  b9 |= b8;
  t.AssertTrue(b10 == b9);
}

static void TestOperatorXor()
{
  Tester t("TestOperatorXor");
  // all 0
  BitSet<8> b1;
  BitSet<8> b2;
  b1 ^= b2;
  t.AssertTrue(b1 == BitSet<8>());

  // all 1
  BitSet<8> b3;
  BitSet<8> b4;
  for (size_t i = 0; i < 8; ++i)
  {
    b3[i] = true;
  }
  b3 ^= b3;
  t.AssertTrue(b3 == BitSet<8>());
  b3 ^= b4;
  t.AssertTrue(b3 == BitSet<8>());

  // 1010 ^ 1100 = 0110
  BitSet<8> b5;
  BitSet<8> b6;
  BitSet<8> b7;
  b5[3] = b5[2] = true;
  b6[3] = b6[1] = true;
  b7[2] = b7[1] = true;
  b5 ^= b6;
  t.AssertTrue(b7 == b5);

  // more than one block
  BitSet<130> b8;
  BitSet<130> b9;
  BitSet<130> b10;
  b8[65] = true;
  b9[65] = true;
  b9[120] = true;
  b9 ^= b8;
  t.AssertTrue(b9[120] == true);
  t.AssertTrue(b9[65] == false);
}

static void TestCountBits()
{
  Tester t("TestCountBits");

  BitSet<130> b1;
  size_t count = 0;
  for (int i = 0; i < 130; ++i)
  {
    t.AssertEq(count, b1.CountTrueBits());
    b1[i] = true;
    ++count;
  }
}

static void TestCopy()
{
  Tester t("TestCopy");

  BitSet<130> b1;
  BitSet<130> b2;

  for (int i = 0; i < 130; ++i)
  {
    if (0 == (i % 2))
    {
      b1[i] = true;
    }
  }

  b2 = b1;
  for (int i = 0; i < 130; ++i)
  {
    if (0 == (i % 2))
    {
      t.AssertTrue(b2[i]);
    }
    else
    {
      t.AssertFalse(b2[i]);
    }
  }

  for (int i = 0; i < 130; ++i)
  {
    b2[i] = false;
  }

  t.AssertEq(b2, BitSet<130>());
  t.AssertTrue(b1[0]);

  // b1 still valid
  b1 = b1;
  t.AssertTrue(b1[0]);

  // cctor
  BitSet<130> b3(b1);
  t.AssertTrue(b3[0]);
  t.AssertFalse(b3[1]);
}