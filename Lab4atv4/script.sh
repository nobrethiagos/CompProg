#!/usr/bin/bash

threads=(1 2 3 4 5 6 7 8 9 10)
repeticao=(1 2 3 4 5 6 7 8 9 10)
echo "N,numThreads,tempo"
for i in ${threads[@]}; do
  for j in ${repeticao[@]}; do
    ./primos 1000 $i
  done
done
for k in ${threads[@]}; do
  for l in ${repeticao[@]}; do
    ./primos 1000000 $k
  done
done
