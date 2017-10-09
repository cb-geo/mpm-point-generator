#ifndef MPM_POINT_GEN_IO_H_
#define MPM_POINT_GEN_IO_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include <eigen3/Eigen/Dense>

#include "material_points.h"
#include "mesh.h"

//! Alias for JSON
#include "json.hpp"
using json = nlohmann::json;

//! \brief Input/Output handler
//! \tparam Tdim dimension
template <unsigned Tdim>
class IO {
 public:
  //! Constructor with json input file
  //! Get mesh_filename and output_directory
  //! \param[in] input directory
  //! \param[in] json input file name
  explicit IO(const std::string& file_directory, const std::string& json_file);

  //! Write coordinates
  void write_coordinates(const std::vector<Eigen::VectorXd>& coordinates);

  //! Write stresses
  void write_stresses(const std::vector<Eigen::VectorXd>& stresses);

  //! Return json object for material properties
  json json_file() const { return json_file_; }

  //! Return mesh file name
  std::string mesh_file_name() const { return mesh_filename_; }

 private:
  //! Input directory
  std::string file_directory_;

  //! Input json file
  std::string json_filename_;

  //! json object of material properties to be passed
  json json_file_;

  //! Input mesh file name
  std::string mesh_filename_;

  //! File name of vertices
  std::string material_points_filename_;

  //! Filename of material point stresses
  std::string stress_filename_;
};

#include "io.tcc"

#endif  // MPM_POINT_GEN_IO_H_
