/************************************************************** PCI BUSMANAGER
 *	Copyright (C) 2007- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _PCIBUS_
#define _PCIBUS_

#include <types.h>
#include <config.h>


class PCIBUS{
public:
	PCIBUS();
private:
	static const u16 CONFIG_ADDR = 0x0cf8;
	static const u16 CONFIG_DATA = 0x0cfc;
	static const u16 CONFIG_ENABLE = 0x0cf8;
	static const u16 CONFIG_FORWARD = 0x0cfa;
	static const u16 CONFIG_BASE = 0xc000;
	u32 (*readConfig)(uint, uint, uint, uint);
	static u32 ReadConfig1(uint bus, uint dev, uint func, uint addr);
	static u32 ReadConfig2(uint bus, uint dev, uint func, uint addr);
	bool Register(uint bus, uint dev, uint func);
};


//////////////////////////////////////////////////////////////////// pci specs

struct PCI_CREGS{
	u32 dvid;

	u32 command : 16;
	u32 stat : 16;

	u32 rev : 8;
	u32 devClass : 24;

	u32 cachelineSize : 8;
	u32 latency : 8;
	u32 hType : 8;
	u32 bist : 8;

	u32 addrs[6];
	u32 res0;
	u32 res1;
	u32 exRomBase;
	u32 res2;
	u32 res3;

	u32 interruptLine : 8;
	u32 interruptPin : 8;
	u32 minGrant : 8;
	u32 maxLatency : 8;
}__attribute__((packed));

struct PCI_SPEC{
	u32 dvid;
};

static inline bool PCI_MatchSpec(
	const PCI_SPEC specs[],
	const PCI_CREGS& creg){
	for(uint i = 0; specs[i].dvid; i++){
		if(specs[i].dvid == creg.dvid){
			return true;
		}
	}
	return false;
}


#endif
