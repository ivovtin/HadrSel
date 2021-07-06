#!/usr/bin/perl -w

use Getopt::Std;
use File::Find;

getopts("hf:t:l:b:d:");
use vars qw/$opt_h $opt_f $opt_t $opt_l $opt_d $opt_b/;

$DefDelta=1;
$DefBatch="remote";
$Bin="exp";
$DefLogPrefix="./$Bin-batch";
$BinDir="/spool/users/kozlova/HadrSel";

if (defined($opt_h)) {
  print "Usage: $0 [-h] -f <run> -t <run> [-l <log>] [-b <query>] [-d <nrun>]\n";
  print "Options: -h - help message\n";
  print "         -f <run> - from run\n";
  print "         -t <run> - to run\n";
  print "         -b <query> - query (def. $DefBatch)\n";
  print "         -d <nrun> - delta for run grouping in one task (def. $DefDelta)\n";
  print "         -l <log> - dir + prefix (def. $DefLogPrefix)\n";
  exit();
}


unless (defined($opt_f)&& defined($opt_t)&&($opt_f<=$opt_t)) {
  print "Usage: $0 [-h] -f <run> -t <run> [-w <dir>]\n";exit(1);}
unless (defined $opt_l) {$opt_l=$DefLogPrefix;}
unless (defined $opt_b) {$opt_b=$DefBatch;}
unless (defined $opt_d) {$opt_d=$DefDelta;}

$cmd="cd $BinDir;  ";
$cmd.= "qsub -q $opt_b -l time=24:00:00 -v KDBHOST=localhost -o \\\$JOB_NAME.\\\$JOB_ID.\\\$TASK_ID -t $opt_f-$opt_t:$opt_d ";

$cmd.=" ./$Bin.pl -f $opt_f -t $opt_t -l $opt_l ";
print "$cmd\n";
system($cmd);
