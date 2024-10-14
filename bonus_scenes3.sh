#!/bin/bash

echo "Running multiple miniRT sessions in parallel..."
echo "This is the mandatory part wuth ambbient light scenes."

# Run each miniRT command in the background and store the PIDs
./miniRT scenes/cornellsbox.rt
pid1=$!

discs_uncapped.rt
discs.rt

earth_and_moon.rt

earth_test.rt

./miniRT scenes/logo.rt 

./miniRT scenes/planets.rt

rubik.rt

shadows.rt

# Wait for all background processes to finish
wait $pid1 

# Optionally, terminate all processes (if needed)
# kill $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7

echo "All miniRT sessions have finished."