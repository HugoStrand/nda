#include <gtest/gtest.h> // NOLINT
#include <cmath>
#include <limits>

#define NDA_BOUNDCHECK
#define NDA_ENFORCE_BOUNDCHECK

#include <nda/nda.hpp>
#include <nda/gtest_tools.hpp>

using namespace std::complex_literals;

template <int R>
using myshape_t = nda::shape_t<R>;

// variables for the test
nda::range_all _;
nda::ellipsis ___;

using nda::range;
using dcomplex = std::complex<double>;
using nda::C_layout;
using nda::F_layout;

#define MAKE_MAIN_MPI                                                                                                                                \
  int main(int argc, char **argv) {                                                                                                                  \
    ::mpi::environment env(argc, argv);                                                                                                              \
    ::testing::InitGoogleTest(&argc, argv);                                                                                                          \
    return RUN_ALL_TESTS();                                                                                                                          \
  }

#define MAKE_MAIN                                                                                                                                    \
  int main(int argc, char **argv) {                                                                                                                  \
    ::testing::InitGoogleTest(&argc, argv);                                                                                                          \
    return RUN_ALL_TESTS();                                                                                                                          \
  }

#define EXPECT_PRINT(X, Y)                                                                                                                           \
  {                                                                                                                                                  \
    std::stringstream ss;                                                                                                                            \
    ss << Y;                                                                                                                                         \
    EXPECT_EQ(X, ss.str());                                                                                                                          \
  }
