#!/usr/bin/perl -w
# Script to run multiple tests on each machine easily.
# Written by: Fekete Andras 2015-03-30
use strict;

my $MAXTHREAD = 32;
my($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime();
$year += 1900;
$mon++;
my $machineName = `uname -n`;
chomp($machineName);
my $fileName = "$machineName-$year-$mon-$mday-$hour-$min";
if(!-d "data") { mkdir("data") || die "Can't create directory for data"; }
my $FILE;
my @progs = qw( asmProc prime );
foreach my $curProg (@progs) {
	open($FILE,">data/$fileName-$curProg.csv") || die "Can't create a test log file.";
	my @str = `./main_$curProg $MAXTHREAD`;
	for(my $i = 0; $i <= $#str; $i++) { print $str[$i]; print $FILE $str[$i]; }
	close($FILE);
}
