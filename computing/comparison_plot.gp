set terminal wxt size 1000,700
set title 'Comparison of Interpolation Methods'
set xlabel 'X'
set ylabel 'Y'
set grid
set key outside
plot 'original_data.txt' using 1:2 with points pt 7 title 'Data Points', \
     'comparison_data.txt' using 1:2 with lines lw 2 title 'Lagrange', \
     'comparison_data.txt' using 1:3 with lines lw 2 title 'Spline', \
     'comparison_data.txt' using 1:4 with lines lw 2 title 'Tchebyshev'
