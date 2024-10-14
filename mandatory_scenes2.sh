#!/bin/bash

echo "Running multiple miniRT sessions in parallel..."
echo "This is the mandatory part wuth ambbient light scenes."

# Run each miniRT command in the background and store the PIDs
./miniRT scenes/deep_space1.rt &
pid1=$!

./miniRT scenes/deep_space4.rt &
pid3=$!





# Wait for all background processes to finish
wait $pid1 $pid2 $pid3 

# Optionally, terminate all processes (if needed)
# kill $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7

echo "All miniRT sessions have finished."