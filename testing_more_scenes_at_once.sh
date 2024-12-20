#!/bin/bash

# Run each miniRT command in the background and store the PIDs
./miniRT scenes/cylinder_test.rt &
pid1=$!
./miniRT z_scenes/planets.rt &
pid2=$!
./miniRT scenes/deep_space1.rt &
pid3=$!
./miniRT scenes/deep_space2.rt &
pid4=$!
./miniRT scenes/deep_space3.rt &
pid5=$!
./miniRT scenes/deep_space4.rt &
pid6=$!
./miniRT z_scenes/coordinate_system.rt &
pid7=$!
./miniRT scenes/cone_test.rt &
pid8=$!
./miniRT z_scenes/discs_uncapped.rt &
pid9=$!
./miniRT z_scenes/billard.rt &
pid10=$!
# Wait for all background processes to finish
wait $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7 $pid8 $pid9 $pid10

# Optionally, terminate all processes (if needed)
# kill $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7

echo "All miniRT sessions have finished."