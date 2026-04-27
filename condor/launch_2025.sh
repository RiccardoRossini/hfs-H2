#!/bin/bash

cd /storage/gpfs_data/famu/riccardo/analysis/hfs-H2

source /storage/gpfs_data/famu/riccardo/analysis/hfs-H2/set_FAMU_env-202508-RV5.sh
root -b -q calcratebatch-202508.cxx

source /storage/gpfs_data/famu/riccardo/analysis/hfs-H2/set_FAMU_env-202509-RV5.sh
root -b -q calcratebatch-202509.cxx
