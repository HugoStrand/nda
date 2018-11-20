#include "gtest.h"
#include <memory>

#define TRIQS_DEBUG_ARRAYS_MEMORY

#include <nda/storages/handle.hpp>
#include <triqs/test_tools/arrays.hpp>

using namespace nda::mem;

void print(rtable_t const &ta) {
  for (int i = 1; i < ta.nrefs().size(); i++) std::cout << ta.nrefs()[i];
  long nac = ta.nac;
  int u    = 0;
  std::cout << " : ";
  while ((u < 10) and (nac < ta.nrefs().size())) {
    std::cout << nac << "->";
    nac = ta.nrefs()[nac];
    ++u;
  }
  std::cout << nac << std::endl;
}

// test the rtable
TEST(rtable, base) {

  rtable_t ta(5);

  print(ta);

  auto c1 = ta.get();
  EXPECT_EQ(c1, 1);
  print(ta);

  auto c2 = ta.get();
  EXPECT_EQ(c2, 2);
  print(ta);

  ta.incref(c1);
  EXPECT_EQ(ta.nrefs()[c1], 2);
  print(ta);

  ta.decref(c1);
  print(ta);

  auto c3 = ta.get();
  EXPECT_EQ(c3, 3);
  print(ta);

  ta.decref(c1);
  print(ta);

  auto c4 = ta.get();
  EXPECT_EQ(c4, 1);
  print(ta);

  auto c5 = ta.get();
  EXPECT_EQ(c5, 4);
  print(ta);

  auto c6 = ta.get();
  EXPECT_EQ(c6, 5);
  print(ta);

  ta.decref(c6);
  print(ta);
  ta.decref(c5);
  print(ta);
  ta.decref(c4);
  print(ta);
  ta.decref(c3);
  print(ta);
  ta.decref(c2);
  print(ta);
}

//---------------------------------------------

// make sure that all memory is deallocated at the end of each test
class Ref : public ::testing::Test {
  protected:
  void TearDown() override {
    EXPECT_TRUE(globals::rtable.empty());
    EXPECT_TRUE(globals::alloc.empty());
  }
};

TEST_F(Ref, HR) {

  handle<int, 'R'> h{10};

  auto h2 = handle<int, 'R'>{10};

  // make sure it is a copy
  h.data[2] = 89;
  handle<int, 'R'> h3{h};
  h.data[2] = 0;
  EXPECT_EQ(h3.data[2], 89);
}

// ---- Contruct R B
TEST_F(Ref, HBR) {

  handle<int, 'R'> h{10};

  handle<int, 'B'> b{h};
  handle<int, 'B'> b2;
  b2 = h;

  // make sure it is a copy
  b.data[2] = 89;
  handle<int, 'R'> h2{b};
  b.data[2] = 0;
  EXPECT_EQ(h2.data[2], 89);
}

// ---- Construct R, S
TEST_F(Ref, HSR) {

  handle<int, 'R'> h{10};

  handle<int, 'S'> s{h};

  EXPECT_EQ(s.nref(), 2);
}


// --- implicit construction 



// ---- More complex 
TEST_F(Ref, HSRS) {

  handle<int, 'R'> h{10};

  handle<int, 'S'> s{h};
  EXPECT_EQ(s.nref(), 2);

  s = handle<int, 'S'>{h};
  EXPECT_EQ(s.nref(), 2);

  handle<int, 'S'> s2{h};
  s = s2;
  EXPECT_EQ(s.nref(), 3);
}

// ---- check with something that is constructed/destructed.
struct Number {
  int u;
  static inline int c = 0;
  Number() {
    c++;
    std::cerr << "Constructing Number \n";
  };
  ~Number() {
    c--;
    std::cerr << "Destructing Number \n";
  };
};

TEST_F(Ref, HR_with_cd) {
  { handle<Number, 'R'> h{5}; }
  EXPECT_EQ(Number::c, 0);
}

// --- check with a shared_ptr

void release_sp(void *x) {
  auto *p = (std::shared_ptr<Number> *)x;
  p->reset();
  delete p;
}

TEST_F(Ref, HR_with_sharedPtr) {
  {
    handle<Number, 'S'> s;
    s.id = globals::rtable.get();
    s.sptr = (void *)new std::shared_ptr<Number>{new Number{}};
    s.release_fnt = (void*)release_sp;
  }
  EXPECT_EQ(Number::c, 0);
}

MAKE_MAIN;
