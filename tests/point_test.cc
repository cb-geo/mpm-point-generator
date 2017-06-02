// Point test
#include <array>
#include <limits>

#include "catch.hpp"
#include "point.h"

//! \brief Check point class in 2D
TEST_CASE("Point base is checked in 2D", "[Point][2D]") {

  //! 2D Test of Point Class
  const double tol = 1.E-12;
  const unsigned DIM = 2;

  //! at coordinate zeroes
  SECTION("2D test at zeros") {
    unsigned index = 0;
    std::array<double, DIM> coordinate{0, 0};
    auto node = std::unique_ptr<Point<DIM>>(new Point<DIM>(index, coordinate));

    REQUIRE(node->id() == 0);
    REQUIRE(node->coordinates().at(0) == Approx(0).epsilon(tol));
    REQUIRE(node->coordinates().at(1) == Approx(0).epsilon(tol));
  }

  //! at minimum limit
  SECTION("2D test at minimum limits") {
    unsigned index = std::numeric_limits<unsigned>::min();
    std::array<double, DIM> coordinate{std::numeric_limits<double>::min(),
                                       std::numeric_limits<double>::min()};
    auto node = std::unique_ptr<Point<DIM>>(new Point<DIM>(index, coordinate));

    REQUIRE(node->id() == std::numeric_limits<unsigned>::min());
    REQUIRE(node->coordinates().at(0) ==
            Approx(std::numeric_limits<double>::min()).epsilon(tol));
    REQUIRE(node->coordinates().at(1) ==
            Approx(std::numeric_limits<double>::min()).epsilon(tol));
  }

  //! at maximum limit
  SECTION("2D test at zeros") {
    unsigned index = std::numeric_limits<unsigned>::max();
    std::array<double, DIM> coordinate{std::numeric_limits<double>::max(),
                                       std::numeric_limits<double>::max()};
    auto node = std::unique_ptr<Point<DIM>>(new Point<DIM>(index, coordinate));

    REQUIRE(node->id() == std::numeric_limits<unsigned>::max());
    REQUIRE(node->coordinates().at(0) ==
            Approx(std::numeric_limits<double>::max()).epsilon(tol));
    REQUIRE(node->coordinates().at(1) ==
            Approx(std::numeric_limits<double>::max()).epsilon(tol));
  }
}

//! \brief Check point class in 3D
TEST_CASE("Point base is checked in 3D", "[Point][3D]") {

  //! 3D Test of Point Class
  const double tol = 1.E-12;
  const unsigned DIM = 3;

  //! at coordinate zeroes
  SECTION("3D test at maximum limits") {
    unsigned index = 0;
    std::array<double, DIM> coordinate{0, 0, 0};
    auto node = std::unique_ptr<Point<DIM>>(new Point<DIM>(index, coordinate));

    REQUIRE(node->id() == 0);
    REQUIRE(node->coordinates().at(0) == Approx(0).epsilon(tol));
    REQUIRE(node->coordinates().at(1) == Approx(0).epsilon(tol));
    REQUIRE(node->coordinates().at(2) == Approx(0).epsilon(tol));
  }

  //! at minimum limit
  SECTION("3D test at zeros") {
    unsigned index = std::numeric_limits<unsigned>::min();
    std::array<double, DIM> coordinate{std::numeric_limits<double>::min(),
                                       std::numeric_limits<double>::min(),
                                       std::numeric_limits<double>::min()};
    auto node = std::unique_ptr<Point<DIM>>(new Point<DIM>(index, coordinate));

    REQUIRE(node->id() == std::numeric_limits<unsigned>::min());
    REQUIRE(node->coordinates().at(0) ==
            Approx(std::numeric_limits<double>::min()).epsilon(tol));
    REQUIRE(node->coordinates().at(1) ==
            Approx(std::numeric_limits<double>::min()).epsilon(tol));
    REQUIRE(node->coordinates().at(2) ==
            Approx(std::numeric_limits<double>::min()).epsilon(tol));
  }

  //! at maximum limit
  SECTION("3D test at zeros") {
    unsigned index = std::numeric_limits<unsigned>::max();
    std::array<double, DIM> coordinate{std::numeric_limits<double>::max(),
                                       std::numeric_limits<double>::max(),
                                       std::numeric_limits<double>::max()};
    auto node = std::unique_ptr<Point<DIM>>(new Point<DIM>(index, coordinate));

    REQUIRE(node->id() == std::numeric_limits<unsigned>::max());
    REQUIRE(node->coordinates().at(0) ==
            Approx(std::numeric_limits<double>::max()).epsilon(tol));
    REQUIRE(node->coordinates().at(1) ==
            Approx(std::numeric_limits<double>::max()).epsilon(tol));
    REQUIRE(node->coordinates().at(2) ==
            Approx(std::numeric_limits<double>::max()).epsilon(tol));
  }
}