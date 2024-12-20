#!/bin/bash

echo "Running multiple miniRT sessions in parallel..."
echo "This is the mandatory part wuth ambbient light scenes."

# Run each miniRT command in the background and store the PIDs

./miniRT scenes/deep_space6.rt &
pid1=$!

./miniRT scenes/deep_space5.rt &
pid2=$!

./miniRT scenes/deep_space3.rt &
pid3=$!

./miniRT scenes/deep_space2.rt &
pid4=$!

./miniRT scenes/checker_floor.rt
pid5=$!

./miniRT scenes/checker_sphere.rt &
pid6=$!

./miniRT scenes/cone_test2.rt &
pid7=$!

./miniRT scenes/coordinate_system.rt &
pid8=$!

./miniRT scenes/check_colors.rt &
pid9=$!

./miniRT scenes/progress.rt &
pid10=$!

# Wait for all background processes to finish
wait $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7 $pid8 $pid9 $pid10


# Optionally, terminate all processes (if needed)
# kill $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7

echo "All miniRT sessions have finished."