################################################### MAKEFILE FOR WHOLE TARGETs

.PHONY: help all clean configure tools emu confall todo cleantools erasetools test

TARGET ?= $(ARCH)
targets = $(dir $(shell ls objs/*/makefile))
m := $(shell grep -c Hz /proc/cpuinfo)

help:
	@cat docs/make.txt

emu:
	@for t in $(targets); do make -j $(m) -C $$t emu || exit -1; done

box:
	@for t in $(targets); do make -j $(m) -C $$t box || exit -1; done

all:
	@for t in $(targets); do make -j $(m) -C $$t wOS.fd.gz || exit -1; done

clean:
	@for t in $(targets); do make -j $(m) -C $$t clean || exit -1; done

tools:
	@for t in $(targets); do make -C $$t gnutools || exit -1; done

cleantools:
	@rm -r objs/gcc* objs/binutils*

erasetools:
	@rm -rf objs/tools

configure:
	@if [ -z $(ARCH) ]; then echo 'set ARCH. do "make help"'; exit -1; fi
	@if ! [ -d arch/$(ARCH) ]; then echo 'set right ARCH. do "ls arch"'; exit -1; fi
	@echo "arch:"$(ARCH)" target:"$(TARGET)
	@if ! [ -d objs/$(TARGET) ]; then mkdir objs/$(TARGET); fi
	@cp arch/$(ARCH)/config objs/$(TARGET)/
	@ln -s ../../arch/$(ARCH)/makefile objs/$(TARGET)
	@ln -s ../../arch/$(ARCH)/target.lds objs/$(TARGET)

confall:
	@echo -n 'prepareing'
	@for t in $(shell ls arch); do if ! [ -d objs/$$t ]; then mkdir objs/$$t; cp arch/$$t/config objs/$$t/; ln -s ../../arch/$$t/target.lds objs/$$t; ln -s ../../arch/$$t/makefile objs/$$t; echo -n '...'$$t; fi; done
	@echo '...done.'

todo:
	@grep -nrI --exclude-dir objs --exclude-dir .git --exclude-dir docs --exclude makefile 'TODO:' .


test:
	@if ! [ -d objs/test ]; then mkdir objs/test; fi
	@gcc -I test/include -o objs/test/test $(wildcard test/*.cc)
	@objs/test/test | tee objs/test/test.log
