#ifndef MPM_POINT_GEN_IO_H_
#define MPM_POINT_GEN_IO_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "mesh.h"
#include "point.h"

#include "json.hpp"

//! \brief Input/Output handler
class IO {
 public:
  //! Constructor with input file
  //! \param[in] pathfile_name Input file name
  explicit IO(const std::string& pathfile_name) {

    try {
      //! Open path file
      std::ifstream pathfile(pathfile_name);
      pathfile.exceptions(std::ifstream::badbit);

      //! Read file and store to private variables
      path_input_ = nlohmann::json::parse(pathfile);
      inputfilename_ = path_input_["inputfile"].get<std::string>();
      outputfilename_vertex_ =
          path_input_["outputfile_vertex"].get<std::string>();
      outputfilename_stress_ =
          path_input_["outputfile_stress"].get<std::string>();

    } catch (std::exception& except) {
      std::cout << "Caught exception: " << except.what() << '\n'
                << "Please check the path file.\n";
    }
  }

  //! Write vertices
  void write_vertices(const std::vector<std::shared_ptr<Point<3>>>& vertices);

  //! Write stresses
  void write_stresses(const std::vector<std::array<double, 6>>& stress);

  //! \brief Get the private properties
  //! return input file name
  std::string inputfilename() const { return inputfilename_; }

 private:
  //! json for the input data to contain paths to other inputs and outputs
  nlohmann::json path_input_;

  //! filename of the input
  std::string inputfilename_;

  //! filename of the output for vertex
  std::string outputfilename_vertex_;

  //! filename of the output for stress
  std::string outputfilename_stress_;
};

#include "io.tcc"

#endif  // MPM_POINT_GEN_IO_H_
