#!/bin/bash

PLOT_DIR="./GnuPlot"
FILES=($(ls ${PLOT_DIR} -I *.gp))
SEP=","

i=1
for f in ${FILES[@]}
do
  GRAPH[${i}]="file${i}='${PLOT_DIR}/"${f}"'"
  ((i++))
done

i=1
for f in ${GRAPH[@]}
do
  GRAPHS+=" -e ${GRAPH[${i}]}"
  ((i++))
done

i=0
for f in ${FILES[@]}
do
  FILENAMES+="${PLOT_DIR}/${FILES[${i}]}${SEP}"
  ((i++))
done

THREADS=1
#threads, shader, scene, sP, sL, width, height, acc, rep,
time ./raytracer \
  "${THREADS}", "1", "4", "1", "1", "800", "800", "1", "1", \
  "/mnt/D/Android_Studio_Projects/MobileRayTracer/app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.obj" \
  "/mnt/D/Android_Studio_Projects/MobileRayTracer/app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.mtl" \
  "true", "false", "true"
#print, async, showImage

#gnuplot \
#  -e "files='${#FILES[@]}'" -e "f='${FILENAMES}" -e "separator='${SEP}'" ${GRAPHS} \
#  -c ${PLOT_DIR}/plot_output.gp