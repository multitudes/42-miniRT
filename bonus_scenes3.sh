#!/bin/bash

echo "Running multiple miniRT sessions in parallel..."
echo "This is the mandatory part wuth ambient light scenes."

# Run each miniRT command in the background and store the PIDs
./miniRT scenes/cornellsbox.rt &
pid1=$!

./miniRT scenes/discs_uncapped.rt &
pid2=$!

./miniRT scenes/discs.rt &
pid3=$!

./miniRT scenes/earth_and_moon.rt &
pid4=$!

./miniRT scenes/earth_test.rt &
pid5=$!

./miniRT scenes/logo.rt &
pid6=$!

./miniRT scenes/planets.rt &
pid7=$!

./miniRT scenes/rubik.rt &
pid8=$!

./miniRT scenes/shadows.rt &
pid9=$!

# Wait for all background processes to finish
wait $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7 $pid8 $pid9

# Optionally, terminate all processes (if needed)
# kill $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7

echo "All miniRT sessions have finished."
