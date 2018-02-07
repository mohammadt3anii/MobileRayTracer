#!/bin/bash

PLOT_SCRIPTS="Plot_Scripts"

if [ -z "${PLOT_GRAPHS}" ]; then
  PLOT_GRAPHS="Plot_Graphs"
fi
mkdir -p ${PLOT_GRAPHS}

for FOLDER in ${PLOT_GRAPHS[@]}
do
  FILES+=($(find ${FOLDER} -type f))
done

OBJ="/mnt/D/Android_Studio_Projects/MobileRayTracer/app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.obj"
MTL="/mnt/D/Android_Studio_Projects/MobileRayTracer/app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.mtl"
#OBJ="/mnt/D/Android_Studio_Projects/MobileRayTracer/app/src/main/assets/WavefrontOBJs/conference/conference.obj"
#MTL="/mnt/D/Android_Studio_Projects/MobileRayTracer/app/src/main/assets/WavefrontOBJs/conference/conference.mtl"

export ASAN_OPTIONS="suppressions=sanitizer_ignore.suppr:verbosity=1:strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:halt_on_error=0:detect_odr_violation=1"

export LSAN_OPTIONS="suppressions=sanitizer_ignore.suppr:verbosity=1:strict_string_checks=1"

SPP="1"
SPL="1"
WIDTH="800"
HEIGHT="800"
REP="1"
PRINT="false"
ASYNC="false"
SHOWIMAGE="false"
SEP="-"

THREADS="1"
REPETITIONS="10"

SHADERS="1 2"
SCENES="2"
ACCELERATORS="1 2"

function execute {
  THREAD="1"
  SHADER="1"
  SCENE="2"
  ACC="2"
  PRINT="true"
  SHOWIMAGE="true"
  echo ""
  echo "THREAD = "${THREAD}
  echo "SHADER = "${SHADER}
  echo "SCENE = "${SCENE}
  echo "ACC = "${ACC}

  #perf script report callgrind > perf.callgrind
  #kcachegrind perf.callgrind
  #perf stat \
  #perf record -g --call-graph 'fp' -- \
  ./raytracer \
            ${THREAD} ${SHADER} ${SCENE} ${SPP} ${SPL} ${WIDTH} ${HEIGHT} ${ACC} ${REP} \
            ${OBJ} ${MTL} ${PRINT} ${ASYNC} ${SHOWIMAGE}
  #perf report -g '' --show-nr-samples --hierarchy
}

function profile {
  trap "exit" INT
  for R in `seq 1 ${REPETITIONS}`;
  do
    for THREAD in ${THREADS[@]}
    do
      for SHADER in ${SHADERS[@]}
      do
        for SCENE in ${SCENES[@]}
        do
          for ACC in ${ACCELERATORS[@]}
          do
            echo ""
            echo "REPETITION = ${R}"
            echo "THREAD = "${THREAD}
            echo "SHADER = "${SHADER}
            echo "SCENE = "${SCENE}
            echo "ACC = "${ACC}

            PLOT_FILE="SC${SCENE}${SEP}SH${SHADER}${SEP}A${ACC}${SEP}R${WIDTH}x${HEIGHT}"

            ./raytracer \
            ${THREAD} ${SHADER} ${SCENE} ${SPP} ${SPL} ${WIDTH} ${HEIGHT} ${ACC} ${REP} \
            ${OBJ} ${MTL} ${PRINT} ${ASYNC} ${SHOWIMAGE} \
            | awk -v threads="${THREAD}" -f ${PLOT_SCRIPTS}/parser_out.awk 2>&1 \
            | tee -a ${PLOT_GRAPHS}/${PLOT_FILE}.dat

          done
        done
      done
    done
    ((R++))
  done
}

PARAM1="time"
PARAM2="drawt"
PARAM3="draws"
PARAM4="test"
PARAM5="exec"

for P in ${@}
do
  case ${P} in
    ${PARAM1}) profile; sleep 2s ;;
    ${PARAM2}) . ${PLOT_SCRIPTS}/plot.sh 0;;
    ${PARAM3}) . ${PLOT_SCRIPTS}/plot.sh 1;;
    ${PARAM4}) awk -f "${PLOT_SCRIPTS}/parser_median.awk" "${PLOT_SCRIPTS}/test.dat"  ;;
    ${PARAM5}) execute ;;
    *) echo ""
       echo "Wrong Parameter: ${P}"
       echo "The valid parameters are:"
       echo "${PARAM1} - Profile application and log the measured times."
       echo "${PARAM2} - Draw a graph with GNU Plot."
       break
       ;;
  esac
done