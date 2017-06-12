#ifndef MPM_POINT_GEN_IO_H_
#define MPM_POINT_GEN_IO_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "mesh.h"
#include "point.h"

//! \brief Input/Output handler
//! \tparam Tdim dimension
template <unsigned Tdim>
class IO {
 public:
  //! Constructor with input file
  //! \param[in] mesh_file Input mesh file name
  explicit IO(const std::string& mesh_file) : mesh_file_name_{mesh_file} {

    // Check if mesh file is present
    std::ifstream meshfile;
    meshfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      meshfile.open(mesh_file);
    } catch (const std::ifstream::failure& except) {
      std::cerr << "Exception opening/reading mesh file";
    }
    meshfile.close();

    //! Material point and stresses
    material_points_filename_ = "material_points.txt";
    stress_filename_ = "initial_stresses.txt";
  }

  //! Write vertices
  void write_material_points(
      const std::vector<std::shared_ptr<Point<Tdim>>>& materialpoints);

  //! Write stresses
  void write_stresses(
      const std::vector<std::array<double, Tdim * 2>>& stresses);

  //! Return mesh file name
  std::string mesh_file_name() const { return mesh_file_name_; }

 private:
  //! Input mesh file name
  std::string mesh_file_name_;

  //! File name of vertices
  std::string material_points_filename_;

  //! Filename of material point stresses
  std::string stress_filename_;
};

#include "io.tcc"

#endif  // MPM_POINT_GEN_IO_H_
