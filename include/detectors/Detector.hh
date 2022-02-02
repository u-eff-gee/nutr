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

	Copyright (C) 2020-2022 Udo Friman-Gayer
*/

#pragma once

#include <vector>

#include "G4LogicalVolume.hh"

using std::vector;

/**
 * \brief Abstract class for a detector with an obvious main axis
 * 
 * This class standardizes the placement of a detector with respect to a point \f$\mathbf{r}_s\f$ 
 * in space (e.g. the location of a radiation source) along a given axis, and the placement of 
 * shielding next to it.
 * 
 * The main axis of the detector will be oriented along an axis defined by the polar (\f$\theta\f$) 
 * and aximuthal (\f$\varphi\f$) angles in spherical coordinates, which are defined with respect
 * to the coordinate system of Geant4.
 * If a 'front part' of the detector can be defined, it is assumed to be facing \f$r_s\f$.
 * The distance of the detector to \f$r_s\f$, measured along the orientation axis, is given by a 
 * parameter \f$d\f$.
 * In most cases, this will be the minimum distance from the source position to the planar surface 
 * of the detector.
 * The class also accounts for intrinsic rotations of the detector by an angle \f$\alpha\f$ around
 * its main axis, so it does not have to be cylindrically symmetric.
 * 
 * It is furthermore assumed that there are well defined ways of placing shielding material 
 * 
 * - in front of (i.e., between the front part of the detector and the radiation source),
 * 
 * and 

 * - around the detector (i.e. 'wrap' the detector in a sheet of shielding material to block
 * incoming radiation from the side).
 * 
 * The former shielding will often be denoted as 'filters', and the latter will be denoted as 
 * 'wrap(pings)'.
 * This class standardizes the definition of both.
 * 
 * \todo Unify the naming of the descriptions and the C++ variables.
 */
class Detector{
	public:
		/**
		 * \brief Constructor
		 * 
         * \param world_logical Initializer for world_logical.
         * \param name Initializer for detector_name.
		 */
		Detector(G4LogicalVolume *World_Logical, const G4String name);

		/**
		 * \brief Construct the detector in a Geant4 geometry
		 * 
		 * The detector is constructed with world_volume as its mother volume.
		 * Within its intrinsic coordinate system, the detector is assumed to be constructed such 
		 * that it is oriented along the z-axis, with the 'center' of its front part exactly at 
		 * the origin for \f$d = 0\f$.
		 * 
		 * Note that filters and wrappings must be added via the respective methods before the 
		 * construction via this method.
		 * 
		 * \param global_coordinates \f$r_s\f$, point in space on which the placement of the 
		 * detector is based.
		 * \param theta \f$\theta\f$ polar angle in spherical coordinates.
		 * \param phi \f$\varphi\f$ polar angle in spherical coordinates.
		 * \param dist_from_center \f$d\f$, distance from the source to the front of the detector.
		 * \param intrinsic_rotation_angle \f$\alpha\f$, intrinsic rotation angle of the detector
		 * around its main axis (default: 0).
		 */
		virtual void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi, G4double dist_from_center, G4double intrinsic_rotation_angle = 0.) = 0;

		/**
		 * \brief Add a filter layer
		 * 
		 * The first filter which is added this way will be the closest to the detector, all 
		 * others will be stacked on top of the first one on the source-facing side.
		 * 
		 * \param filter_material G4Material name
		 * \param filter_thickness Thickness of the filter.
		 * \param filter_radius Radius of the filter. Depending on the detector type, this may have
		 * a different interpretation.
		 */
		void Add_Filter(G4String filter_material, G4double filter_thickness, G4double filter_radius);
		
		/**
		 * \brief Add a wrapping layer
		 * 
		 * The first wrapping which is added this way will be the closest to the detector, all 
		 * others will be wrapped around the first one.
		 * 
		 * Currently, it is assumed that the dimensions of the wrapping are defined by the 
		 * detector dimensions, therefore there is no additional size parameter as in Add_Filter.
		 * 
		 * \param wrap_material G4Material name
		 * \param wrap_thickness Thickness of the wrapping.
		 */
		void Add_Wrap(G4String wrap_material, G4double wrap_thickness);

		/**
		 * \brief Return detector's sensitive logical volumes.
		 */
		vector<G4LogicalVolume*> get_sensitive_logical_volumes(){ return sensitive_logical_volumes; };

	protected:
		G4LogicalVolume *world_Logical; /**< Logical volume in which the detector will be placed. */
		const G4String detector_name; /**< Name of the detector. This name will be used as a prefix for all parts of the geometry. */

		vector<G4String> filter_materials; /**< List of filter materials given as G4Material names. */
		vector<G4double> filter_thicknesses; /**< List of filter thicknesses. */
		vector<G4double> filter_radii; /**< List of filter radii. This assumes that the filters are disks. If they aren't, the filter_radii may be assigned a different meaning which should be explained in the derived class. */

		vector<G4String> wrap_materials; /**< List of filter materials given as G4Material names. */
		vector<G4double> wrap_thicknesses; /**< List of filter thicknesses. */

		/**
		 * \brief Return radial unit vector in spherical coordinates
		 * 
		 * The radial unit vector \f$\hat{e_r}\f$ is the direction along which the main axis of 
		 * the detector is oriented.
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
		 * The polar unit vector \f$\hat{e_\theta}\f$ is perpendicular to the main axis of the 
		 * detector and can be used to place off-axis elements in the geometry.
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
		 * The azimuthal unit vector \f$\hat{e_\theta}\f$ is perpendicular to the main axis of the 
		 * detector and can be used to place off-axis elements in the geometry.
		 * 
		 * \param theta \f$\theta\f$, polar angle
		 * \param phi \f$\varphi\f$, aximuthal angle
		 * 
		 * \return \f$\hat{e_\varphi}\f$
		 */
		G4ThreeVector unit_vector_phi(const double theta, const double phi) const;

		/**
		 * \brief Set up rotation matrix for transformation into the desired coordinate system
		 * 
		 * This function sets the member rotation_matrix to a G4RotationMatrix that will transform
		 * an object with a main axis that was originally oriented along the z axis into an 
		 * orientation along a new axis given by \f$\theta\f$ and \f$\varphi\f$.
		 * Furthermore, a third angle \f$\alpha\f$ can be provided to rotate the object around 
		 * its main axis.
		 * 
		 * \param theta \f$\theta\f$, polar angle
		 * \param phi \f$\varphi\f$, aximuthal angle
		 * \param alpha \f$\alpha\f$, intrinsic rotation angle of the detector
		 * around its main axis
		 */
		void rotate(const double theta, const double phi, const double alpha);

		vector<G4LogicalVolume*> sensitive_logical_volumes; /**< List of sensitive logical volumes of this detector. */
		G4RotationMatrix *rotation_matrix; /**< Rotation matrix for transformation into desired coordinate system.*/
};
