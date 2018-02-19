
#ifndef _CONFIG_H_
#define	_CONFIG_H_

#if defined(__cplusplus)
extern "C"{
#endif



//#define	WINDOWS32
//#define	PROTOTYPES
//#define	LITTLE_ENDIAN
//#define	LOWHI

/*
 * AUTO DEFINES (DON'T TOUCH!)
 */

#ifndef	CSTRTD
typedef char *CSTRTD;
#endif
#ifndef	BSTRTD
typedef unsigned char *BSTRTD;
#endif

#ifndef	BYTE
typedef unsigned char BYTE;
#endif
#ifndef	UINT
typedef unsigned int UINT;
#endif
#ifndef	USHORT
typedef unsigned short USHORT;
#endif
#ifndef	ULONG
typedef unsigned long ULONG;
#endif
#ifndef	DIGIT
typedef USHORT DIGIT;	/* 16-bit word */
#endif
#ifndef	DBLWORD
typedef ULONG DBLWORD;  /* 32-bit word */
#endif

#ifndef	WORD64
typedef ULONG WORD64[2];  /* 64-bit word */
#endif


#if defined(__cplusplus)
}
#endif

#endif /* _CONFIG_H_ */
