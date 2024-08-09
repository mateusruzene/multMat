#!/usr/bin/gnuplot -c
set grid
set style data point
set style function line
set style line 1 lc 3 pt 7 ps 0.3
set boxwidth 1
set xtics
set xrange [64:4500]
set xlabel  "N (bytes)"

set logscale x
# set logscale y

## set datafile separator {whitespace | tab | comma | "<chars>"}
set datafile separator comma

#
# FLOPS_DP
#
ARQ="Resultados/FLOPS_DP.csv"
set key right top
unset logscale y
set ylabel  "FLOPS DP [MFlops/s]"
set title   "FLOPS DP"
set terminal qt 1 title "FLOPS DP"
plot ARQ using 1:2 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:4 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:6 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:8 title "MatMat-ujb" lc rgb "cyan" with linespoints

#
# FLOPS_AVX
#
ARQ="Resultados/FLOPS_DP.csv"
set key right top
unset logscale y
set ylabel  "FLOPS AVX [MFlops/s]"
set title   "FLOPS AVX"
set terminal qt 6 title "FLOPS_AVX"
plot ARQ using 1:3 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:5 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:7 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:9 title "MatMat-ujb" lc rgb "cyan" with linespoints

# pause -1

#
# L3
#
ARQ="Resultados/L3.csv"
set key left top
unset logscale y
set ylabel  "L3 [MBytes/s]"
set title   "L3"
set terminal qt 3 title "L3"
plot ARQ using 1:2 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:3 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:4 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:5 title "MatMat-ujb" lc rgb "cyan" with linespoints

# pause -1

#
# L2CACHE
#
ARQ="Resultados/L2CACHE.csv"
set key right bottom
unset logscale y
set ylabel  "L2 miss ratio"
set title   "L2 miss ratio"
set terminal qt 4 title "L2 miss ratio"
plot ARQ using 1:2 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:3 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:4 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:5 title "MatMat-ujb" lc rgb "cyan" with linespoints

#pause -1

#
# ENERGY
#
ARQ="Resultados/ENERGY.csv"
set key center top
unset logscale y
set ylabel  "Energia [J]"
set title   "Energia"
set terminal qt 2 title "Energia"
plot ARQ using 1:2 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:3 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:4 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:5 title "MatMat-ujb" lc rgb "cyan" with linespoints, \


#
# TIME
#
ARQ="Resultados/Tempos.csv"
set key center top
unset logscale y
set ylabel  "Tempos"
set title   "Tempos"
set terminal qt 5 title "Tempos"
plot ARQ using 1:2 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:3 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:4 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:5 title "MatMat-ujb" lc rgb "cyan" with linespoints, \

pause -1
