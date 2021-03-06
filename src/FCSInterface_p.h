/*
  Copyright (C) 2011-2012 Rene Halver

  This file is part of ScaFaCoS.

  ScaFaCoS is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ScaFaCoS is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser Public License for more details.

  You should have received a copy of the GNU Lesser Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/



#ifndef FCS_INTERFACE_P_INCLUDED
#define FCS_INTERFACE_P_INCLUDED

#include <mpi.h>

#include "FCSDefinitions.h"
#include "FCSResult_p.h"

/**
 * @file FCSInterface_p.h
 * @brief supplying the C-interface routine for the ScaFaCoS library
 * @author Rene Halver, Olaf Lenz
 */

/**
 * @brief a handle for the FCS-object which should be used instead of the
 * data structure itself
 */
typedef struct FCS_t *FCS;

#define FCS_NULL  NULL

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief initialization routine for the basic parameters needed to
 * run any method within ScaFaCoS, as well as method specific initialization
 * @param handle FCS* pointer to an FCS handle that will represent the FCS solver object
 * will be saved
 * @param method character string selecting the method to be used
 * @param communicator MPI communicator to be used by handle / method
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_init (FCS *handle, const char* method, MPI_Comm communicator);

/**
 * @brief initialization routine for the basic parameters needed to
 * run any method within ScaFaCoS, as well as method specific initialization
 * (FORTRAN2003 wrapper function)
 * @param handle pointer to a void pointer, which will be returned to FORTRAN,
 * after being used as space for a FCS (see fcs_init)
 * @param method character string selecting the method to be used
 * @param communicator fcs_int to be used as a MPI communicator in FORTRAN
 * @return FCSResult-object containing the return state
 */
 FCSResult fcs_init_f (FCS *handle, const char* method, MPI_Fint communicator);

/**
 * @brief function to set the obligatory information into a
 * given FCS
 * @param handle FCS handle representing an FCS solver object
 * @param near_field_flag integer whether near field computations
 * should be performed by the method (value 1) or not (value 0)
 * @param box_a float* first base vector of system box
 * @param box_b float* second base vector of system box
 * @param box_c float* third base vector of system box
 * @param offset float* position (vector) of system box (origin)
 * @param periodicity int* periodicity of the system
 * @param total_particles integer giving the total number of
 * particles in the system
 * @return FCSResult-object containing the return state or NULL if successful
 */
FCSResult fcs_set_common(FCS handle, fcs_int near_field_flag, fcs_float* box_a, fcs_float* box_b, fcs_float* box_c, fcs_float* offset, fcs_int* periodicity, fcs_int total_particles);

/**
 * @brief getter function for method
 * @param handle FCS handle representing an FCS solver object
 * @return character string containing the method
 */
fcs_int fcs_get_method(FCS handle);

/**
 * @brief getter function for method
 * @param handle FCS handle representing an FCS solver object
 * @return MPI_Comm containing the communicator
 */
MPI_Comm fcs_get_communicator(FCS handle);

/**
 * @brief setter function for system dimension
 * @param handle FCS handle representing an FCS solver object
 * @param dim fcs_int dimension of the system (1-3)
 * @return FCSResult-object containing the return state or NULL if successful
 */
FCSResult fcs_set_dimension(FCS handle, fcs_int dim);

/**
 * @brief getter function for method
 * @param handle FCS handle representing an FCS solver object
 * @return fcs_int containing the system dimension (1-3)
 */
fcs_int fcs_get_dimension(FCS handle);

/**
 * @brief setter function for near field flag
 * @param handle FCS handle representing an FCS solver object
 * @param near_field_flag whether near field computations
 * should be performed by the method (value 1) or not (value 0)
 * @return FCSResult-object containing the return state or NULL if successful
 * */
FCSResult fcs_set_near_field_flag(FCS handle, fcs_int near_field_flag);

/**
 * @brief getter function for method
 * @param handle FCS handle representing an FCS solver object
 * @return whether near field computations
 * should be performed by the method (value 1) or not (value 0)
 */
fcs_int fcs_get_near_field_flag(FCS handle);

/**
 * @brief setter function for first box vector
 * @param handle FCS handle representing an FCS solver object
 * @param box_a fcs_float pointer to be inserted
 * @return FCSResult-object containing the return state or NULL if successful
 * */
FCSResult fcs_set_box_a(FCS handle, fcs_float* box_a);

/**
 * @brief getter function for method
 * @param handle FCS handle representing an FCS solver object
 * @return fcs_float* containing the first box vector
 */
fcs_float* fcs_get_box_a(FCS handle);

/**
 * @brief setter function for second box vector
 * @param handle FCS handle representing an FCS solver object
 * @param box_b fcs_float pointer to be inserted
 * @return FCSResult-object containing the return state or NULL if successful
 * */
FCSResult fcs_set_box_b(FCS handle, fcs_float* box_b);

/**
 * @brief getter function for method
 * @param handle FCS handle representing an FCS solver object
 * @return fcs_float* containing the second box vector
 */
fcs_float* fcs_get_box_b(FCS handle);

/**
 * @brief setter function for third box vector
 * @param handle FCS handle representing an FCS solver object
 * @param box_c fcs_float pointer to be inserted
 * @return FCSResult-object containing the return state or NULL if successful
 * */
FCSResult fcs_set_box_c(FCS handle, fcs_float* box_c);

/**
 * @brief getter function for method
 * @param handle FCS handle representing an FCS solver object
 * @return fcs_float* containing the third box vector
 */
fcs_float* fcs_get_box_c(FCS handle);

/**
 * @brief setter function for offset of the box
 * @param handle FCS handle representing an FCS solver object
 * @param offset fcs_float pointer: [0.0,0.0,0.0] - <left lower forward corner of original system box>
 * @return FCSResult-object containing the return state or NULL if successful
 * */
FCSResult fcs_set_offset(FCS handle, fcs_float* offset);

/**
 * @brief getter function for offset
 * @param handle FCS handle representing an FCS solver object
 * @return fcs_float* containing the offset
 */
fcs_float* fcs_get_offset(FCS handle);

/**
 * @brief setter function for periodicity of the box
 * @param handle FCS handle representing an FCS solver object
 * @param periodicitiy fcs_int pointer to be inserted
 * @return FCSResult-object containing the return state or NULL if successful
 * */
FCSResult fcs_set_periodicity(FCS handle, fcs_int* periodicity);

/**
 * @brief getter function for periodicity
 * @param handle FCS handle representing an FCS solver object
 * @return fcs_int* containing the periodicity
 */
fcs_int* fcs_get_periodicity(FCS handle);

/**
 * @brief setter function for number of total particles
 * @param handle FCS handle representing an FCS solver object
 * @param total_particles fcs_int to be inserted
 * @return FCSResult-object containing the return state or NULL if successful
 */
FCSResult fcs_set_total_particles(FCS handle, fcs_int total_particles);

/**
 * @brief getter function for method
 * @param handle FCS handle representing an FCS solver object
 * @return fcs_int containing the total particle count
 */
fcs_int fcs_get_total_particles(FCS handle);

/**
 * @brief function for formatted output of passed FCS-object on stdout
 * @param handle FCS handle representing an FCS solver object
 */
void fcs_printHandle(FCS handle);

/**
 * @brief function to set method context informations
 * @param handle FCS handle representing an FCS solver object
 * @param pointer to method context informations
 * @return FCSResult-object containing the return state or NULL if successful
 */
FCSResult fcs_set_method_context(FCS handle, void *method_context);

/**
 * @brief function to get method context informations
 * @param handle FCS handle representing an FCS solver object
 * @return pointer to method context informations
 */
void *fcs_get_method_context(FCS handle);

/**
 * @brief tuning routine where method specific parameters can be tuned,
 * that depend on local particle numbers, etc.
 * @param handle FCS handle representing an FCS solver object
 * @param local_particles fcs_int number of local particles
 * @param local_max_particles fcs_int estimation for maximum amount of particles on local process,
 * e.g. for first array allocation
 * @param positions fcs_float* containing position data for particle based optimizations
 * @param charges fcs_float* containing charges for charge based optimizations
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_tune (FCS handle, fcs_int local_particles, fcs_int local_max_particles,
 fcs_float *positions,  fcs_float *charges);

/**
 * @brief the method to actually calculate with the chosen method, if parameters changed or method
 * is not tuned yet call fcs_tune()
 * if a method modifies positions or charges, it has to create copies
 * positions must contain local_particles*3 fcs_float values
 * charges must contain local_particles fcs_float values
 * @param handle FCS handle representing an FCS solver object
 * @param local_particles the amount of particles on local process
 * @param local_max_particles fcs_int estimation for maximum amount of particles on local process,
 * e.g. for first array allocation (only used if fcs_tune() must be called)
 * @param positions fcs_float* containing position data
 * @param charges fcs_float* containing charges
 * @param output FCSOutput-object pointer in whose space the FCSOutput-object will be created
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_run (FCS handle, fcs_int local_particles,  fcs_int local_max_particles,
 fcs_float *positions, fcs_float *charges, fcs_float *field, fcs_float *potentials );

/**
 * @brief returns whether the method requires a near field or not
 * @param handle FCS handle representing an FCS solver object
 * @param has_near whether or not the method provides a near field
 * component or not
 * @return FCSResult-object containing the return state
 * 
 * Note that it is in the responsibility of the user of this function
 * to make sure that the function will get values of r between 0.0 and
 * the cutoff range. Values outside of this range might result in
 * undefined behavior.
 */
FCSResult fcs_method_has_near(FCS handle, fcs_int *has_near);

/**
 * @brief compute the near-field component of the potential of the method
 * @param handle FCS handle representing an FCS solver object
 * @param dist the distance of both charges
 * @param potential pointer to the fcs_float where the resulting
 * potential will be written. Note that the function will return
 * 1/dist, not q1q2/dist
 * @return FCSResult-object containing the return state
 * 
 * Note that it is in the responsibility of the user of this function
 * to make sure that the function will get values of r between 0.0 and
 * the cutoff range. Values outside of this range might result in
 * undefined behavior.
 */
FCSResult fcs_compute_near_potential(FCS handle, fcs_float dist, fcs_float* potential);

/**
 * @brief compute the near-field component of the field of the method
 * @param handle FCS handle representing an FCS solver object
 * @param dist the distance of both charges
 * @param field pointer to the fcs_float where the resulting
 * field will be written. Note that the function will return
 * 1/dist, not q1q2/dist
 * @return FCSResult-object containing the return state
 * 
 * Note that it is in the responsibility of the user of this function
 * to make sure that the function will get values of r between 0.0 and
 * the cutoff range. Values outside of this range might result in
 * undefined behavior.
 */
FCSResult fcs_compute_near_field(FCS handle, fcs_float dist, fcs_float* field);

/**
 * @brief compute the near-field components of field and potential of the method
 * @param handle FCS handle representing an FCS solver object
 * @param dist the distance of both charges
 * @param potential pointer to the fcs_float where the resulting
 * potential will be written. Note that the function will return
 * 1/dist, not q1q2/dist
 * @param field pointer to the fcs_float where the resulting
 * field will be written. Note that the function will return
 * 1/dist, not q1q2/dist
 * @return FCSResult-object containing the return state
 * 
 * Note that it is in the responsibility of the user of this function
 * to make sure that the function will get values of r between 0.0 and
 * the cutoff range. Values outside of this range might result in
 * undefined behavior.
 */
FCSResult fcs_compute_near(FCS handle, fcs_float dist, fcs_float* potential, fcs_float* field);

/**
 * @brief compute the correction to the field and total energy when
 * periodic boundary conditions with a finite dielectric constant of
 * the surrounding medium epsilon are used.
 * @param handle FCS handle representing an FCS solver object
 * @param local_particles number of particles in local process
 * @param positions Array of 3*local_particles values that contains the
 * positions of the charges. These positions should not be folded into
 * the central box.
 * @param charges Array of local_particles values that contains the
 * value of the charges.
 * @param epsilon Value of the dielectric constant of the surrounding
 * medium. For metallic boundary conditions, use -1.0 (or simply do
 * not use this function!).
 * @param field_correction Array of 3 fcs_float values. When the
 * function returns, the array contains the required correction to the
 * field values caused by the dipole term.
 * @param energy_correction Pointer to an fcs_float variable. When the
 * function returns, the variable contains the required correction to
 * the total energy caused by the dipole term.
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_compute_dipole_correction(FCS handle,
					fcs_int local_particles,
					fcs_float* positions, fcs_float *charges, 
					fcs_float epsilon,
					fcs_float *field_correction,
					fcs_float *energy_correction);

/**
 * @brief parser for string-based input into the FCS-object
 * @param handle FCS handle representing an FCS solver object
 * @param parameters char* string containing the parameters
 * (format: "<1st variable_name>,<1st comma-separated value(s)>,<2nd variable_name>,<2nd comma-separated value(s)>,...")
 * @param continue_on_errors fcs_bool if true, then the parser continues when a setter fails
 * @return FCSResult-object containing the return state or NULL if successful
 */
FCSResult fcs_parser(FCS handle, const char *parameters, fcs_bool continue_on_errors);

/**
 * @brief Destroy a FCS method object.
 * @param handle FCS handle representing an FCS solver object
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_destroy(FCS handle);

/**
 * @brief function to activate computation of the virial
 * @param handle FCS handle representing an FCS solver object
 * @param flag whether or not to compute the virial in the next call
 * to fcs_run().
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_require_virial(FCS handle, fcs_int flag);

/**
 * @brief function to fetch the virial
 * @param handle FCS handle representing an FCS solver object
 * @param virial pointer to the array where the virial is returned.
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_get_virial(FCS handle, fcs_float *virial);

/**
 * @brief function to set the nearfield cutoff radius
 * @param handle FCS handle representing an FCS solver object
 * @param r_cut float that contains the value of the cutoff radius
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_set_r_cut(FCS handle, fcs_float r_cut);

/**
 * @brief function to deactivate user defined nearfield cutoff radius
 * @param handle FCS handle representing an FCS solver object
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_unset_r_cut(FCS handle);

/**
 * @brief function to fetch the nearfield cutoff radius
 * @param handle FCS handle representing an FCS solver object
 * @param r_cut pointer to the float where the cutoff radius is returned
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_get_r_cut(FCS handle, fcs_float *r_cut);

/**
 * @brief function to set the error tolerance
 * @param handle FCS handle representing an FCS solver object
 * @param tolerance_type FCS constant that determines the type of tolerance
 * @param tolerance_value value of error tolerance
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_set_tolerance(FCS handle, fcs_int tolerance_type, fcs_float tolerance);

/**
 * @brief function to fetch the error tolerance
 * @param handle FCS handle representing an FCS solver object
 * @param tolerance_type FCS constant that determines the type of tolerance
 * @param tolerance_value value of error tolerance
 * @return FCSResult-object containing the return state
 */
FCSResult fcs_get_tolerance(FCS handle, fcs_int *tolerance_type, fcs_float *tolerance_value);

/**
 * @brief set max. distance particles are away from the domain of the local process, e.g. because they have moved since the last grid sorting
 * @param handle FCS handle representing an FCS solver object
 * @param max_particle_move fcs_float max. distance particles have moved
 */
FCSResult fcs_set_max_particle_move(FCS handle, fcs_float max_particle_move);

/**
 * @brief function to set whether resort support is requested or not (default is no resort support)
 *   if resort support is requested (and supported by the solver) then the solver tries to retain its sorted particle data order,
 *   i.e., the distribution and order of the given particles is changed by fcs_run such that the position, charge, field, and potential values correspond to the sorted particle order of the solver,
 *   this can only be performed successfully if the local sizes of the particle arrays (specified by the local_max_particles of fcs_run) on all processes are large enough to store the sorted particle order,
 *   after performing fcs_run, function fcs_get_resort_availability can be used to determine whether resort support is available or not,
 *   if resort support is available then fcs_get_resort_particles returns the new local number of particles and fcs_resort_[ints,floats,bytes] can be used to bring additional particle data into the new sorted order
 * @param handle FCS handle representing an FCS solver object
 * @param resort fcs_int whether resort support is requested or not
 */
FCSResult fcs_set_resort(FCS handle, fcs_int resort);

/**
 * @brief function to retrieve whether resort support is requested or not
 * @param handle FCS handle representing an FCS solver object
 * @param resort fcs_int* whether resort support is requested or not
 */
FCSResult fcs_get_resort(FCS handle, fcs_int *resort);

/**
 * @brief function to retrieve whether resort support is available after performing fcs_run
 * @param handle FCS handle representing an FCS solver object
 * @param availability fcs_int* whether resort support is available or not
 */
FCSResult fcs_get_resort_availability(FCS handle, fcs_int *availability);

/**
 * @brief function to retrieve the new local number of particles
 * @param handle FCS handle representing an FCS solver object
 * @param availability fcs_int* new local number of particles
 */
FCSResult fcs_get_resort_particles(FCS handle, fcs_int *resort_particles);

/**
 * @brief function to bring additional integer particle data into the new sorted particle order
 * @param handle FCS handle representing an FCS solver object
 * @param src fcs_int* array of integer values in unsorted (original) order
 * @param dst fcs_int* array to store sorted integer values
 * @param n fcs_int number of integer values for each particle
 */
FCSResult fcs_resort_ints(FCS handle, fcs_int *src, fcs_int *dst, fcs_int n);

/**
 * @brief function to bring additional float particle data into the new sorted particle order
 * @param handle FCS handle representing an FCS solver object
 * @param src fcs_float* array of float values in unsorted (original) order
 * @param dst fcs_float* array to store sorted float values
 * @param n fcs_int number of float values for each particle
 */
FCSResult fcs_resort_floats(FCS handle, fcs_float *src, fcs_float *dst, fcs_int n);

/**
 * @brief function to bring additional byte particle data into the new sorted particle order
 * @param handle FCS handle representing an FCS solver object
 * @param src void* array of byte values in unsorted (original) order
 * @param dst void* array to store sorted byte values
 * @param n fcs_int number of byte values for each particle
 */
FCSResult fcs_resort_bytes(FCS handle, void *src, void *dst, fcs_int n);


#ifdef __cplusplus
}
#endif

#endif
