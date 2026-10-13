#!/bin/bash

# Launch all submitlaunch_20*.sub files in the current directory
for file in submitlaunch_20*.sub
do
    echo 
    echo " SUBMITTING $file"
    condor_submit "$file"
done