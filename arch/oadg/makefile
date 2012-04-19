############################################################## MAKEFILE FOR OADG

arch = oadg
cpu = x86
gnuArch = i686
base_ccopts = -m32

include ../../makefile.in
include ../../arch/oadg/loader/makefile.in


##### EXTRA RULES

.phony : emu
qemuOptFiles = .qemu-netopt .qemu-nicopt
qemuOpts = -vga std -serial stdio -smp 8 -m 4096 $(shell cat $(qemuOptFiles))


emu : emufd

emufd : wOS.fd hddImage $(qemuOptFiles)
	qemu $(qemuOpts) -fda wOS.fd -hda hddImage -boot a
emucd : wOS.iso hddImage $(qemuOptFiles)
	qemu $(qemuOpts) -cdrom wOS.iso -hda hddImage -boot d

box : wOS.fd
	bochs -q 'boot:a' 'floppya: 1_44=wOS.fd, status=inserted'

##### qemu network options
.phony : qemu-tapnet qemu-usernet

.qemu-netopt :
	echo -n '-net user ' > $@

qemu-usernet :
	echo -n '-net user ' > .qemu-netopt

qemu-tapnet :
	# be sure qemu is sudoed and /etc/qemu-ifup is ready.
	# for detail, read docs/qemuMemo.txt
	echo -n '-net tap ' > .qemu-netopt


##### qemu NIC options
.phony : qemuNic-i82551 qemuNic-i82557b qemuNic-i82559er qemuNic-smc91c111
.phony : qemuNic-ne2kpci qemuNic-ne2kisa qemuNic-pcnet qemuNic-rtl8139

.qemu-nicopt :
	echo -n '-net nic,model=ne2k_pci ' > .qemu-nicopt

qemuNic-i82551 :
	echo -n '-net nic,model=i82551 ' > .qemu-nicopt

qemuNic-i82557b :
	echo -n '-net nic,model=i82557b ' > .qemu-nicopt

qemuNic-i82559er :
	echo -n '-net nic,model=i82559er ' > .qemu-nicopt

qemuNic-smc91c111 :
	echo -n '-net nic,model=smc91c111 ' > .qemu-nicopt

qemuNic-ne2kpci :
	echo -n '-net nic,model=ne2k_pci ' > .qemu-nicopt

qemuNic-ne2kisa :
	echo -n '-net nic,model=ne2k_isa ' > .qemu-nicopt

qemuNic-pcnet :
	echo -n '-net nic,model=pcnet ' > .qemu-nicopt

qemuNic-rtl8139 :
	echo -n '-net nic,model=rtl8139 ' > .qemu-nicopt


wOS.fd.gz : wOS.fd
	gzip -c wOS.fd > wOS.fd.gz
wOS.fd : wOS fdloader
	cat fdloader wOS /dev/zero | dd of=wOS.fd bs=1024 count=1440
wOS.iso : wOS.fd
	rm -rf tmp
	mkdir tmp
	cp wOS.fd tmp
	mkisofs -o wOS.iso -b wOS.fd tmp
	rm -rf tmp







emuhd : pb=$(strip $(shell for p in 0x1be 0x1ce 0x1de 0x1ee; do if [ `od -An -tu1 -N1 -j $$(($$p + 4)) hddImage`"" = 32 ]; then echo $$p; break; fi; done))
emuhd : pp=$(strip $(shell od -An -tu4 -N4 -j $$(($(pb) + 8)) hddImage))
emuhd : wOS hdloader hddImage
	cat hdloader wOS | dd of=hddImage bs=512 seek=$(pp) conv=notrunc
	echo -ne `od -An -txC -N4 -j $$(($(pb) + 8)) hddImage | sed -e ":r s/ /\\\\\\x/" -e "t r"` | dd of=hddImage seek=`echo "$$(($(pp) * 512 + 0x1f6))"` bs=1 conv=notrunc

hddImage : mbr
	cp $< $@
	dd if=/dev/zero of=$@ bs=1024 count=1 seek=102399 conv=notrunc



