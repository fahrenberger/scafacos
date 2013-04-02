program main
  use, intrinsic :: iso_c_binding
  implicit none
  include "mpif.h"
  include "fftw3-mpi.f03"
  include "pfft.f03"

  integer np(2)
  integer(C_INTPTR_T) :: n(3)
  integer(C_INTPTR_T) :: alloc_local
  integer(C_INTPTR_T) :: local_ni(3), local_i_start(3)
  integer(C_INTPTR_T) :: local_no(3), local_o_start(3)
  double precision err
  complex(C_DOUBLE_COMPLEX), pointer :: in(:), out(:)
  type(C_PTR) :: plan_forw, plan_back, cin, cout
  integer comm_cart_2d

  integer myrank, ierror

  n  = [31,27,29]
  np = [2,2]

  ! Initialize MPI and PFFT
  call MPI_Init(ierror)
  call pfft_init()
  call MPI_Comm_rank(MPI_COMM_WORLD, myrank, ierror)

if(myrank .eq. 0) then
  write(*,*) "Create procmesh"
endif

  ! Create two-dimensional process grid of size np(1) x np(2), if possible
  ierror =  pfft_create_procmesh(2, MPI_COMM_WORLD, np, comm_cart_2d)
  if (ierror .ne. 0) then
    if(myrank .eq. 0) then
      write(*,*) "Error: This test file only works with ", np(1)*np(2), " processes"
    endif
    call MPI_Finalize(ierror)
    call exit(1)
  endif

  ! Get parameters of data distribution
  alloc_local = pfft_local_size_dft_3d(n, comm_cart_2d, PFFT_TRANSPOSED_NONE, &
      local_ni, local_i_start, local_no, local_o_start);

  write(*,*) "local_ni = (", local_ni(1), ", ", local_ni(2), ", ", local_ni(3)
  write(*,*) "local_i_start = (", local_i_start(1), ", ", local_i_start(2), ", ", local_i_start(3)
  write(*,*) "local_no = (", local_no(1), ", ", local_no(2), ", ", local_no(3)
  write(*,*) "local_o_start = (", local_o_start(1), ", ", local_o_start(2), ", ", local_o_start(3)

if(myrank .eq. 0) then
  write(*,*) "Allocate memory, alloc_local = ", alloc_local
endif

  ! Allocate memory
  cin  = pfft_alloc_complex(alloc_local)
  cout = pfft_alloc_complex(alloc_local)

  ! Convert data pointers to Fortran format
  call c_f_pointer(cin,  in,  [local_ni(3), local_ni(2), local_ni(1)])
  call c_f_pointer(cout, out, [local_no(3), local_no(2), local_no(1)])

if(myrank .eq. 0) then
  write(*,*) "Plan Forw PFFT"
endif

  ! Plan parallel forward FFT
  plan_forw = pfft_plan_dft_3d( &
      n, in, out, comm_cart_2d, PFFT_FORWARD, PFFT_TRANSPOSED_NONE + PFFT_MEASURE + PFFT_DESTROY_INPUT)

if(myrank .eq. 0) then
  write(*,*) "Plan Back PFFT"
endif

  ! Plan parallel backward FFT
  plan_back = pfft_plan_dft_3d( &
      n, out, in, comm_cart_2d, PFFT_BACKWARD, PFFT_TRANSPOSED_NONE + PFFT_MEASURE + PFFT_DESTROY_INPUT)

  ! Initialize input with random numbers
  call pfft_init_input_c2c_3d(n, local_ni, local_i_start, &
      in)

if(myrank .eq. 0) then
  write(*,*) "Exec Forw PFFT"
endif

  ! Execute parallel forward FFT
  call pfft_execute(plan_forw)
  
if(myrank .eq. 0) then
  write(*,*) "Exec Back PFFT"
endif

  ! Execute parallel backward FFT
  call pfft_execute(plan_back)

if(myrank .eq. 0) then
  write(*,*) "Scale output"
endif

  ! Scale data
  in = in / (n(1)*n(2)*n(3))
  
  ! Print error of back transformed data
  err = pfft_check_output_c2c_3d(n, local_ni, local_i_start, in, comm_cart_2d)
  if(myrank .eq. 0) then
    write(*,*) "Error after one forward and backward trafo of size n=(", n(1), ", ", n(2), ", ", n(3), "):"
    write(*,*) "maxerror = ", err
  endif 

if(myrank .eq. 0) then
  write(*,*) "Finalize"
endif

  ! Free mem and finalize
  call pfft_destroy_plan(plan_forw)
  call pfft_destroy_plan(plan_back)
  call MPI_Comm_free(comm_cart_2d, ierror)
  call pfft_free(cin)
  call pfft_free(cout)
  call MPI_Finalize(ierror)
end program main

