/************************************************************** PCI BUSMANAGER
	Copyright (C) 2007- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/

#include <arch/pcibus.h>
#include <cpu/io.h>
#include <debug.h>


static PCIBUS pcibus __attribute__((init_priority(5000)));

u32 PCIBUS::ReadConfig1(uint bus, uint dev, uint func, uint addr){
	out32(CONFIG_ADDR, 0x80000000 | (bus << 16) |
	     (dev << 11) | (func << 8) | (addr & 0xfc));
	return in32(CONFIG_DATA);
}

u32 PCIBUS::ReadConfig2(uint bus, uint dev, uint func, uint addr){
	if(16 <= dev){
		return 0;
	}
	out32(CONFIG_ENABLE, 0xf0 | func << 1);
	out32(CONFIG_FORWARD, bus);
	return in32(CONFIG_BASE + (dev << 8) + addr);
}

bool PCIBUS::Register(uint bus, uint dev, uint func){
	const uint regSize = sizeof(PCI_CREGS) / sizeof(u32);
	PCI_CREGS reg;

	///// read common parametor register
	for(uint i = 0; i < regSize; i++){
		((u32*)&reg)[i] = readConfig(bus, dev, func, i << 2);
	}

	eprintf("bus:%d dev:%d func:%d devID:%08x class:%06x \n",
		bus,
		dev,
		func,
		reg.dvid,
		reg.devClass);

	///// find driver & register the device
#if 0
	RESOURCES res(reg);
	if(DEVICE::NewDevice((DEVICE::PARAM){
		DEVICE::pci,
		{ reg.dvid, reg.rev, reg.devClass },
		0,
		&res,
		0})){
#if !CF_SHOW_ALL_PCI_DEVS
		cprintf("bus:%d dev:%d func:%d devID:%08x class:%06x \n",
			bus,
			dev,
			func,
			reg.dvid,
			reg.devClass);
#endif
	}

	return !(reg.hType & 0x80);
#endif
	return false;
}

PCIBUS::PCIBUS(){
	u32 val;
	dputs("PCI-bus: scanning..." INDENT);

	///// check type
	val = ReadConfig1(0, 0, 0, 0);
	if(val && (val & 0xffff) != 0xffff){
		readConfig = &ReadConfig1;
	}else{
		readConfig = &ReadConfig2;
	}

	///// scan buses
	for(uint bus = 0; bus < 256; bus++){
		val = readConfig(bus, 0, 0, 0);
		for(uint dev = 0; dev < 32; dev++){
			val = readConfig(bus, dev, 0, 0);
			if(!val || (val & 0xffff) == 0xffff){
				continue;
			}
			if(Register(bus, dev, 0)){
				continue;
			}
			for(uint func = 1; func < 8; func++){
				val = readConfig(bus, dev, func, 0);
				if(val && (val & 0xffff) != 0xffff){
					Register(bus, dev, func);
				}
			}
		}
	}

	///// into normal mode
	out32(CONFIG_ADDR, 0);
	out32(CONFIG_ENABLE, 0);

	dputs(UNINDENT "OK.\n");
}


