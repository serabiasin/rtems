include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am

include $(top_srcdir)/ada.am

AM_ADAFLAGS += -I$(srcdir)/..

noinst_PROGRAMS = mp05_ada_mp05_node1
mp05_ada_mp05_node1_SOURCES = mp05_node1.adb ../mptest.adb config.h
mp05_ada_mp05_node1_SOURCES += ../mptest.ads
mp05_ada_mp05_node1_SOURCES += ../../../support/init.c

mp05_ada_mp05_node1$(EXEEXT): mp05_node1.adb ../mptest.adb init.$(OBJEXT)
	$(GNATCOMPILE) -margs -a $< -o $@

scndir = $(rtems_ada_testsdir)
dist_scn_DATA = ada_mp05-node1.scn

