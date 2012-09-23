/*************************************************************** DEBUG CONSOLE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <config.h>
#include <types.h>
#include <debug.h>
#include <stdarg.h>
#include <clock.h>


#if CF_DEBUG_LEVEL
extern void __ARCH_putc(char);
static uint indentLevel(0);
static void _putc(char c){
	if(indentLevel < CF_DEBUG_LEVEL){
		__ARCH_putc(c);
	}
}

void dputc(char c){
	static char p;

	switch(c){
	case '\n' :
		_putc('\r');
		_putc(c);
		break;
	case 1 :
		indentLevel++;
		break;
	case 2 :
		if(0 < indentLevel){ indentLevel--; }
		if(p == '\n'){
			for(uint i(0); i < indentLevel; _putc(' '), i++);
		}
		break;
	default :
		if(p == 1 && c != 2){
			_putc('\r');
			_putc('\n');
		}
		if(p == '\n' || p == 1){
			for(uint i(0); i < indentLevel; _putc(' '), i++);
		}
		_putc(c);
	}
	p = c;
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


static inline void Mem(u64 size){
	static const u64 KiB(1024ULL);
	static const u64 MiB(KiB*KiB);
	static const u64 GiB(MiB*KiB);
	static const u64 TiB(GiB*KiB);
	static const u64 PiB(TiB*KiB);
	static const struct SCALE{
		u64 scale;
		const char* name;
	}scale[] = {
		{ PiB, "[PiB]" },
		{ TiB, "[TiB]" },
		{ GiB, "[GiB]" },
		{ MiB, "[MiB]" },
		{ KiB, "[KiB]" },
		{ 0, "[Byte(s)]" }, };
	const SCALE* sc(scale);
	for(; size < (*sc).scale; sc++);
	if((*sc).scale){
		UDec(size / (*sc).scale, ' ', ' ', 0);
		dputc('.');
		UDec((size & ((*sc).scale - 1)) * 100 / (*sc).scale, '0', ' ', 2);
	}else{
		UDec(size, ' ', ' ', 0);
	}
	dputs((*sc).name);
}

static inline void Str(const char* str){
	dputs(str ? str : "(null)");
}

void dprintf(const char* f, ...){
	va_list p;
	va_start(p, f);
	for(; *f; f++){
		if(*f == '%'){
			uint longLevel;
			char h;
			int n;
			if(!++f){
				break;
			}

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
				Str(va_arg(p, char*));
				longLevel = 0;
				break;
			case 'd' :
				if(1 < longLevel){
					Dec(va_arg(p, i64), h, n);
				}else{
					Dec(va_arg(p, int), h, n);
				}
				break;
			case 'u' :
				if(1 < longLevel){
					UDec(va_arg(p, u64), h, ' ', n);
				}else{
					UDec(va_arg(p, uint), h, ' ', n);
				}
				break;
			case 'x' :
				if(1 < longLevel){
					Hex(va_arg(p, u64), h, n);
				}else{
					Hex(va_arg(p, uint), h, n);
				}
				break;
			case 'c' :
				dputc(va_arg(p, int));
				break;
			case 'p' : //仮想アドレス
				Hex(va_arg(p, munit), '0', sizeof(munit) * 2);
				break;
			case 'r' : //実アドレス
				Hex(va_arg(p, runit), '0', sizeof(runit) * 2);
				break;
			case 'm' : //自動副単位メモリサイズ(u64で指定すること。//TODO:可変長整数にする)
				Mem(va_arg(p, u64));
				break;
			case 't' : //tunitを時間で
				{
				const tunit now(va_arg(p, tunit));
				const CLOCK::DATE d(now);
				dprintf("%llu/%02u/%02u %02u:%02u:%02u.%06u",
					d.year, d.month, d.date,
					d.hour, d.min, d.sec, d.usec);
				}break;
			default :
				dputc(*f);
				break;
			}
		}else{
			dputc(*f);
		}
	}
	va_end(p);
}


void Dump(const void* start){
	const u32* s((const u32*)start);
	for(uint i = 0; i < 128; i++, s++){
		if(!(i & 3)){
			dputc('\n');
			Hex((munit)start + i, '0', sizeof(munit) * 2);
			dputc(':');
		}
		dputc(' ');
		Hex(*s, '0', 8);
	}
}



void Panic(const char* message){
	dputs(message);
#if CF_SERIAL_DEBUG
	Dump((const void*)&message);
#endif
	for(;;){
		asm volatile("hlt");
	};
}


void __AssertionFailed(const char* file, const unsigned line){
	dprintf(LIGHTRED "Assertion FAILED at %s:%u" ORIGIN, file, line);
	Panic("");
}
#if 5 <= CF_DEBUG_LEVEL
void __AssertionPassed(const char* file, const unsigned line){
	dprintf("Assertion passed at %s:%u\n", file, line);
}
#endif

#endif
