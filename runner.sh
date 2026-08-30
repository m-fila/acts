#!/bin/bash

numa_node=1

threads=$(seq 1 32)
#threads=(24)

files=(
"/shared/mafila/traccc_data/odd-simulations-20240509/geant4_ttbar_mu20"
"/shared/mafila/traccc_data/odd-simulations-20240509/geant4_ttbar_mu200"
)

# Per-file configuration (same index as files[])
processed_events=(
	10000 
	3000
)
cold_run_events=(
	5000
	1500
)

repeats=(0 1 2)
#repeats=(0)
connections=$(seq 1 32)

for repeat in "${repeats[@]}"; do
  for i in "${!files[@]}"; do
    for connection in $connections; do
      file="${files[$i]}"
      log_file="reco_${repeat}.csv"
      processed_ev="${processed_events[$i]}"
      cold_ev="${cold_run_events[$i]}"
  
      for thread in "${threads[@]}"; do
  
          cmd=(
      env CUDA_DEVICE_MAX_CONNECTIONS=$connection
  	  numactl --cpunodebind=$numa_node --membind=$numa_node
            ./bin/traccc_throughput_mt_cuda
            --input-directory "$file"
            --reco-stage=seeding
            --cpu-threads "$thread"
            --input-events 500
            --cold-run-events "$cold_ev"
            --processed-events "$processed_ev"
            --track-candidates-range=5:100
            --seedfinder-vertex-range=-150:150
            --initial-links-per-seed=6
            --deterministic=on
            --log-file "$log_file"
          )
  
          echo "Running: ${cmd[*]}"
          "${cmd[@]}"
  
        done
    done
  done
done
