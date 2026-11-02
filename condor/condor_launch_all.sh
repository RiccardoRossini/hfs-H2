#!/bin/bash

# usage: ./submit_all.sh <ndet>      e.g. ./submit_all.sh 10
export NDET=${1:?usage: $0 <ndet>}

# Launch all submitlaunch_20*.sub files in the current directory
for file in submitlaunch_20*.sub
do
    echo 
    echo " SUBMITTING $file"
    condor_submit "$file"
done