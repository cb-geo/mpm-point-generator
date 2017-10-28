//! Read mesh
//! \tparam Tdim Dimension
//! \tparam Tvertices Number of vertices in element
//! \param[in] filename Input mesh filename
template <unsigned Tdim, unsigned Tvertices>
void GMSH<Tdim, Tvertices>::read_mesh(const std::string& filename) {

  std::ifstream file;
  file.open(filename.c_str(), std::ios::in);
  if (!file.is_open())
    throw std::runtime_error("Specified GMSH file does not exist");
  if (file.good()) {
    read_vertices(file);
    read_elements(file);
  }
  file.close();
}

//! Read keyword
//! \tparam Tdim Dimension
//! \tparam Tvertices Number of vertices in element
//! \param[in] filename Input mesh filename
//! \param[in] keyword search for the word of interest
template <unsigned Tdim, unsigned Tvertices>
void GMSH<Tdim, Tvertices>::read_keyword(std::ifstream& file,
                                         const std::string& keyword) {

  bool read_status = false;
  std::string line;
  file.clear();
  file.seekg(0, std::ios::beg);

  //! The loop is trying to find specific keyword and get the line number
  //! The code will obtain the data after the specific keyword
  //! Should the keyword is not found, it terminates and sends error messsage

  while (std::getline(file, line)) {
    if (line != keyword) {
      if (line.find(keyword) != std::string::npos) {
        break;
      };
    } else {
      std::cout << "Read keyword: " << keyword << " successfully\n";
      read_status = true;
      break;
    }
  }
  if (!read_status) std::cerr << "Cannot find keyword: " << keyword << '\n';
}

//! Read vertex id and coordinates in GMSH
//! \tparam Tdim Dimension
//! \tparam Tvertices Number of vertices in element
//! \param[in] filename Input mesh filename
template <unsigned Tdim, unsigned Tvertices>
void GMSH<Tdim, Tvertices>::read_vertices(std::ifstream& file) {

  //! Find the line of interest
  read_keyword(file, "$Nodes");

  std::string line;
  std::getline(file, line);
  std::istringstream istream(line);

  //! Read number of vertices
  unsigned nvertices = std::numeric_limits<unsigned>::max();
  istream >> nvertices;
  getline(istream, line);

  //! Vertices id
  unsigned vertid = 0;

  //! Read vertex coordinates & id
  for (unsigned i = 0; i < nvertices; ++i) {
    std::getline(file, line);
    std::istringstream istream(line);

    if (line.find('#') == std::string::npos && line != "") {
      //! Coordinates of vertex
      Eigen::VectorXd vertex(Tdim);

      istream >> vertid;

      if (Tdim == 3) {
        istream >> vertex[0] >> vertex[1] >> vertex[2];
      } else {
        istream >> vertex[0] >> vertex[1];
      }
      this->vertices_.insert(std::make_pair(vertid, vertex));
    }
  }

  this->nvertices_ = vertices_.size();

  //! Check that the number of vertices are correct
  if (nvertices_ != nvertices)
    std::cout << "Error: number of vertices do not match.\n";

  std::cout << "Number of Vertices: " << nvertices_ << '\n';
}

//! Read GMSH elements
//! \tparam Tdim Dimension
//! \tparam Tvertices Number of vertices in element
//! \param[in] filename Input mesh filename and directory
template <unsigned Tdim, unsigned Tvertices>
void GMSH<Tdim, Tvertices>::read_elements(std::ifstream& file) {

  //! Find the line of interest
  read_keyword(file, "$Elements");

  std::string line;
  std::getline(file, line);
  std::istringstream istream(line);

  //! Number of elements
  unsigned nelements = std::numeric_limits<unsigned>::max();
  istream >> nelements;
  getline(istream, line);

  //! Element type
  double elementtype = std::numeric_limits<double>::max();

  double physical = std::numeric_limits<double>::max();
  double elementry = std::numeric_limits<double>::max();

  //! Element id
  unsigned elementid = std::numeric_limits<unsigned>::max();

  //! Array to store vertices coordinates
  Eigen::VectorXd elementarray(Tvertices);

  //! specify element type 4 = tetrahedral, 5 = hexahedron
  //! Documentation from GMSH
  //! 2 - Triangle (3 nodes)
  //! 3 - Quadrangle (4 nodes)
  //! 4 - Tetrahedron (4 nodes)
  //! 5 - Hexahedron (8 nodes)
  //! For more informtion on element types, visit:
  //! http://gmsh.info/doc/texinfo/gmsh.html#File-formats
  const unsigned element_type = 5;

  //! Iterate through all elements in the file
  for (unsigned i = 0; i < nelements; ++i) {
    std::getline(file, line);
    std::istringstream istream(line);
    if (line.find('#') == std::string::npos && line != "") {
      istream >> elementid >> elementtype >> elementry >> physical >> elementry;

      //! If element type not equals to specified Tvertices, skip element
      if (elementtype != element_type) {
        istream >> line;
      } else {
        istream >> elementarray[0] >> elementarray[1] >> elementarray[2] >>
            elementarray[3] >> elementarray[4] >> elementarray[5] >>
            elementarray[6] >> elementarray[7];
        this->elements_.emplace_back(new Element(elementid, elementarray));
      }
    }
  }
  std::cout << "Number of Elements: " << elements_.size() << '\n';

  //! Get the coordinates for each vertex of each element
  this->store_element_vertices();
}

//! Store element vertices
//! \tparam Tdim Dimension
//! \tparam Tvertices Number of vertices in element
template <unsigned Tdim, unsigned Tvertices>
void GMSH<Tdim, Tvertices>::store_element_vertices() {

  Eigen::VectorXd elementkeyvalues(Tvertices);

  //! Iterate through element_
  for (const auto& element : elements_) {

    std::vector<Eigen::Vector3d> verticescoordinates;

    //! Iterate through the vertices to get coordinates depending on the element
    for (unsigned j = 0; j < Tvertices; ++j) {
      //! Get the vertex wanted from the id
      const auto vertex_id = element->vertex_id(j);
      //! Get the coordinates for the required vertex idz
      auto verticesfind = vertices_.find(vertex_id);

      //! For each vertex, store the coordinates
      verticescoordinates.push_back(verticesfind->second);
    }

    element->coordinates(verticescoordinates);
  }
  std::cout
      << "The coordinates for vertices of each element have been stored.\n";
}

//! Compute material points based on the centroid
//! \tparam Tdim Dimension
//! \tparam Tvertices Number of vertices in element
//! \param[in] ngauss_points Number of gauss points per coordinate
template <unsigned Tdim, unsigned Tvertices>
void GMSH<Tdim, Tvertices>::compute_material_points(unsigned ngauss_points) {

  //! Storing ngauss_points to member variable and get constants from namespace
  ngauss_points_ = ngauss_points;
  std::vector<double> gauss_constants =
      element::gauss_points.find(ngauss_points_)->second;

  //! Create a matrix of xi from gauss points
  //! Matrix is size npoints x Tdim
  //! For 3D only
  unsigned npoints = std::pow(ngauss_points_, Tdim);
  Eigen::MatrixXd xi_gauss_points(npoints, Tdim);
  unsigned counter = 0;
  for (unsigned ii = 0; ii < ngauss_points_; ++ii) {
    for (unsigned jj = 0; jj < ngauss_points_; ++jj) {
      for (unsigned kk = 0; kk < ngauss_points_; ++kk) {
        xi_gauss_points(counter, 0) = gauss_constants.at(ii);
        xi_gauss_points(counter, 1) = gauss_constants.at(jj);
        xi_gauss_points(counter, 2) = gauss_constants.at(kk);
        ++counter;
      }
    }
  }

  Eigen::VectorXd pointsarray(Tdim);

  //! last_global_id should be changed later if more than one material
  //! properties are used
  //! material_id is the index of materialpoints
  unsigned last_global_id = 0;
  unsigned material_id = 0;

  //! Update vector of material points
  //! Fill materialpoints_ vector for the first component
  materialpoints_.emplace_back(std::unique_ptr<MaterialPoints<Tdim>>(
      new MaterialPoints<Tdim>(material_id)));

  for (const auto& element : elements_) {

    //! Store coordinates in Tdim x Tvertices matrix
    //! Where N is the number of nodes per element
    //! This is rearranging of the data to have stored in matrix form
    Eigen::MatrixXd node_coordinates(Tdim, Tvertices);
    for (unsigned i = 0; i < Tvertices; ++i) {
      for (unsigned j = 0; j < Tdim; ++j) {
        node_coordinates(j, i) = element->vertex_coordinates(i)[j];
      }
    }

    //! Compute the gauss points (there are npoints)
    for (unsigned k = 0; k < npoints; ++k) {

      //! Get array of xi for this gauss point
      std::array<double, Tdim> xi;
      for (unsigned l = 0; l < Tdim; ++l) xi.at(l) = xi_gauss_points(k, l);

      // Compute gauss point in cartesian coordinate
      for (unsigned i = 0; i < Tdim; ++i) {
        pointsarray[i] = 0;
        Eigen::VectorXd shape_function = element::hexahedron::shapefn(xi);
        for (unsigned j = 0; j < Tvertices; ++j) {
          pointsarray[i] += shape_function[j] * node_coordinates(i, j);
        }
      }

      //! Make class point and store to material points
      materialpoints_.at(material_id)
          ->add_points(std::unique_ptr<Point<Tdim>>(new Point<Tdim>(
              element->id(), element->id() + last_global_id, pointsarray)));
    }
  }

  //! Find number of material points generated
  unsigned nmaterialpoints = 0;

  for (const auto& materialpoint : materialpoints_)
    nmaterialpoints += materialpoint->coordinates().size();

  std::cout << "Number of Material Points: " << nmaterialpoints << '\n';
}
