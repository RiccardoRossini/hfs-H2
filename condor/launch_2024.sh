#!/bin/bash

cd /storage/gpfs_data/famu/riccardo/analysis/hfs-H2

source /storage/gpfs_data/famu/riccardo/analysis/hfs-H2/set_FAMU_env-202404-RV5.sh
root -b -q calcratebatch-202404.cxx

source /storage/gpfs_data/famu/riccardo/analysis/hfs-H2/set_FAMU_env-202405-RV5.sh
root -b -q calcratebatch-202405.cxx
