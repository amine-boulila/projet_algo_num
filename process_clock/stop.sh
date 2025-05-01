#!/bin/sh

# Stop all processes started by run.sh
for EXEC in process_scalar process_vector process_matrix; do
  pkill -f "./$EXEC"
done

echo "All processes have been stopped."