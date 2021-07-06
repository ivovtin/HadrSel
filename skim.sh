
#!sh
#$ -S /bin/bash
#$ -v KDBHOST=bison-2
#$ -cwd
../bin/Skim -f 23206 -t 23500
