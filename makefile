################################################### MAKEFILE FOR WHOLE TARGETs

.PHONY: help all clean configure tools emu confall

TARGET ?= $(ARCH)
targets = $(dir $(shell ls objs/*/makefile))

help:
	@cat docs/make.txt

emu:
	@for t in $(targets); do make -C $$t emu || exit -1; done

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

confall:
	@echo -n 'prepareing...'
	@for t in $(shell ls arch); do if ! [ -d objs/$$t ]; then mkdir objs/$$t; cp arch/$$t/makefile arch/$$t/talos.conf objs/$$t; echo -n $$t; fi; done
	@echo '...done.'
