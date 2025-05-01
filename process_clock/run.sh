#!/bin/sh

if [ $# -ne 1 ]; then
  echo "Usage: ./run.sh <clock_type>"
  echo "1 = scalar, 2 = vector, 3 = matrix"
  exit 1
fi

CLOCK_TYPE=$1

# Choose the right file
case $CLOCK_TYPE in
  1)
    FILE="process_scalar.c"
    EXEC="process_scalar"
    ;;
  2)
    FILE="process_vector.c"
    EXEC="process_vector"
    ;;
  3)
    FILE="process_matrix.c"
    EXEC="process_matrix"
    ;;
  *)
    echo "Invalid clock type. Use 1 (scalar), 2 (vector), or 3 (matrix)."
    exit 1
    ;;
esac

# Compile the selected file
gcc $FILE -o $EXEC -pthread

# Check for xterm
if ! command -v xterm >/dev/null 2>&1; then
  echo "xterm is not installed. Install it with: sudo apt install xterm"
  exit 1
fi

# Run 4 processes in parallel
for i in 0 1 2 3; do
  xterm -hold -e "./$EXEC $i $CLOCK_TYPE" &
done
