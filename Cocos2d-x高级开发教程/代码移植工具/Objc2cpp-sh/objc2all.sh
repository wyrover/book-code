#!/usr/bin/env sh

# WARNING:
# WARNING: This script may destroy your code, make sure to backup before running.
# WARNING:

################################
# convert .m files to .cpp
################################
#ls -1 *.m | while read f
#do
#    mv $f $(basename $f .m).cpp
#done

################################
# convert .mm files to .cpp
################################
#ls -1 *.mm | while read f
#do
#    mv $f $(basename $f .mm).cpp
#done

################################
# loop through files and execute
# objc2cpp.sh on each
################################
SELF_DIR=`dirname $0`

WORKING_DIR=$(pwd)
SCRIPT="$SELF_DIR/objc2cpp.sh"

echo "Current working directory to read files from: $WORKING_DIR"
echo "Will run single-pass script: $WORKING_DIR"

ls -1 $WORKING_DIR/*.{cpp,h} | while read f
do
    $SCRIPT $f
done