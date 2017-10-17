//! Return a vector of coordinates
//! \tparam Tdim Dimension of the mesh
//! \tparam Tvertices Number of vertices in an element
template <unsigned Tdim, unsigned Tvertices>
std::vector<Eigen::VectorXd> Mesh<Tdim, Tvertices>::coordinates() {

  std::vector<Eigen::VectorXd> coordinates;

  // Iterate over materialpoints_ to get coordinates
  for (const auto& materialpoint : materialpoints_) {
    for (const auto& coordinate : materialpoint->coordinates()) {
      coordinates.emplace_back(coordinate);
    }
  }
  return coordinates;
}

//! Return a vector of stresses
//! \tparam Tdim Dimension of the mesh
//! \tparam Tvertices Number of vertices in an element
template <unsigned Tdim, unsigned Tvertices>
std::vector<Eigen::VectorXd> Mesh<Tdim, Tvertices>::stress() {

  std::vector<Eigen::VectorXd> stresses;

  // Iterate over materialpoints_ to get stress
  for (const auto& materialpoint : materialpoints_) {
    for (const auto& stress : materialpoint->stress()) {
      stresses.emplace_back(stress);
    }
  }
  return stresses;
}

//! Assign material properties to MaterialPoints
//! \tparam Tdim Dimension of the mesh
//! \tparam Tvertices Number of vertices in an element
//! \param[in] material pointer to MaterialProperties
template <unsigned Tdim, unsigned Tvertices>
void Mesh<Tdim, Tvertices>::assign_material_properties(
    const std::shared_ptr<MaterialProperties>& material) {

  for (const auto& materialpoint : materialpoints_) {
    materialpoint->assign_material_properties(material);
  }
}

//! Compute stress of material points
//! \tparam Tdim Dimension of the mesh
//! \tparam Tvertices Number of vertices in an element
template <unsigned Tdim, unsigned Tvertices>
void Mesh<Tdim, Tvertices>::compute_stresses() {

  for (const auto& materialpoint : materialpoints_) {
    materialpoint->compute_stress();
  }
}