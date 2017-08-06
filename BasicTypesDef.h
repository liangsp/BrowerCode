//===----------------------------------------------------------------------===//
// @file:       BasicTypesDef.h
// @brief:      基本平台数据类型定义
// @author:     shiping.liang
// @date:       2012/11/25
// @version:    1.0.1
// @revise:     1.0.0 - 2012/11/25 - shiping.liang - 创建
// @revise:     1.0.1 - 2012/11/29 - shiping.liang - 修改
//===----------------------------------------------------------------------===//

#ifndef _BASIC_TYPES_DEF_H_
#define _BASIC_TYPES_DEF_H_ 

#if defined(AIX) ||defined(_AIX_) || defined(_AIX) || defined(_AIX_SOURCE)
    #undef AIX
    #define AIX    /**< define AIX platform */
#elif defined(HPUX) || defined(HP_UX) ||defined(HP_UNIX) || defined(_HP_UX) || defined (_HP_SOURCE) || defined(HP)
    #undef HPUX
    #define HPUX   /**< define HP-UNIX platform */
#elif defined(LINUX) || defined(_LINUX_) || defined(_LINUX) || defined(_LINUX_SOURCE)
    #undef LINUX
    #define LINUX  /**< define LINUX platform */
#elif defined(SUNOS) || defined(_SUN_OS) || defined(_SUNOS_SOURCE)
    #undef SUNOS
    #define SUNOS  /**< define SUNOS platform */
#elif defined(WIN32) || defined(_WIN32) || defined(_MSC_VER)
    #undef WIN32
    #define WIN32  /**< define WINDOWS platform */
#else
    #error Cannot find a define type for your platform
#endif

#if defined(AIX) //! define sizes for AIX
    #define HAVE_STDINT_H       1
    #if defined(AIX_BITS64) || defined(BIT64)
        #define SIZEOF_CHAR              1
        #define SIZEOF_SHORT             4
        #define SIZEOF_INT               8
        #define SIZEOF_LONG              8
        #define SIZEOF_LONG_LONG         8
        #define SIZEOF_VOID_P            8
        #define SIZEOF_PTRDIFF_T         8
        #define SIZEOF_SIZE_T            8
    #elif defined(AIX_BITS32) || defined(BIT32)
        #define SIZEOF_CHAR              1
        #define SIZEOF_SHORT             2
        #define SIZEOF_INT               4
        #define SIZEOF_LONG              4
        #define SIZEOF_LONG_LONG         8
        #define SIZEOF_VOID_P            4
        #define SIZEOF_PTRDIFF_T         4
        #define SIZEOF_SIZE_T            4
    #endif

#elif defined(HPUX) //! define sizes for HP-UX
    #if defined(HP_BITS64) || defined(BIT64)
        #define SIZEOF_CHAR              1
        #define SIZEOF_SHORT             4
        #define SIZEOF_INT               8
        #define SIZEOF_LONG              8
        #define SIZEOF_LONG_LONG         8
        #define SIZEOF_VOID_P            8
        #define SIZEOF_PTRDIFF_T         8
        #define SIZEOF_SIZE_T            8
    #elif defined(HP_BITS32) || defined(BIT32)
        #define SIZEOF_CHAR              1
        #define SIZEOF_SHORT             2
        #define SIZEOF_INT               4
        #define SIZEOF_LONG              4
        #define SIZEOF_LONG_LONG         8
        #define SIZEOF_VOID_P            4
        #define SIZEOF_PTRDIFF_T         4
        #define SIZEOF_SIZE_T            4
    #endif
#elif defined(LINUX) //! define sizes for LINUX
    #define SIZEOF_VOID_P            4
    #define SIZEOF_PTRDIFF_T         4
    #define SIZEOF_SIZE_T            4
    #define HAVE_INTTYPES_H          1
#elif defined(WIN32) //! define sizes for WIN32
    #define SIZEOF_VOID_P            4
    #define SIZEOF_PTRDIFF_T         4
    #define SIZEOF_SIZE_T            4
#elif defined(SUNOS) //! define sizes for SUNOS
    //!< TODO define something
#else
    #error Cannot find a define type for your platform
#endif

#if HAVE_STDINT_H
    #include <stdint.h>
    #ifndef _INTTYPES_DEFINED
        #define _INTTYPES_DEFINED 1
    #endif
#endif

#if HAVE_INTTYPES_H
    #define __STDC_FORMAT_MACROS
    #include <inttypes.h>
    #ifndef _INTTYPES_DEFINED
        #define _INTTYPES_DEFINED 1
    #endif
#endif

#ifndef _INTTYPES_DEFINED
    #define _INTTYPES_DEFINED 1
    #if defined(WIN32) && defined(_MSC_VER)
        typedef signed char             int8_t;
        typedef unsigned char           uint8_t;
        typedef __int16                 int16_t;
        typedef unsigned __int16        uint16_t;
        typedef int                     int32_t;
        typedef unsigned int            uint32_t;
        typedef __int64                 int64_t;
        typedef unsigned __int64        uint64_t;
        typedef          int            intptr_t;
        typedef unsigned int            uintptr_t;
    #else
        /** 自动识别编译器的类型长度 */
        #if SIZEOF_CHAR == 1
            typedef signed   char  int8_t;
            typedef unsigned char uint8_t;
        #else
            #error Cannot find an 8-bit type for your platform
        #endif

        #if SIZEOF_CHAR == 2
            typedef signed   char  int16_t;
            typedef unsigned char uint16_t;
        #elif SIZEOF_SHORT == 2
            typedef          short  int16_t;
            typedef unsigned short uint16_t;
        #elif SIZEOF_INT == 2
            typedef          int  int16_t;
            typedef unsigned int uint16_t;
        #else
            //#error Cannot find a 16-bit type for your platform
        #endif

        #if SIZEOF_SHORT == 4
            typedef          short  int32_t;
            typedef unsigned short uint32_t;
        #elif SIZEOF_INT == 4
            typedef          int  int32_t;
            typedef unsigned int uint32_t;
        #elif SIZEOF_LONG == 4
            typedef          long  int32_t;
            typedef unsigned long uint32_t;
        #else
            //#error Cannot find a 32-bit type for your platform
        #endif

        #if SIZEOF_INT == 8
            typedef          int  int64_t;
            typedef unsigned int uint64_t;
        #elif SIZEOF_LONG == 8
            typedef          long  int64_t;
            typedef unsigned long uint64_t;
        #elif SIZEOF_LONG_LONG == 8
            typedef          long long  int64_t;
            typedef unsigned long long uint64_t;
        #else
            #error Cannot find a 64-bit type for your platform
        #endif

        #if SIZEOF_VOID_P == SIZEOF_SHORT
            typedef          short  intptr_t;
            typedef unsigned short uintptr_t;
        #elif SIZEOF_VOID_P == SIZEOF_INT
            typedef          int  intptr_t;
            typedef unsigned int uintptr_t;
        #elif SIZEOF_VOID_P == SIZEOF_LONG
            typedef          long  intptr_t;
            typedef unsigned long uintptr_t;
        #elif SIZEOF_VOID_P == SIZEOF_LONG_LONG
            typedef          long long  intptr_t;
            typedef unsigned long long uintptr_t;
        #else
            #error Cannot find a integral pointer-sized type for your platform
        #endif
    #endif

    #if defined(HPUX) && defined(__STDC_32_MODE__)
        /** HPUX inttypes.h 在有些情况下会遗漏下面的定义 */
        typedef          long long  int64_t;
        typedef unsigned long long uint64_t;
    #endif
#endif

#ifndef BOOL
    typedef int BOOL;
#endif

#ifndef TRUE
    #define TRUE  1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifdef WIN32
    typedef __int64 LONGLONG;
#else
    typedef long long LONGLONG;
#endif

typedef float float32_t;
typedef double float64_t;
typedef unsigned long ULong;
typedef long DWORD;
typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef unsigned int *PUINT;
typedef char *LPCSTR, *PCSTR, *LPCTSTR;
typedef char *LPSTR, *PSTR, *LPTSTR;
typedef void *LPVOID;

/* 一些平台可能会缺少下面的宏定义 */
#undef int_least8_t
#undef int_least16_t
#undef int_least32_t
#undef int_least64_t
#undef uint_least8_t
#undef uint_least16_t
#undef uint_least32_t
#undef uint_least64_t

#undef int_fast8_t
#undef int_fast16_t
#undef int_fast32_t
#undef int_fast64_t
#undef uint_fast8_t
#undef uint_fast16_t
#undef uint_fast32_t
#undef uint_fast64_t

#define int_least8_t  	int8_t
#define int_least16_t 	int16_t
#define int_least32_t 	int32_t
#define int_least64_t 	int64_t
#define uint_least8_t  	uint8_t
#define uint_least16_t 	uint16_t
#define uint_least32_t  uint32_t
#define uint_least64_t  uint64_t

#define int_fast8_t     int8_t
#define int_fast16_t    int16_t
#define int_fast32_t    int32_t
#define int_fast64_t    int64_t
#define uint_fast8_t    uint8_t
#define uint_fast16_t   uint16_t
#define uint_fast32_t   uint32_t
#define uint_fast64_t   uint64_t

/* 为使用安全起见，重新定义下面的宏 */
#undef intmax_t
#undef uintmax_t

#define intmax_t	    int64_t
#define uintmax_t	    uint64_t

/* 类型最大最小值定义宏 */
#undef INT8_MIN
#undef INT8_MAX
#undef UINT8_MAX
#undef INT16_MIN
#undef INT16_MAX
#undef UINT16_MAX
#undef INT32_MIN
#undef INT32_MAX
#undef UINT32_MAX
#undef INT64_MIN
#undef INT64_MAX
#undef UINT64_MAX

#undef INT_LEAST8_MIN
#undef INT_LEAST8_MAX
#undef UINT_LEAST8_MAX
#undef INT_LEAST16_MIN
#undef INT_LEAST16_MAX
#undef UINT_LEAST16_MAX
#undef INT_LEAST32_MIN
#undef INT_LEAST32_MAX
#undef UINT_LEAST32_MAX
#undef INT_LEAST64_MIN
#undef INT_LEAST64_MAX
#undef UINT_LEAST64_MAX

#undef INT_FAST8_MIN
#undef INT_FAST8_MAX
#undef UINT_FAST8_MAX
#undef INT_FAST16_MIN
#undef INT_FAST16_MAX
#undef UINT_FAST16_MAX
#undef INT_FAST32_MIN
#undef INT_FAST32_MAX
#undef UINT_FAST32_MAX
#undef INT_FAST64_MIN
#undef INT_FAST64_MAX
#undef UINT_FAST64_MAX

#define INT8_MIN	(-128)
#define INT8_MAX	( 127)
#define UINT8_MAX	( 255)
#define INT16_MIN	(-32768)
#define INT16_MAX	( 32767)
#define UINT16_MAX	( 65535)
#define INT32_MIN	(-2147483648)
#define INT32_MAX	( 2147483647)
#define UINT32_MAX	( 4294967295)

//解决在Linux 32位平台下gcc version 3.4.3 20041212 (Red Hat 3.4.3-9.EL4)会报超过最大的long型范围 begin
#define INT64_MIN	(-9223372036854775808LL)
#define INT64_MAX	( 9223372036854775807LL)
//end

#define UINT64_MAX	( 18446744073709551615)

#define INT_LEAST8_MIN		INT8_MIN
#define INT_LEAST8_MAX		INT8_MAX
#define UINT_LEAST8_MAX		UINT8_MAX
#define INT_LEAST16_MIN		INT16_MIN
#define INT_LEAST16_MAX		INT16_MAX
#define UINT_LEAST16_MAX	UINT16_MAX
#define INT_LEAST32_MIN		INT32_MIN
#define INT_LEAST32_MAX		INT32_MAX
#define UINT_LEAST32_MAX	UINT32_MAX
#define INT_LEAST64_MIN		INT64_MIN
#define INT_LEAST64_MAX		INT64_MAX
#define UINT_LEAST64_MAX	UINT64_MAX

#define INT_FAST8_MIN       INT8_MIN
#define INT_FAST8_MAX       INT8_MAX
#define UINT_FAST8_MAX      UINT8_MAX
#define INT_FAST16_MIN      INT16_MIN
#define INT_FAST16_MAX      INT16_MAX
#define UINT_FAST16_MAX     UINT16_MAX
#define INT_FAST32_MIN      INT32_MIN
#define INT_FAST32_MAX      INT32_MAX
#define UINT_FAST32_MAX     UINT32_MAX
#define INT_FAST64_MIN      INT64_MIN
#define INT_FAST64_MAX      INT64_MAX
#define UINT_FAST64_MAX     UINT64_MAX

#undef INTPTR_MIN
#undef INTPTR_MAX
#undef UINTPTR_MAX

#if   SIZEOF_VOID_P == 2
    #define INTPTR_MIN  	INT16_MIN
    #define INTPTR_MAX  	INT16_MAX
    #define UINTPTR_MAX  	UINT16_MAX
#elif SIZEOF_VOID_P == 4
    #define INTPTR_MIN  	INT32_MIN
    #define INTPTR_MAX  	INT32_MAX
    #define UINTPTR_MAX  	UINT32_MAX
#elif SIZEOF_VOID_P == 8
    #define INTPTR_MIN 	INT64_MIN
    #define INTPTR_MAX  	INT64_MAX
    #define UINTPTR_MAX  	UINT64_MAX
#else
    #error you have a bizarre void * size
#endif

#undef PTRDIFF_MIN
#undef PTRDIFF_MAX

#if   SIZEOF_PTRDIFF_T == 2
    #define PTRDIFF_MIN      INT16_MIN
    #define PTRDIFF_MAX      INT16_MAX
#elif SIZEOF_PTRDIFF_T == 4
    #define PTRDIFF_MIN      INT32_MIN
    #define PTRDIFF_MAX      INT32_MAX
#elif SIZEOF_PTRDIFF_T == 8
    #define PTRDIFF_MIN      INT64_MIN
    #define PTRDIFF_MAX      INT64_MAX
#else
    #error you have a bizarre ptrdiff_t size
#endif

#undef SIZE_MAX

#if   SIZEOF_SIZE_T == 2
    #define SIZE_MAX      UINT16_MAX
#elif SIZEOF_SIZE_T == 4
    #define SIZE_MAX      UINT32_MAX
#elif SIZEOF_SIZE_T == 8
    #define SIZE_MAX      UINT64_MAX
#else
    #error you have a bizarre size_t size
#endif

/* 下面的类型转换宏可以展开为简单的类型后缀，但比较难以自动检测 */
#undef INT8_C
#undef UINT8_C
#undef INT16_C
#undef UINT16_C
#undef INT32_C
#undef UINT32_C
#undef INT64_C
#undef UINT64_C

#undef INTMAX_C
#undef UINTMAX_C

#define INT8_C(value)    ((int_least8_t)value##LL)
#define UINT8_C(value)   ((uint_least8_t)value##ULL)
#define INT16_C(value)   ((int_least16_t)value##LL)
#define UINT16_C(value)  ((uint_least16_t)value##ULL)
#define INT32_C(value)   ((int_least32_t)value##LL)
#define UINT32_C(value)  ((uint_least32_t)value##ULL)
#define INT64_C(value)   ((int_least64_t)value##LL)
#define UINT64_C(value)  ((uint_least64_t)value##ULL)

#define INTMAX_C(value)  ((int_max_t)value##LL)
#define UINTMAX_C(value) ((uint_max_t)value##ULL)

#endif//_BASIC_TYPES_DEF_H_
