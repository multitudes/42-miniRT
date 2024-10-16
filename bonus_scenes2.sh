#!/bin/bash

echo "Running multiple miniRT sessions in parallel..."
echo "This is the mandatory part wuth ambient light scenes."

# Run each miniRT command in the background and store the PIDs
./miniRT scenes/deep_space_ambient0.rt &
pid1=$!

./miniRT scenes/basic_shapes.rt
pid2=$!

./miniRT scenes/atom.rt
pid3=$!

./miniRT scenes/main_cyl_uncapped_disk.rt
pid4=$!

./miniRT scenes/coloured_lights.rt
pid5=$!

./miniRT scenes/cone_ball.rt
pid6=$!

./miniRT scenes/billard.rt
pid7=$!

./miniRT scenes/cyl_cone_test.rt
pid8=$!

./miniRT scenes/template.rt
pid9=$!

./miniRT scenes/main_checkerfloors.rt
pid10=$!

# Wait for all background processes to finish
wait $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7 $pid8 $pid9 $pid10 

# Optionally, terminate all processes (if needed)
# kill $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7

echo "All miniRT sessions have finished."