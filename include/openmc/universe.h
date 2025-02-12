#ifndef OPENMC_UNIVERSE_H
#define OPENMC_UNIVERSE_H

#include "openmc/cell.h"

namespace openmc {

#ifdef DAGMC
class DAGUniverse;
#endif

class Universe;
class UniversePartitioner;

namespace model {

extern std::unordered_map<int32_t, int32_t> universe_map;
extern vector<unique_ptr<Universe>> universes;

} // namespace model

//==============================================================================
//! A geometry primitive that fills all space and contains cells.
//==============================================================================

class Universe {
public:
  int32_t id_;            //!< Unique ID
  vector<int32_t> cells_; //!< Cells within this universe

  //! \brief Write universe information to an HDF5 group.
  //! \param group_id An HDF5 group id.
  virtual void to_hdf5(hid_t group_id) const;

  virtual bool find_cell(Particle& p) const;

  BoundingBox bounding_box() const;

  const GeometryType& geom_type() const { return geom_type_; }
  GeometryType& geom_type() { return geom_type_; }

  unique_ptr<UniversePartitioner> partitioner_;

private:
  GeometryType geom_type_ = GeometryType::CSG;
};

//==============================================================================
//! Speeds up geometry searches by grouping cells in a search tree.
//
//! The UniversePartitioner is an abstract class. Thus, the actual partitioning 
//! algorithm is implemented in subclasses like ZPlanePartitioner.
//==============================================================================

class UniversePartitioner {
public:
  //! Although the constructor is left to be defined by the derived class,
  //! it should ideally take a Universe as an argument 
  
  //! Dummy destructor 
  virtual ~UniversePartitioner();

  //! Return the list of cells that could contain the given coordinates.
  virtual const vector<int32_t>& get_cells(Position r, Direction u) const = 0;
};

} // namespace openmc
#endif // OPENMC_UNIVERSE_H
