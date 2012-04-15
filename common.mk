############################################################## COMMON MAKEFILE

.SUFFIXES :
.SUFFIXES : .a .o .i .s .S .c .cc .h

all: wOS
keyfiles = ../../makefile.in makefile config
ldkeyfiles = $(keyfiles) ../../arch/$(arch)/common.lds


############################################################### BUILDING TOOLS
NLD := $(LD)
NAS := $(AS)
NCC := $(CC)

CC = gcc
CPP = $(prefix)cpp
AS = $(prefix)as
AR = $(prefix)ar
LD = $(prefix)ld
OC = $(prefix)objcopy
LIBPATH = $(toolPath)/lib/gcc/$(gnuArch)-wOS-elf/$(gccVer)/



###################################################################### OPTIONS
base_ccopts += -pipe -O2 -fomit-frame-pointer -Wall -Werror -g -nostdinc\
	-nostdlib -fno-common -iquote. -fkeep-inline-functions\
	-finline-functions-called-once $(addprefix -I,$(incs))\
	-I ../../objs/tools/lib/gcc/$(gnuArch)-wOS-elf/$(gccVer)/include

copts = $(base_ccopts) -ffreestanding
ccopts = $(base_ccopts) -Wa,-W -nostdinc++ -fno-enforce-eh-specs\
	-fno-threadsafe-statics -fno-check-new\
	-fno-rtti -fno-exceptions -fno-unwind-tables -ffunction-sections
aopts = $(incs)
ldopts = -N --cref -nostdlib -z nodefaultlib --gc-sections



############################################################### IMPLICIT RULES
ifneq ($(MAKECMDGOALS),gnutools)
-include $(addsuffix .d, $(mods))
%.o : %.cc $(keyfiles)
	@echo $@
	@$(CC) $(ccopts) -c -o $@ $<

%.o : %.c $(keyfiles)
	@echo $@
	@$(CC) $(copts) -c -o $@ $<

%.o : %.S $(keyfiles)
	@echo $@
	@$(CC) $(copts) -c -o $@ $<

%.d : %.cc $(keyfiles)
	@echo $@
	@$(CPP) $(ccopts) -M -MG $< > $@

%.d : %.c $(keyfiles)
	@echo $@
	@$(CPP) $(copts) -M -MG $< > $@

%.d : %.S $(keyfiles)
	@echo $@
	@$(CPP) $(copts) -M -MG $< > $@
endif
