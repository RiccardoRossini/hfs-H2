#!/bin/bash

cd /storage/gpfs_data/famu/riccardo/analysis/stat_comparison_del

source /storage/gpfs_data/famu/riccardo/analysis/stat_comparison_del/set_FAMU_env-202404-RV5.sh
root -b -q calcratebatch-202404.cxx

source /storage/gpfs_data/famu/riccardo/analysis/stat_comparison_del/set_FAMU_env-202405-RV5.sh
root -b -q calcratebatch-202405.cxx
