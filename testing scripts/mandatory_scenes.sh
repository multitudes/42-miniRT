#!/bin/bash

echo "Running multiple miniRT sessions in parallel..."
echo "This is the mandatory part wuth ambient light scenes."

# Run each miniRT command in the background and store the PIDs
./miniRT scenes/deep_space_ambient0.rt &
pid1=$!

./miniRT scenes/deep_space_ambient0fulllight.rt &
pid2=$!

./miniRT scenes/deep_space_ambient0blue.rt &
pid3=$!

./miniRT scenes/deep_space_ambient0green.rt &
pid4=$!

./miniRT scenes/deep_space_ambient0red.rt &
pid5=$!

./miniRT scenes/deep_space_ambient1.rt &
pid6=$!

./miniRT scenes/deep_space_ambient2.rt &
pid7=$!

./miniRT scenes/deep_space_ambient3.rt &
pid8=$!

./miniRT scenes/ambient_light_test.rt &
pid9=$!

./miniRT scenes/deep_space1.rt &
pid10=$!

./miniRT scenes/deep_space4.rt &
pid11=$!

# Wait for all background processes to finish
wait $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7 $pid8 $pid9 $pid10 $pid11

# Optionally, terminate all processes (if needed)
# kill $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7

echo "All miniRT sessions have finished."