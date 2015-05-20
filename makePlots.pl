#!/usr/bin/perl -w
use strict;
use autodie qw(:all);

foreach my $file (<"data/*.csv">) {
	if(!-e "$file.png") {
		print "Creating $file.png...\n";
		my $PLOT;
		open $PLOT, '|-','gnuplot';
		print $PLOT <<EOF;
set term png transparent
set key inside right top vertical Right noreverse enhanced autotitle columnhead nobox
set style data linespoints
set xtics  norangelimit
set xtics   ()
set ylabel "time"
set xlabel "Number of threads"
x = 0.0
EOF
		print $PLOT "set output '$file.png'\n";
		print $PLOT "plot";
		for(my $i = 2; $i < 6; $i++) { print $PLOT " '$file' using 1:$i with linespoints,"; }
		print $PLOT " '$file' using 1:6 with linespoints";
		close $PLOT;
	}
}
