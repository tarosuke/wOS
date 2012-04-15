/********************************************************* VESA DISPLAY DRIVER
	Copyright (C) 2004- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id$
*/

#include <types.h>
#include <config.h>
#include <debug.h>

extern "C"{
	extern struct __attribute__ ((packed)){
		uchar	signature[4];
		u16	version;
		u32	oemString;
		u32	capabilities[4];
		u32	videoModes;
		u16	totalMemory;
		u16	oemRev;
		u32	venderName;
		u32	productName;
		u32	productRevision;
		uchar	reserved[222];
		uchar	oemData[256];
	}__VESA_InfoBlock;
	extern struct __attribute__ ((packed)){
		u16	modeAttributes;
		uchar	winAAttributes;
		uchar	winBAttributes;
		u16	winGranularity;
		u16	winSize;
		u16	winASegment;
		u16	winBSegment;
		u32	winFuncPt;
		u16	bytesPerScanLine;
		u16	xResolution;
		u16	yResolution;
		uchar	xCharSize;
		uchar	yCharSize;
		uchar	numberOfPlanes;
		uchar	bitPerPixel;
		uchar	numberOfBanks;
		uchar	memoryModel;
		uchar	bankSize;
		uchar	numberOfImagePages;
		uchar	res0;
		uchar	redMaskSize;
		uchar	redFieldPosition;
		uchar	greenMaskSize;
		uchar	greenFieldPosition;
		uchar	blueMaskSize;
		uchar	blueFieldPosition;
		uchar	rsvdMaskSize;
		uchar	rsvdFieldPosition;
		uchar	directColorModeInfo;
		u32	physBasePtr;
		u32	res1;
		u16	res2;
		u16	linBytePerScanLine;
		uchar	bnkNumberOfImagePages;
		uchar	linNumberOfImagePages;
		uchar	linRedMaskSize;
		uchar	linRedFieldPosition;
		uchar	linGreenMaskSize;
		uchar	linGreenFieldPosition;
		uchar	linBlueMaskSize;
		uchar	linBlueFieldPosition;
		uchar	linRsvdMaskSize;
		uchar	linRsvdFieldPosition;
		u32	maxPixelClock;
		uchar	res3[189];
	}__VESA_displayInfo;
	extern char __kernel_base[];
};

struct PMIB{
	char signature[4];
	u16 entryPoint;
	u16 initEntryPoint;
	u16 BIOSDataSel;
	u16 A0Sel;
	u16 B0Sel;
	u16 B8Sel;
	u16 codeSegSel;
	char inProtectMode;
	char checksum;
}__attribute__((packed));



static void* Get16(munit p){
	return (void*)(((p & 0xffff0000) >> 12) + (p & 0xffff));
}

void VESA_Check(){
#if 3 <= CF_DEBUG_LEVEL
	dputs("enumlating supported display modes...\n");
	for(u16* m((u16*)Get16(__VESA_InfoBlock.videoModes)); *m != 0xffff; m++){
		if(0x10d <= (*m & 0x1ff)){
			dprintf(" %04x", *m);
		}
	}

	dprintf("\n VESA signature:%04s\n", __VESA_InfoBlock.signature);
	dprintf("        version:%04x\n", __VESA_InfoBlock.version);
	dprintf("     OEM string:%s\n", Get16(__VESA_InfoBlock.oemString));
	dprintf("     venderName:%s\n", Get16(__VESA_InfoBlock.venderName));
	dprintf("    productName:%s\n", Get16(__VESA_InfoBlock.productName));
	dprintf("productRevision:%s\n", Get16(__VESA_InfoBlock.productRevision));
	dprintf("     videoModes:%08x.\n", __VESA_InfoBlock.videoModes);
	dprintf("          width:%u.\n", __VESA_displayInfo.xResolution);
	dprintf("         height:%u.\n", __VESA_displayInfo.yResolution);
	dprintf("            bpp:%u.\n", __VESA_displayInfo.bitPerPixel);
	dprintf("       BaseAddr:%08lx.\n", __VESA_displayInfo.physBasePtr);

	dputs("scanning protect-mode VESA interface..."INDENT);
	const PMIB* pm(0);
	for(u32* p((u32*)&__kernel_base[0xc0000]); p <
(u32*)&__kernel_base[0xd0000]; p++){
		if(*p == 0x44494d50){
			pm = (PMIB*)p;
			break;
		}
	}
	if(pm){
		const char* const s((*pm).signature);
		dprintf("     signeture:%c%c%c%c\n", s[0], s[1], s[2], s[3]);
		dprintf("    entryPoint:%x\n", (*pm).entryPoint);
		dprintf("initEntryPoint:%x\n", (*pm).initEntryPoint);
		dprintf("   BIOSDataSel:%x\n", (*pm).BIOSDataSel);
		dprintf("         A0Sel:%x\n", (*pm).A0Sel);
		dprintf("         B0Sel:%x\n", (*pm).B0Sel);
		dprintf("         B8Sel:%x\n", (*pm).B8Sel);
		dprintf("    codeSegSel:%x\n", (*pm).codeSegSel);
		dprintf(" inProtectMode:%x\n", (*pm).inProtectMode);
	}else{
		dputs(UNINDENT"not found.\n");
	}
#endif
}

