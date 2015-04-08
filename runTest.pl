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
my $fileName = "$machineName-$year-$mon-$mday-$hour:$min";
if(!-d "data") { mkdir("data") || die "Can't create directory for data"; }
my $FILE;
open($FILE,">data/$fileName.csv") || die "Can't create a test log file.";
&printPlaces($FILE, "numThread\t"); # get the header looking like in the program
&printPlaces($FILE,`./main 1`);
for(my $i = 2; $i <= $MAXTHREAD; $i++) {
  &printPlaces($FILE,"$i\t" . `./main $i | sed -n '2p'`); # ignores the header
}
close($FILE);

sub printPlaces() {
  my $FILE = shift;
  my $str = shift;
  print $str;
  print $FILE $str;
}
