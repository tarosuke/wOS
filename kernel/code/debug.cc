/*************************************************************** DEBUG CONSOLE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <debug.h>
#include <version.h>

#if CF_DEBUG_LEVEL
static void _putc(char c){
	asm volatile(
		"mov	$0x03fd, %%dx;"
		"2: inb	%%dx, %%al;"
		"test	$0x20, %%al;"
		"je	2b;"
		"mov	%%cl, %%al;"
		"mov	$0x03f8, %%dx;"
		"outb	%%al, %%dx"
		:: "c"(c) : "ax", "dx");
}


void dputc(char c){
	if(c == '\n'){
		_putc('\r');
	}
	_putc(c);
}

void dputs(const char* s){
	for(; *s; s++){
		dputc(*s);
	}
}


static inline void Hex(u64 val, char head, int chars){
	static const char* n = "0123456789abcdef";
	char buff[20];
	char* p;

	// 生成
	p = buff;
	do{
		*p++ = n[val & 15];
		val >>= 4;
		chars--;
	}while(val);

	//
	while(0 < chars--){
		*p++ = head;
	}

	//出力
	for(p--; buff <= p; p--){
		dputc(*p);
	}
}

static void UDec(u64 val, char head, char sign, int chars){
	char buff[20];
	char* p;

	// 生成
	p = buff;
	do{
		*p++ = '0' + (val % 10);
		val /= 10;
		chars--;
	}while(val);

	//
	if(sign == '-'){
		chars--;
	}
	while(0 < chars--){
		*p++ = head;
	}

	//出力
	if(sign == '-'){
		dputc('-');
	}
	for(p--; buff <= p; p--){
		dputc(*p);
	}
}

static inline void Dec(i64 val, char head, int chars){
	char sign = ' ';
	if(val < 0){
		sign = '-';
		val = -val;
	}
	UDec((uint)val, head, sign, chars);
}


void dprintf(const char* f, ...){
	const munit* p;
	char h;
	int n;

	for(p = reinterpret_cast<munit*>((munit)&f), p++; *f; f++){
		if(*f == '%'){
			uint longLevel;
			f++;

			/***** 桁フォーマット取得 */
			h = ' ';
			n = 0;
			if(*f == '0'){
				h = '0';
				f++;
			}
			while('0' <= *f && *f <= '9'){
				n *= 10;
				n += *f++ - '0';
			}

			for(longLevel = 0; *f == 'l'; f++, longLevel++);

			/***** 出力 */
			switch(*f){
				case 0 : return;
				case 's' :
					dputs((const char*)*p++);
					longLevel = 0;
					break;
				case 'd' :
					if(1 < longLevel){
						Dec(*(i64*)p, h, n);
						p += 2;
					}else{
						Dec((int)*p++, h, n);
					}
					break;
				case 'u' :
					if(1 < longLevel){
						UDec(*(u64*)p, h, ' ', n);
						p += 2;
					}else{
						UDec((uint)*p++, h, ' ', n);
					}
					break;
				case 'x' :
					if(1 < longLevel){
						Hex(*(u64*)p, h, n);
						p += 2;
					}else{
						Hex((uint)*p++, h, n);
					}
					break;
				case 'c' :
					dputc((char)*p++);
					break;
				default :
					dputc(*f);
					break;
			}
		}else{
			dputc(*f);
		}
	}
}


void Panic(const char* message){
	dputs(message);
#if 3 <= CF_DEBUG_LEVEL
	u32* s = (u32*)(void*)&message;
	for(uint i = 0; i < 128; i++, s++){
		if(!(i & 7)){
			dprintf("\n%02x: ", i);
		}
		dprintf("%08x ", *s);
	}
#endif
	for(;;){
		asm volatile("hlt");
	};
}


void __AssertionFailed(const char* file, const unsigned line){
	dprintf("Assertion fAILED at %s:%u", file, line);
	Panic("");
}
#if 5 <= CF_DEBUG_LEVEL
void __AssertionPassed(const char* file, const unsigned line){
	dprintf("Assertion passed at %s:%u\n", file, line);
}
#endif

#endif