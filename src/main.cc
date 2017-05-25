#include <iostream>
#include <memory>

#include "GMSH.cc"

int main(int argc, char** argv) {
  try {
    if (argc != 3) {
      std::cout << "Usage: ./mpm-point-generator /path/to/mesh_file.msh\n"
                   "Dimenssion 2 or 3\n";
      throw std::runtime_error("Incorrect number of input arguments");
    }

    int dimension = std::atoi(argv[2]);
    std::string filename = argv[1];

    std::unique_ptr<GMSH> mesh(new GMSH());
    mesh->get_vertices(filename);
    mesh->output_vertices();
    mesh->output_stresses();

  } catch (std::exception& except) {
    std::cout << "Caught exception: " << except.what() << '\n';
  }
}
