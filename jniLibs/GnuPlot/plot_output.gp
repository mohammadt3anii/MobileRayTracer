#!/usr/bin/gnuplot

# define terminal
reset
set datafile separator ","
#set output 'graph.png'
set terminal wxt size 800, 800 enhanced font "Verdana,8" title "Performance comparation" persist raise ctrl
set key outside

# define axis - remove border on top and right and set color to black
set linestyle 1 linecolor rgb 'black' linetype 1
set border 3 back linestyle 1
set tics nomirror

# array to store file names and colors
arrayGet(name, i) = value(sprintf("_%s_%i", name, i))
arraySet(name, i, value) = sprintf("_%s_%i=\"%s\"", name, i, value)
arrayPush(name, value) = arraySet(name, index = index + 1, value)

# colors of graphs
index = 0
eval arrayPush("COLORS", "red")
eval arrayPush("COLORS", "green")
eval arrayPush("COLORS", "blue")
eval arrayPush("COLORS", "purple")
eval arrayPush("COLORS", "orange")
eval arrayPush("COLORS", "brown")
eval arrayPush("COLORS", "white")

# files to plot
index = 0
startSep=0
endSep = startSep + strstrt(f[startSep:], separator)
fx = f[startSep : endSep - 1]
eval arrayPush("FILES", fx)
endSep = endSep + 1

do for [i=2:files] {
	startSep = endSep
	endSep = startSep + strstrt(f[startSep:], separator)
	fx = f[startSep : endSep - 2]
	eval arrayPush("FILES", fx)
}

# stats to get min and max values
X_min = 0
X_max = 0
Y_min = 0
Y_max = 0

do for [i=1:files] {
	file = arrayGet("FILES", i)
	stats file using 1 nooutput name 'Fx_'
	stats file using 2 nooutput name 'Fy_'

	X_min = Fx_min < X_min? Fx_min : X_min
	X_max = Fx_max > X_max? Fx_max : X_max
	Y_min = Fy_min < Y_min? Fy_min : Y_min
	Y_max = Fy_max > Y_max? Fy_max : Y_max
}

# axis labels
set xlabel '#Threads'
set xrange [0:0<*]
set xtics X_min, 1, X_max offset graph 0, graph 0

set ylabel 'Time (ms)'
set yrange [0:0<*]
set ytics Y_min, 100, Y_max offset graph 0, graph 0

# line definitions
set linestyle 1 pointtype 7 pointsize 1.0 linetype 3 linewidth 2.5 dashtype 3

# plot
plot \
for [i = 1 : files] \
	name = arrayGet("FILES", i) name using 1:2 title name[11:] \
	with linespoints linestyle 1 linecolor rgb arrayGet("COLORS", i)