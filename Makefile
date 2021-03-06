#--------------------------------------------------------------------------
# ��������� 3 ������ ����� �� �������
ifeq (,$(WORKDIR))
WORKDIR=..
endif
#WORKDIR=/home/kozlova/release/

VERBOSEMAKE=""

#���������� ��� ����� ������
#CC_LOCAL := gcc
#������ ��� ����� ������
#LD_LOCAL := g++

# �������������� ����� ��� ����������� C/C++
#COPTS  = -I./ -I../VDRec/. -I../VDDCRec/ -I../VDDCRec/
COPTS  = `root-config --cflags` -I./ 

# �������������� ����� ��� ����������� Fortran
FOPTS  =
# -I./ -I../VDRec/. -I../VDDCRec/ -I/space/KEDR/current -I/space/KEDR/current -I/space/KEDR/current/AppFramework -I/space/KEDR/current/DchGeom -I/space/KEDR/current/KDB -I/space/KEDR/current/KdDCSim -I/space/KEDR/current/KsCERNJetset -I/space/KEDR/current/KsCERNPythia -I/space/KEDR/current/ReadNat -I/space/KEDR/current/VDDCRec

# �������������� ����� ��� �������
#LDOPTS = -lstdc++ -lg2c $(shell )$(ROOTSYS)/bin/root-config --libs --glibs)  -lMinuit -lNew 
#-rpath $(shell /usr/local/root-5.14.00/bin/root-config --libdir)
LDOPTS = -lg2c -Xlinker -rpath -Xlinker `root-config --libdir` \
 `root-config --libs --glibs`  -lNew -lMinuit -llapack

# -rpath $(shell /usr/local/root-5.14.00/bin/root-config --libdir)

#���� ���������� ���������� ONLYBINARY, �� ���������� � ������ �����������
ONLYBINARY=""

#���� ���������� �������� CERNLIB, �� ����� ��������������� ���� ��������
CERNLIBRARY = ""
#jetset74 mathlib graflib geant321 grafX11 packlib lapack
#packlib  lapack
#jetset74 graflib geant321 grafX11 packlib  lapack
#mathlib 
# ��� ���������� ����������� �����
#BINDIR := ./


# �������������� ���� (����������� ����� ���)
LIB_LOCAL= -lpq -lcrypt -lbz2 -lstdc++



# ���������, ����� ��������� �� ����� ��������
BINARIES =  PIdExample PIdExample_beam ##Skim JpsExp
#  dPPsample  getcalib
#dM
PIdExample_MODULES = PIdExample
PIdExample_LIBS = VDDCRec  KDisplay  ReadNat VDDCRec  KDisplay  ReadNat  KDB KrdEdxPId KrDCCalibdEdx DchdEdxDataRoot VDDCRec  KsTrg  KdDCSim   KrToF KsToF KrMu KEmcRec  LKrTools   KrAtc KdConvert KrObjects AppFramework FitTools DchGeom
#DchdEdxDataRoot KrdEdxPId  KrDCCalibdEdx2 VDDCRec   KsTrg  KDisplay VDRec ReadNat  KDB DchdEdxDataRoot KdDCSim  KrAtc VDDCRec  KrToF KsToF KrMu KEmcRec  LKrTools KdConvert KrObjects  AppFramework FitTools DchGeom
#KrdEdxPId  KrDCCalibdEdx2 DchdEdxDataRoot VDDCRec  KDisplay  ReadNat  KDB VDDCRec  KsTrg  KdDCSim KrToF KsToF KrMu KEmcRec  VDDCRec  KrAtc LKrTools  KdConvert KrObjects AppFramework FitTools DchGeom
#DchdEdxDataRoot KrdEdxPId  KrDCCalibdEdx2 VDDCRec   KsTrg  KDisplay VDRec ReadNat  KDB DchdEdxDataRoot KdDCSim  KrAtc VDDCRec  KrToF KsToF KrMu KEmcRec  LKrTools KdConvert KrObjects  AppFramework FitTools DchGeom 

PIdExample_beam_MODULES = PIdExample_beam
PIdExample_beam_LIBS = VDDCRec  KDisplay  ReadNat VDDCRec  KDisplay  ReadNat  KDB KrdEdxPId KrDCCalibdEdx DchdEdxDataRoot VDDCRec  KsTrg  KdDCSim   KrToF KsToF KrMu KEmcRec  LKrTools   KrAtc KdConvert KrObjects AppFramework FitTools DchGeom
#

##Skim_MODULES = Skim
##Skim_LIBS =  DchdEdxDataRoot KrdEdxPId  KrDCCalibdEdx2 VDDCRec   KsTrg  KDisplay VDRec ReadNat  KDB DchdEdxDataRoot KdDCSim  KrAtc VDDCRec  KrToF KsToF KrMu KEmcRec  LKrTools KdConvert KrObjects  AppFramework FitTools DchGeom
#KrdEdxPId  KrDCCalibdEdx2 VDDCRec  KDisplay  ReadNat  KDB VDDCRec  KsTrg  KdDCSim KrToF KsToF KrMu KEmcRec  VDDCRec  LKrTools   KrAtc KdConvert KrObjects AppFramework FitTools DchGeom

##JpsExp_MODULES = JpsExp
##JpsExp_LIBS = DchdEdxDataRoot KrdEdxPId  KrDCCalibdEdx2 VDDCRec   KsTrg  KDisplay VDRec ReadNat  KDB DchdEdxDataRoot KdDCSim  KrAtc VDDCRec  KrToF KsToF KrMu KEmcRec  LKrTools KdConvert KrObjects  AppFramework FitTools DchGeom

##JpsExp_LD = g++


# ������, �� ����� ������� ����� ������ ��� �������
# (��� ������ �� ����� �������� � ����������)
# � ����� ���������� ���� ���������� ��� ������

# ��������� ������ ����� �� �������
include $(WORKDIR)/KcReleaseTools/rules.mk

