
#define CF_AMD64	1

/* PAE有効に強制 */
#ifndef CF_PAE
#define CF_PAE 1
#else
#if !CF_PAE
#undef CF_PAE
#define CF_PAE 1
#endif
#endif