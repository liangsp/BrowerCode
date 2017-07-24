/*!
 * @file      traits_sample.h
 * @brief     Traits��̼��� Traits���Ի�ȡһ�����͵������Ϣ
 *
 * Copyright (c) 2015 chinasoft Tech.Co.,Ltd
 *
 * @author    lsp74391
 * @date      2017/7/20
 * @version   1.0.0
 *
*******************************************************************************
 */
#ifndef __I_ALLOC_H__
#define __I_ALLOC_H__

#include <stdio.h>
#include <stdlib.h>

template <class _Iterator>
struct iterator_traits
{
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::value_type        value_type;
    typedef typename _Iterator::difference_type   difference_type;
    typedef typename _Iterator::pointer           pointer;
    typedef typename _Iterator::reference         reference;
};

template <class _Tp>
struct iterator_traits<_Tp *>
{
    //typedef random_access_iterator_tag iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef _Tp                        *pointer;
    typedef _Tp                        &reference;
};

template <class _Tp>
struct iterator_traits<const _Tp *>
{
    //typedef random_access_iterator_tag iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const _Tp                  *pointer;
    typedef const _Tp                  &reference;
};


template <int __inst>
class __malloc_alloc_template
{

private:
    static void *_S_oom_malloc(size_t);
    static void *_S_oom_realloc(void *, size_t);
    static void (* __malloc_alloc_oom_handler)();

public:
    static void *allocate(size_t __n)
    {
        void *__result = malloc(__n);
        if(0 == __result)
        {
            __result = _S_oom_malloc(__n);
        }
        return __result;
    }

    static void deallocate(void *__p, size_t /* __n */)
    {
        free(__p);
    }

    static void *reallocate(void *__p, size_t /* old_sz */, size_t __new_sz)
    {
        void *__result = realloc(__p, __new_sz);
        if(0 == __result)
        {
            __result = _S_oom_realloc(__p, __new_sz);
        }
        return __result;
    }

    static void (* __set_malloc_handler(void (*__f)()))()
    {
        void (* __old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = __f;
        return(__old);
    }

};

template <int __inst>
void (* __malloc_alloc_template<__inst>::__malloc_alloc_oom_handler)() = 0;

template <int __inst>
void *__malloc_alloc_template<__inst>::_S_oom_malloc(size_t __n)
{
    void (* __my_malloc_handler)();
    void *__result;

    for(;;)
    {
        __my_malloc_handler = __malloc_alloc_oom_handler;
        if(0 == __my_malloc_handler)
        {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        (*__my_malloc_handler)();
        __result = malloc(__n);
        if(__result)
        {
            return(__result);
        }
    }
}

template <int __inst>
void *__malloc_alloc_template<__inst>::_S_oom_realloc(void *__p, size_t __n)
{
    void (* __my_malloc_handler)();
    void *__result;

    for(;;)
    {
        __my_malloc_handler = __malloc_alloc_oom_handler;
        if(0 == __my_malloc_handler)
        {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        (*__my_malloc_handler)();
        __result = realloc(__p, __n);
        if(__result)
        {
            return(__result);
        }
    }
}

typedef __malloc_alloc_template<0> malloc_alloc;


template <bool threads, int inst>
class __default_alloc_template
{

private:
    // Really we should use static const int x = N
    // instead of enum { x = N }, but few compilers accept the former.
    enum {_ALIGN = 8};
    enum {_MAX_BYTES = 128};
    enum {_NFREELISTS = 16}; // _MAX_BYTES/_ALIGN

    static size_t _S_round_up(size_t __bytes)
    {
        return (((__bytes) + (size_t) _ALIGN - 1) & ~((size_t) _ALIGN - 1));
    }

private:
    union _Obj
    {
        union _Obj *_M_free_list_link;
        char _M_client_data[1];    /* The client sees this.        */
    };

private:
    // Specifying a size results in duplicate def for 4.1
    static _Obj *volatile _S_free_list[_NFREELISTS];

    static size_t _S_freelist_index(size_t __bytes)
    {
        return (((__bytes) + (size_t)_ALIGN - 1) / (size_t)_ALIGN - 1);
    }

    // Returns an object of size __n, and optionally adds to size __n free list.
    static void *_S_refill(size_t __n);

    // Allocates a chunk for nobjs of size size.  nobjs may be reduced
    // if it is inconvenient to allocate the requested number.
    static char *_S_chunk_alloc(size_t __size, int &__nobjs);

    // Chunk allocation state.
    static char *_S_start_free;
    static char *_S_end_free;
    static size_t _S_heap_size;

public:

    /* __n must be > 0      */
    static void *allocate(size_t __n)
    {
        void *__ret = 0;

        if(__n > (size_t) _MAX_BYTES)
        {
            __ret = malloc_alloc::allocate(__n);
        }
        else
        {
            _Obj *volatile *__my_free_list = _S_free_list + _S_freelist_index(__n);
            // Acquire the lock here with a constructor call.
            // This ensures that it is released in exit or during stack
            // unwinding.
            _Obj *__result = *__my_free_list;
            if(__result == 0)
            {
                __ret = _S_refill(_S_round_up(__n));
            }
            else
            {
                *__my_free_list = __result->_M_free_list_link;
                __ret = __result;
            }
        }

        return __ret;
    };

    /* __p may not be 0 */
    static void deallocate(void *__p, size_t __n)
    {
        if(__n > (size_t) _MAX_BYTES)
        {
            malloc_alloc::deallocate(__p, __n);
        }
        else
        {
            _Obj *volatile  *__my_free_list
                = _S_free_list + _S_freelist_index(__n);
            _Obj *__q = (_Obj *)__p;

            __q->_M_free_list_link = *__my_free_list;
            *__my_free_list = __q;
        }
    }

    static void *reallocate(void *__p, size_t __old_sz, size_t __new_sz);

} ;


/* We allocate memory in large chunks in order to avoid fragmenting     */
/* the malloc heap too much.                                            */
/* We assume that size is properly aligned.                             */
/* We hold the allocation lock.                                         */
template <bool __threads, int __inst>
char *__default_alloc_template<__threads, __inst>::_S_chunk_alloc(size_t __size, int &__nobjs)
{
    char *__result;
    size_t __total_bytes = __size * __nobjs;
    size_t __bytes_left = _S_end_free - _S_start_free;

    if(__bytes_left >= __total_bytes)
    {
        __result = _S_start_free;
        _S_start_free += __total_bytes;
        return(__result);
    }
    else if(__bytes_left >= __size)
    {
        __nobjs = (int)(__bytes_left / __size);
        __total_bytes = __size * __nobjs;
        __result = _S_start_free;
        _S_start_free += __total_bytes;
        return(__result);
    }
    else
    {
        size_t __bytes_to_get = 2 * __total_bytes + _S_round_up(_S_heap_size >> 4);
        // Try to make use of the left-over piece.
        if(__bytes_left > 0)
        {
            _Obj *volatile *__my_free_list = _S_free_list + _S_freelist_index(__bytes_left);

            ((_Obj *)_S_start_free)->_M_free_list_link = *__my_free_list;
            *__my_free_list = (_Obj *)_S_start_free;
        }
        _S_start_free = (char *)malloc(__bytes_to_get);
        if(0 == _S_start_free)
        {
            size_t __i;
            _Obj *volatile *__my_free_list;
            _Obj *__p;
            // Try to make do with what we have.  That can't
            // hurt.  We do not try smaller requests, since that tends
            // to result in disaster on multi-process machines.
            for(__i = __size; __i <= (size_t) _MAX_BYTES; __i += (size_t) _ALIGN)
            {
                __my_free_list = _S_free_list + _S_freelist_index(__i);
                __p = *__my_free_list;
                if(0 != __p)
                {
                    *__my_free_list = __p->_M_free_list_link;
                    _S_start_free = (char *)__p;
                    _S_end_free = _S_start_free + __i;
                    return(_S_chunk_alloc(__size, __nobjs));
                    // Any leftover piece will eventually make it to the
                    // right free list.
                }
            }
            _S_end_free = 0;    // In case of exception.
            _S_start_free = (char *)malloc_alloc::allocate(__bytes_to_get);
            // This should either throw an
            // exception or remedy the situation.  Thus we assume it
            // succeeded.
        }
        _S_heap_size += __bytes_to_get;
        _S_end_free = _S_start_free + __bytes_to_get;
        return(_S_chunk_alloc(__size, __nobjs));
    }
}


/* Returns an object of size __n, and optionally adds to size __n free list.*/
/* We assume that __n is properly aligned.                                */
/* We hold the allocation lock.                                         */
template <bool __threads, int __inst>
void *__default_alloc_template<__threads, __inst>::_S_refill(size_t __n)
{
    int __nobjs = 20;
    char *__chunk = _S_chunk_alloc(__n, __nobjs);
    _Obj *volatile *__my_free_list;
    _Obj *__result;
    _Obj *__current_obj;
    _Obj *__next_obj;
    int __i;

    if(1 == __nobjs)
    {
        return(__chunk);
    }
    __my_free_list = _S_free_list + _S_freelist_index(__n);

    /* Build free list in chunk */
    __result = (_Obj *)__chunk;
    *__my_free_list = __next_obj = (_Obj *)(__chunk + __n);
    for(__i = 1; ; __i++)
    {
        __current_obj = __next_obj;
        __next_obj = (_Obj *)((char *)__next_obj + __n);
        if(__nobjs - 1 == __i)
        {
            __current_obj ->_M_free_list_link = 0;
            break;
        }
        else
        {
            __current_obj ->_M_free_list_link = __next_obj;
        }
    }
    return(__result);
}

template <bool threads, int inst>
void *__default_alloc_template<threads, inst>::reallocate(void *__p, size_t __old_sz, size_t __new_sz)
{
    void *__result;
    size_t __copy_sz;

    if(__old_sz > (size_t) _MAX_BYTES && __new_sz > (size_t) _MAX_BYTES)
    {
        return(realloc(__p, __new_sz));
    }
    if(_S_round_up(__old_sz) == _S_round_up(__new_sz))
    {
        return(__p);
    }
    __result = allocate(__new_sz);
    __copy_sz = __new_sz > __old_sz ? __old_sz : __new_sz;
    memcpy(__result, __p, __copy_sz);
    deallocate(__p, __old_sz);
    return(__result);
}

template <bool __threads, int __inst>
char *__default_alloc_template<__threads, __inst>::_S_start_free = 0;

template <bool __threads, int __inst>
char *__default_alloc_template<__threads, __inst>::_S_end_free = 0;

template <bool __threads, int __inst>
size_t __default_alloc_template<__threads, __inst>::_S_heap_size = 0;

template <bool __threads, int __inst>
typename __default_alloc_template<__threads, __inst>::_Obj *volatile
__default_alloc_template<__threads, __inst>::_S_free_list[
     __default_alloc_template<__threads, __inst>::_NFREELISTS
 ] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


typedef __default_alloc_template<false, 0> alloc;


#endif//__I_ALLOC_H__