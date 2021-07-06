#!/bin/sh
#$ -S /bin/sh
#$ -cwd
#$ -v LD_LIBRARY_PATH=/usr/gridengine/lib/lx24-86:/usr/local/root/lib,ROOTLIBDIR=/usr/local/root/lib,ROOTSYS=/usr/local/root

./exp-batch.pl -f 22468 -t 22570 -b remote -d 1

