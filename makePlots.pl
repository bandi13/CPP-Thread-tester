#!/usr/bin/perl -w
use strict;
use autodie qw(:all);

my %comps;
$comps{'fusion'} = "Intel Xeon E5420 @ 2.5GHz 4-core (no HT)";
$comps{'andras-Z68'} = "Intel Core i3-2100 @ 3.1GHz 4-core (HT)";
$comps{'andras-X202E'} = "Intel Core i3-3217U @ 1.8GHz 4-core (HT)";
$comps{'tvbox'} = "AMD Athlon 64 X2 3600+ 2-core (no HT)";
$comps{'c0.cs.unh.edu'} = "Intel Xeon X7460 @ 2.66GHz 12-core (no HT)";
$comps{'agate.cs.unh.edu'} = "Intel Xeon E5-2687Wv3 @ 3.1GHz 20-core (no HT)";
$comps{'linux101.mcs.sdsmt.edu'} = "Intel Q6600 @ 2.4GHz 4-core (no HT)";
$comps{'linux02.mcs.sdsmt.edu'} = "Intel i7-2600 @ 3.4GHz 4-core (HT)";
$comps{'skynet.mcs.sdsmt.edu'} = "Intel Xeon E5-2630 @ 2.3CHz 12-core (HT)";
$comps{'guinness'} = "Intel Xeon E5-2687W @ 3.1GHz 16-core (HT)";
$comps{'image.mcs.sdsmt.edu'} = "Intel Q9400 @ 2.7GHz 4-core (no HT)";
$comps{'giotto'} = "Intel E5-4640 256-core (no HT)";

# Generate table that shows the speedup
foreach my $file(<"data/*.csv">) {
	if(!($file =~ m/-speedup.csv$/)) {
		my $outFile = $file;
		$outFile =~ s/\.csv/-speedup.csv/g;
		if(!-e $outFile) {
			my $INFH;
			my $OUTFH;
			open $INFH,$file || die "Can't open file: $file\n";
			open $OUTFH,">$outFile" || die "Can't create file: $outFile\n";
			$_ = <$INFH>;
			my @data = split(/\t/,$_);
			print $OUTFH $data[0] . "\t". join("\t",@data[2..$#data]) . "\n";
			while(<$INFH>) {
				@data = split(/\t/,$_);
				print $OUTFH $data[0];
				for(my $i = 2; $i < $#data; $i++) { print $OUTFH "\t" . ($data[1] / $data[$i]); }
				print $OUTFH "\n";
			}
			close $OUTFH;
			close $INFH;
		}
	}
}

# Plot all the tables
foreach my $file (<"data/*.csv">) {
	if(!-e "$file.png") {
		print "Creating $file.png...\n";
		my $compID = $file;
		$compID =~ s/data\///g;
		$compID =~ s/-2015-.*//g;
		if(defined($comps{$compID})) { $compID = $comps{$compID}; }
		my $testID = $file;
		$testID =~ s/.*\d-//g;
		$testID =~ s/\.csv$//g;
		my $ylabel = "time";
		my $totalPoints = `tail -n 1 $file | awk '{ print NF}'`;
		chomp($totalPoints);
		print "$file -> $totalPoints\n";
		if($testID =~ m/speedup/) { $ylabel = "speedup"; }
		my $PLOT;
		open $PLOT, '|-','gnuplot';
		print $PLOT <<EOF;
set term png
set key inside right top vertical Right noreverse enhanced autotitle columnhead nobox
set style data linespoints
set xtics  norangelimit
set xtics   ()
set title "$compID -- $testID"
set ylabel "$ylabel"
set xlabel "Number of threads"
x = 0.0
set output '$file.png'
EOF
		print $PLOT "plot";
		for(my $i = 2; $i < $totalPoints; $i++) { print $PLOT " '$file' using 1:$i with linespoints,"; }
		print $PLOT " '$file' using 1:$totalPoints with linespoints\n";
#		print $PLOT "f(x) = m*x + b\n";
#		print $PLOT "fit f(x)";
#		for(my $i = 2; $i < 6; $i++) { print $PLOT " '$file' using 1:$i via m,b,"; }
#		print $PLOT " '$file' using 1:6 via m,b\n";
		close $PLOT;
	}
}
