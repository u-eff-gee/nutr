/*
    This file is part of nutr.

    nutr is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    nutr is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with nutr.  If not, see <https://www.gnu.org/licenses/>.

        Copyright (C) 2020-2022 Udo Friman-Gayer and Oliver Papst
*/

#pragma once

#include <functional>
#include <limits>
#include <string>
#include <vector>

#include "G4LogicalVolume.hh"

using std::string;
using std::vector;

/**
 * \brief Container for filter properties
 */
struct Filter {
  Filter(const string _material, const double _thickness, const double _radius)
      : custom_filter_shape(nullptr), material(_material),
        thickness(_thickness), radius(_radius), use_default_radius(false),
        use_custom_filter_shape(false) {}
  Filter(const string _material, const double _thickness)
      : custom_filter_shape(nullptr), material(_material),
        thickness(_thickness), radius(std::numeric_limits<double>::quiet_NaN()),
        use_default_radius(true), use_custom_filter_shape(false) {}
  Filter(const string _material,
         const std::function<G4VSolid *(const string)> _custom_filter_shape,
         const double _thickness)
      : custom_filter_shape(_custom_filter_shape), material(_material),
        thickness(_thickness), radius(std::numeric_limits<double>::quiet_NaN()),
        use_default_radius(false), use_custom_filter_shape(true) {}

  std::function<G4VSolid *(const string)> custom_filter_shape;
  string material;  /**< Filter materials given as G4Material names. */
  double thickness; /**< Filter thickness. */
  double radius;    /**< Filter radius. If zero, some default might be used. */
  bool use_default_radius;
  bool use_custom_filter_shape;
};
typedef Filter Wrap;

/**
 * @brief Container for Filter objects and general information about the filter
 * array.
 */
struct FilterConfiguration {
  FilterConfiguration(const vector<Filter> _filters,
                      const bool _use_filter_case)
      : filters(_filters), use_filter_case(_use_filter_case) {}
  FilterConfiguration(const vector<Filter> _filters)
      : filters(_filters), use_filter_case(false) {}
  vector<Filter> filters;
  bool use_filter_case;
};

/**
 * \brief Abstract class for a detector with an obvious main axis
 *
 * This class standardizes the placement of a detector with respect to a point
 \f$\mathbf{r}_s\f$
 * in space (e.g. the location of a radiation source) along a given axis, and
 the placement of
 * shielding next to it.
 *
 * The main axis of the detector will be oriented along an axis defined by the
 polar (\f$\theta\f$)
 * and aximuthal (\f$\varphi\f$) angles in spherical coordinates, which are
 defined with respect
 * to the coordinate system of Geant4.
 * If a 'front part' of the detector can be defined, it is assumed to be facing
 \f$r_s\f$.
 * The distance of the detector to \f$r_s\f$, measured along the orientation
 axis, is given by a
 * parameter \f$d\f$.
 * In most cases, this will be the minimum distance from the source position to
 the planar surface
 * of the detector.
 * The class also accounts for intrinsic rotations of the detector by an angle
 \f$\alpha\f$ around
 * its main axis, so it does not have to be cylindrically symmetric.
 *
 * It is furthermore assumed that there are well defined ways of placing
 shielding material
 *
 * - in front of (i.e., between the front part of the detector and the radiation
 source),
 *
 * and

 * - around the detector (i.e. 'wrap' the detector in a sheet of shielding
 material to block
 * incoming radiation from the side).
 *
 * The former shielding will often be denoted as 'filters', and the latter will
 be denoted as
 * 'wrap(pings)'.
 * This class standardizes the definition of both.
 *
 * \todo Unify the naming of the descriptions and the C++ variables.
 */
class Detector {
public:
  /**
   * \brief Constructor
   *
   * \param name Initializer for detector_name.
   * \param theta \f$\theta\f$ polar angle in spherical coordinates.
   * \param phi \f$\varphi\f$ polar angle in spherical coordinates.
   * \param dist_from_center \f$d\f$, distance from the source to the front of
   * the detector. \param intrinsic_rotation_angle \f$\alpha\f$, intrinsic
   * rotation angle of the detector around its main axis (default: 0).
   */
  Detector(const string _name, const double _theta, const double _phi,
           const double _dist_from_center,
           const FilterConfiguration _filter_configuration = {{}},
           const vector<Wrap> _wraps = {},
           const double _intrinsic_rotation_angle = 0.,
           const vector<double> dead_layer = {},
           const double _default_filter_radius =
               std::numeric_limits<double>::quiet_NaN());

  /**
   * \brief Construct the detector in a Geant4 geometry
   *
   * The detector is constructed with world_volume as its mother volume.
   * Within its intrinsic coordinate system, the detector is assumed to be
   * constructed such that it is oriented along the z-axis, with the 'center' of
   * its front part exactly at the origin for \f$d = 0\f$.
   *
   * Note that filters and wrappings must be added via the respective methods
   * before the construction via this method.
   *
   * \param global_coordinates \f$r_s\f$, point in space on which the placement
   * of the detector is based.
   */
  void Construct(G4LogicalVolume *world_logical,
                 G4ThreeVector global_coordinates);

  /**
   * \brief Return detector's sensitive logical volumes.
   */
  vector<G4LogicalVolume *> get_sensitive_logical_volumes() {
    return sensitive_logical_volumes;
  };

protected:
  double Construct_Filters(G4LogicalVolume *world_logical,
                           G4ThreeVector global_coordinates);
  virtual void Construct_Detector(G4LogicalVolume *world_logical,
                                  G4ThreeVector global_coordinates) = 0;
  virtual void Construct_Filter_Case(G4LogicalVolume *world_logical,
                                     G4ThreeVector global_coordinates,
                                     double filter_dist_from_center) = 0;
  virtual G4VSolid *Filter_Shape(const string name,
                                 const Filter &filter) const = 0;

  const string detector_name; /**< Name of the detector. This name will be used
                                 as a prefix for all parts of the geometry. */
  const double default_filter_radius, theta, phi, dist_from_center;
  const FilterConfiguration
      filter_configuration;   /**< Filters placed in front of the detector. */
  const vector<Filter> wraps; /**< Filters wrapped around the detector face */
  const double intrinsic_rotation_angle;
  const vector<double> dead_layer;

  /**
   * \brief Return radial unit vector in spherical coordinates
   *
   * The radial unit vector \f$\hat{e_r}\f$ is the direction along which the
   * main axis of the detector is oriented.
   *
   * \param theta \f$\theta\f$, polar angle
   * \param phi \f$\varphi\f$, aximuthal angle
   *
   * \return \f$\hat{e_r}\f$
   */
  G4ThreeVector unit_vector_r(const double theta, const double phi) const;

  /**
   * \brief Return polar unit vector in spherical coordinates
   *
   * The polar unit vector \f$\hat{e_\theta}\f$ is perpendicular to the main
   * axis of the detector and can be used to place off-axis elements in the
   * geometry.
   *
   * \param theta \f$\theta\f$, polar angle
   * \param phi \f$\varphi\f$, aximuthal angle
   *
   * \return \f$\hat{e_\theta}\f$
   */
  G4ThreeVector unit_vector_theta(const double theta, const double phi) const;

  /**
   * \brief Return azimuthal unit vector in spherical coordinates
   *
   * The azimuthal unit vector \f$\hat{e_\theta}\f$ is perpendicular to the main
   * axis of the detector and can be used to place off-axis elements in the
   * geometry.
   *
   * \param theta \f$\theta\f$, polar angle
   * \param phi \f$\varphi\f$, aximuthal angle
   *
   * \return \f$\hat{e_\varphi}\f$
   */
  G4ThreeVector unit_vector_phi(const double theta, const double phi) const;

  /**
   * \brief Set up rotation matrix for transformation into the desired
   * coordinate system
   *
   * This function sets the member rotation_matrix to a G4RotationMatrix that
   * will transform an object with a main axis that was originally oriented
   * along the z axis into an orientation along a new axis given by \f$\theta\f$
   * and \f$\varphi\f$. Furthermore, a third angle \f$\alpha\f$ can be provided
   * to rotate the object around its main axis.
   *
   * \param theta \f$\theta\f$, polar angle
   * \param phi \f$\varphi\f$, aximuthal angle
   * \param alpha \f$\alpha\f$, intrinsic rotation angle of the detector
   * around its main axis
   */
  void rotate(const double theta, const double phi, const double alpha);

  vector<G4LogicalVolume *>
      sensitive_logical_volumes; /**< List of sensitive logical volumes of this
                                    detector. */
  G4RotationMatrix *rotation_matrix; /**< Rotation matrix for transformation
                                        into desired coordinate system.*/
};
