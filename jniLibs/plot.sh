#!/bin/bash

PLOT_GRAPHS="Plot_Graphs"
PLOT_SCRIPTS="Plot_Scripts"
FILES=($(ls ${PLOT_GRAPHS} -I *.gp -I *.awk))
SEP=","

i=1
for f in ${FILES[@]}
do
  GRAPH[${i}]="file${i}='${PLOT_GRAPHS}/"${f}"'"
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
  FILENAMES+="${PLOT_GRAPHS}/${FILES[${i}]}${SEP}"
  ((i++))
done

gnuplot \
  -e "files='${#FILES[@]}'" -e "f='${FILENAMES}" -e "separator='${SEP}'" ${GRAPHS} \
  -c ${PLOT_SCRIPTS}/plot_output.gp