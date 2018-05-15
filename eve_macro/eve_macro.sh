#!/bin/sh
# A script to run LoadVertexes.cc, LoadPhotonPath.cc and LoadSortedPMTHit.cc with ROOT compiler.
# For more detail, you can check the README.txt
#

# create the folder for output data
if [ ! -d "./eve_data" ]; then
  mkdir ./eve_data
fi

# run the root macro

root -l LoadVertexes.cc++
root -l LoadPhotonPath.cc++
root -l LoadSortedPMTHit.cc++
