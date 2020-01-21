#!/bin/bash

mkdir -p output

geometries="0 1 2 3 4 6"

for var in `seq 100 50 10000`
do
	for geometry in $geometries; do
		( ./acoustic_camera_test -f ${var} -t "$geometry" --dimension 512 -o $(printf output/out_wall_f%06d_t%d.pgm "${var}" "$geometry") ) &
		( ./acoustic_camera_test -f ${var} -t "$geometry" --polar -o $(printf output/out_polar_f%06d_t%d.png "${var}" "$geometry") ) &
	done

	wait
done
