#!/usr/bin/perl -w
# -------------------------------------------
# --          use perl                     --
#$ -S /usr/bin/perl
# -------------------------------------------
# --             batch name                --
#$ -N exp
# -------------------------------------------
# --     What to redirect to where         --
#$ -cwd
#$ -o $JOB_NAME.$JOB_ID
#$ -j y
# -------------------------------------------
# --             Qeue list                 --
#$ -q extralong
# -------------------------------------------
# --             mail me                   --
#$ -M kozlova@kedr.inp.nsk.su
#$ -m ase
# -------------------------------------------
use Getopt::Std;
use File::Find;
use File::stat;

getopts("hf:t:l:");
use vars qw/$opt_h $opt_f $opt_t $opt_l/;


$Bin="/spool/users/kozlova/bin/Skim";
$DefLogPrefix="./$Bin-batch";


if (defined($opt_h)) {
  print "Usage: $0 [-h] -f <run> -t <run> [-l <dir>]\n";
  print "Options: -h - help message\n";
  print "         -f <run> - from run\n";
  print "         -t <run> - to run\n";
  print "         -l <log> - dir + prefix (def. $DefLogPrefix)\n";
  exit();
}

unless (defined $opt_l) {$opt_l=$DefLogPrefix;}
unless (defined($opt_f)&& defined($opt_t)&&($opt_f<=$opt_t)) {
  print "Usage: $0 [-h] -f <run> -t <run> [-w <dir>]\n";
  exit(1);
}

if (defined($ENV{SGE_TASK_STEPSIZE})&&$ENV{SGE_TASK_STEPSIZE}>1) 
  {$Delta=$ENV{SGE_TASK_STEPSIZE};}
else {$Delta=$opt_t-$opt_f+1;}


#Для запуска массива для выборки скимов
if (defined ($ENV{SGE_TASK_ID})) {
  $opt_f=$ENV{SGE_TASK_ID};
  if ($opt_t>$ENV{SGE_TASK_ID}+$Delta-1) {
    $opt_t=$ENV{SGE_TASK_ID}+$Delta-1;
  }
}

$cmd="KDBHOST=localhost $Bin -f $opt_f -t $opt_t ";
print "$cmd\n";
system($cmd);

