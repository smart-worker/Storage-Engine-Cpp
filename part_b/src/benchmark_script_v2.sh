#!/bin/bash

PORT=9002  # Change this to the Redis server port if needed
RESULTS_DIR="results"
mkdir -p $RESULTS_DIR

concurrent_requests=(10000 100000 1000000)
parallel_connections=(10 100 1000)

# Perform all SET benchmarks first
for req in "${concurrent_requests[@]}"; do
    for con in "${parallel_connections[@]}"; do
        output_file="$RESULTS_DIR/result_${req}_${con}.txt"
        > "$output_file"  # Clear the file before writing
        
        echo "Running SET benchmark with $req requests and $con connections..."
        redis-benchmark -h 127.0.0.1 -p $PORT -t set -n $req -c $con -r 100000000 --csv -q > "$output_file"
        echo "SET Results stored in $output_file"
    done
done

# Perform all GET benchmarks after all SET benchmarks are done
for req in "${concurrent_requests[@]}"; do
    for con in "${parallel_connections[@]}"; do
        output_file="$RESULTS_DIR/result_${req}_${con}.txt"
        
        echo "Running GET benchmark with $req requests and $con connections..."
        redis-benchmark -h 127.0.0.1 -p $PORT -t get -n $req -c $con -r 100000000 --csv | tail -n 1 >> "$output_file"
        echo "GET Results appended to $output_file"
    done
done
