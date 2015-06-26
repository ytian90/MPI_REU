#!/opt/local/bin/gunplot

#set terminal postscript eps enhanced color font "Helvetica, 10"
#set output 'image_julia.eps'
#set xrange [0:2000]
#set yrange [0:2000]
plot "julia_set.dat" using 1:2:3 with image
#set title "Julia set"

