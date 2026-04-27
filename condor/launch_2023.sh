#!/bin/bash

cd /storage/gpfs_data/famu/riccardo/analysis/hfs-H2

source /storage/gpfs_data/famu/riccardo/analysis/hfs-H2/set_FAMU_env-202304-RV5.sh
root -b -q calcratebatch-202304.cxx

source /storage/gpfs_data/famu/riccardo/analysis/hfs-H2/set_FAMU_env-202305-RV5.sh
root -b -q calcratebatch-202305.cxx
