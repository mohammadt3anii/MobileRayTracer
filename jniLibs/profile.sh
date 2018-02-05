PLOT_GRAPHS="Plot_Graphs"
PLOT_SCRIPTS="Plot_Scripts"

OBJ="/mnt/D/Android_Studio_Projects/MobileRayTracer/app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.obj"
MTL="/mnt/D/Android_Studio_Projects/MobileRayTracer/app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.mtl"
#OBJ="/mnt/D/Android_Studio_Projects/MobileRayTracer/app/src/main/assets/WavefrontOBJs/conference/conference.obj"
#MTL="/mnt/D/Android_Studio_Projects/MobileRayTracer/app/src/main/assets/WavefrontOBJs/conference/conference.mtl"

SPP="1"
SPL="1"
WIDTH="800"
HEIGHT="800"
REP="1"
PRINT="false"
ASYNC="false"
SHOWIMAGE="false"
SEP="-"

THREADS="1 2 3 4"
SHADERS="0 1 2"
SCENES="0 1 2 3"
ACCELERATORS="0 1 2"
for THREAD in ${THREADS[@]}
do
  for SHADER in ${SHADERS[@]}
  do
    for SCENE in ${SCENES[@]}
    do
      for ACC in ${ACCELERATORS[@]}
      do
        echo ""
        echo "THREAD = "${THREAD}
        echo "SHADER = "${SHADER}
        echo "SCENE = "${SCENE}
        echo "ACC = "${ACC}

        PLOT_FILE="SC${SCENE}${SEP}SH${SHADER}${SEP}A${ACC}${SEP}R${WIDTH}x${HEIGHT}"

        #perf script report callgrind > perf.callgrind
        #kcachegrind perf.callgrind
        #perf stat
        #perf record -g --call-graph 'fp' -- \
        #./raytracer \
        #${THREAD} ${SHADER} ${SCENE} ${SPP} ${SPL} ${WIDTH} ${HEIGHT} ${ACC} ${REP} \
        #${OBJ} ${MTL} ${PRINT} ${ASYNC} ${SHOWIMAGE} \
        #| awk -v threads="${THREAD}" -f ${PLOT_SCRIPTS}/parser_out.awk 2>&1 \
        #| tee -a ${PLOT_GRAPHS}/${PLOT_FILE}.txt
        #perf report -g '' --show-nr-samples --hierarchy

      done
    done
  done
done

bash plot.sh