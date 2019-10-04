#!/bin/bash
#mpirun -np $1                        -mca btl ^openib a.out
mpirun -np $1 -machinefile hosts.txt -mca btl ^openib fire