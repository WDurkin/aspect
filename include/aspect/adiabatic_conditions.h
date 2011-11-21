//-------------------------------------------------------------
//    $Id: simulator.h 232 2011-10-19 13:30:15Z bangerth $
//
//    Copyright (C) 2011 by the authors of the ASPECT code
//
//-------------------------------------------------------------
#ifndef __aspect__adiabatic_conditions_h
#define __aspect__adiabatic_conditions_h


#include <aspect/material_model_base.h>
#include <aspect/geometry_model_base.h>
#include <aspect/gravity_model_base.h>
#include <deal.II/base/point.h>


namespace aspect
{
  using namespace dealii;


  /**
   * A class that represents adiabatic conditions, i.e. that starts at the top of
   * the domain and integrates pressure and temperature as we go down into depth.
   *
   * @note The implementation has numerous deficiencies indicated in the .cc file
   * and may not quite compute what we want. Specifically, it doesn't currently
   * take into account all the physical parameters it needs, and it also doesn't
   * get gravity right with the exception of the simplest cases.
   */
  template <int dim>
  class AdiabaticConditions
  {
    public:
      /**
       * Constructor. Compute the adiabatic conditions along a vertical
       * transect of the geometry based on the given material model.
       */
      AdiabaticConditions (const GeometryModel::Interface<dim> &geometry_model,
                           const GravityModel::Interface<dim>  &gravity_model,
                           const MaterialModel::Interface<dim> &material_model);

      /**
       * Return the adiabatic temperature at a given point of the domain.
       */
      double temperature (const Point<dim> &p) const;

      /**
       * Return the adiabatic pressure at a given point of the domain.
       */
      double pressure (const Point<dim> &p) const;

    private:
      /**
       * Number of points at which we compute the adiabatic values.
       */
      const unsigned int n_points;

      /**
       * Vectors of values of temperatures and pressures on a transect into
       * depth at which we have computed them. The public member functions
       * of this class interpolate linearly between these points.
       */
      std::vector<double> temperatures, pressures;

      /**
       * Interval spacing between each two data points in the tables above
       * with regard to the depth coordinate.
       */
      double delta_z;

      /**
       * Function object that can be used to convert from a given point to
       * the (positive) depth with which we can look up in the table what the
       * corresponding conditions would be.
       *
       * This object is set in the constructor where we have knowledge of the
       * geometry object in use.
       */
      std_cxx1x::function<double (const Point<dim>&)> point_to_depth_converter;
  };

}


#endif