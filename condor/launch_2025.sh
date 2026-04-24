#!/bin/bash

cd /storage/gpfs_data/famu/riccardo/analysis/stat_comparison_del

source /storage/gpfs_data/famu/riccardo/analysis/stat_comparison_del/set_FAMU_env-202508-RV5.sh
root -b -q calcratebatch-202508.cxx

source /storage/gpfs_data/famu/riccardo/analysis/stat_comparison_del/set_FAMU_env-202509-RV5.sh
root -b -q calcratebatch-202509.cxx
