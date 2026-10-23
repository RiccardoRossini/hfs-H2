#!/bin/bash
cd /storage/gpfs_data/famu/riccardo/analysis/hfs-H2
source set_FAMU_env-$1-RV5.sh
root -b -q calcratebatch-$1.cxx