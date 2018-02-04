#!/bin/bash

PLOT_DIR="./GnuPlot"
FILES=($(ls ${PLOT_DIR} -I *.gp))

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

SEP=","
i=0
for f in ${FILES[@]}
do
  FILENAMES+="${PLOT_DIR}/${FILES[${i}]}${SEP}"
  ((i++))
done

gnuplot \
  -e "files='${#FILES[@]}'" -e "f='${FILENAMES}" -e "separator='${SEP}'" ${GRAPHS} \
  -c ${PLOT_DIR}/plot_output.gp