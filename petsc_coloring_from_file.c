
static char help[] = " \n\n";
/*
 Example:
       mpiexec -np 16 ./petsc_coloring_from_file -f ./jacobian_sparsity.dat -mat_coloring_type jp -mat_coloring_view -log_view
 */

#include <petscmat.h>
#include <stdio.h>

int main(int argc, char** args)
{
  PetscBool flag;
  PetscMPIInt rank;
  PetscMPIInt nranks;
  PetscViewer fd;
  PetscErrorCode ierr;
  char file[PETSC_MAX_PATH_LEN];  /* input file name */

  PetscInitialize(&argc, &args, (char*)0, 0);
  MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
  MPI_Comm_size(PETSC_COMM_WORLD, &nranks);

  PetscOptionsGetString(NULL,NULL,"-f",file,PETSC_MAX_PATH_LEN,&flag);
  if (!flag){
    SETERRQ(PETSC_COMM_WORLD,1,"Must indicate binary file with the -f option");
  }
  PetscViewerBinaryOpen(PETSC_COMM_WORLD, file, FILE_MODE_READ, &fd);

  /* Load the matrix; then destroy the viewer. */
  Mat            Asparse;
  ierr = MatCreate(PETSC_COMM_WORLD,&Asparse);CHKERRQ(ierr);
  ierr = MatSetFromOptions(Asparse);CHKERRQ(ierr);
  ierr = MatLoad(Asparse,fd);CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&fd);CHKERRQ(ierr);

  PetscPrintf(PETSC_COMM_WORLD, "Loaded matrix file %s\n", file);
  PetscPrintf(PETSC_COMM_WORLD, "Computing coloring.\n");

  MatColoring coloring;
  ISColoring iscoloring;
  MatColoringCreate(Asparse, &coloring);
  MatColoringSetFromOptions(coloring);
  MatColoringApply(coloring, &iscoloring);
  MatColoringDestroy(&coloring);

  PetscFinalize();
  return 0;
}
