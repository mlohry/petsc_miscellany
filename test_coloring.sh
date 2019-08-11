#!/bin/bash

np=16
executable=../petsc_coloring_from_file
filename=./jacobian_sparsity.dat

declare -a colortypes=("sl" "lf" "jp" "id" "greedy")

for color in "${colortypes[@]}"
do
    echo "color type: ${color}"
    command="time mpiexec -np ${np} ${executable} -f ${filename} -mat_coloring_type ${color} -mat_coloring_view"
    eval $command    
done
