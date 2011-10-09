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
		uchar	signeture[4];
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
		u16	modeAttribtes;
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
};



#if 0
static u32 Mask(int size, int pos){
	return (~0UL << pos) & ~(~0UL << (pos + size));
}
static const COLORMODEL vesaColor(
	(uint)vesaInfo.bytesPerScanLine / vesaInfo.xResolution,
	(int)vesaInfo.redFieldPosition + vesaInfo.redMaskSize - 8,
	(int)vesaInfo.greenFieldPosition + vesaInfo.greenMaskSize - 8,
	(int)vesaInfo.blueFieldPosition + vesaInfo.blueMaskSize - 8,
	(DISPLAYCOLOR)Mask(vesaInfo.redMaskSize,
		vesaInfo.redFieldPosition),
	(DISPLAYCOLOR)Mask(vesaInfo.greenMaskSize,
		vesaInfo.greenFieldPosition),
	(DISPLAYCOLOR)Mask(vesaInfo.blueMaskSize,
		vesaInfo.blueFieldPosition));



static DEVICE* Init(const DEVICE::PARAM&){
	const POINT size(vesaInfo.xResolution, vesaInfo.yResolution);
	const COLORMODEL model(vesaColor);
	void* const body(HEAP::Map(vesaInfo.physBasePtr,
		vesaInfo.bytesPerScanLine * vesaInfo.yResolution));
	cprintf("VESA display\x1b+\n"
		"VRAM:%08x(%08x)\n"
		"xres:%d\n"
		"yres:%d\n"
		"opp:%d\n"
		"bit/mask\x1b+\n"
		"r:%d/%08x\n"
		"g:%d/%08x\n"
		"b:%d/%08x\x1b-\n"
		"\x1b-OK.\n",
		body, vesaInfo.physBasePtr,
		vesaInfo.xResolution,
		vesaInfo.yResolution,
		model.opp,
		model.redBit, model.redMask,
		model.greenBit, model.greenMask,
		model.blueBit, model.blueMask);

	COLORMODELEDIMAGE* display(0);
	switch(model.opp){
	case 1 :
		display = new DISPLAY<1>(size, model, body);
		break;
	case 2 :
		display = new DISPLAY<2>(size, model, body);
		break;
	case 3 :
		display = new DISPLAY<3>(size, model, body);
		break;
	case 4 :
		display = new DISPLAY<4>(size, model, body);
		break;
	default :
		cprintf("illigal opp:%d.\n", model.opp);
		break;
	}

	if(display){
		DISPLAYEVENT::New(EVENT::evNewDisplay, *display);
	}

	return 0;
}

static const DEVICE::ID ids[] = { { LOCALBUS::vesaDisplay }, { 0 } };
DRIVER_DEF{
	DEVICE::local,
	ids,
	DEVICE::mlDevice,
	0,
	Init
};

#endif


