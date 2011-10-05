################################################### MAKEFILE FOR WHOLE TARGETs

.PHONY: help all clean configure tools

TARGET ?= $(ARCH)
targets = $(dir $(shell ls objs/*/makefile))

help:
	@cat docs/make.txt

all:
	@for t in $(targets); do make -C $$t || exit -1; done

clean:
	@for t in $(targets); do make -C $$t clean || exit -1; done

tools:
	@for t in $(targets); do make -C $$t gnutools || exit -1; done

configure:
	@if [ -z $(ARCH) ]; then echo 'set ARCH. do "make help"'; exit -1; fi
	@if ! [ -d arch/$(ARCH) ]; then echo 'set right ARCH. do "ls arch"'; exit -1; fi
	@echo "arch:"$(ARCH)" target:"$(TARGET)
	@if ! [ -d objs/$(TARGET) ]; then mkdir objs/$(TARGET); fi
	@cp arch/$(ARCH)/makefile arch/$(ARCH)/talos.conf objs/$(TARGET)

