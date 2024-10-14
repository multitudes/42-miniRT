#!/bin/bash

echo "Running multiple miniRT sessions in parallel..."
echo "This is the mandatory part wuth ambbient light scenes."

# Run each miniRT command in the background and store the PIDs
./miniRT scenes/deep_space_ambient0.rt &
pid1=$!

./miniRT scenes/basic_shapes.rt

./miniRT scenes/atom.rt



./miniRT scenes/coloured_lights.rt

./miniRT scenes/cone_ball.rt

./miniRT scenes/billard.rt


# Wait for all background processes to finish
wait $pid1 

# Optionally, terminate all processes (if needed)
# kill $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7

echo "All miniRT sessions have finished."