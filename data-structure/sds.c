//
// Created by erpang on 6/6/23.
//
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include "../util/log.h"
#include "sds.h"
#include "../memory/sdsalloc.h"

const char *SDS_NOINIT = "SDS_NOINIT";

// 根据字符串长度，获取SDS结构类型
static inline char sdsReqType(size_t string_size) {
    if (string_size < 1<<5)
        return SDS_TYPE_5;
    if (string_size < 1<<8)
        return SDS_TYPE_8;
    if (string_size < 1<<16)
        return SDS_TYPE_16;
#if (LONG_MAX == LLONG_MAX)
    if (string_size < 1ll<<32)
        return SDS_TYPE_32;
    return SDS_TYPE_64;
#else
    return SDS_TYPE_32;
#endif
}

// 获取SDS最大可支持内存
static inline size_t sdsTypeMaxSize(char type) {
    if (type == SDS_TYPE_5)
        return (1<<5) - 1;
    if (type == SDS_TYPE_8)
        return (1<<8) - 1;
    if (type == SDS_TYPE_16)
        return (1<<16) - 1;
#if (LONG_MAX == LLONG_MAX)
    if (type == SDS_TYPE_32)
        return (1ll<<32) - 1;
#endif
    return -1; /* this is equivalent to the max SDS_TYPE_64 or SDS_TYPE_32 */
}


// 获取SDS类型对应的结构体占用大小
static inline int sdsHdrSize(char type) {
    switch(type&SDS_TYPE_MASK) {
        case SDS_TYPE_5:
            return sizeof(struct sdshdr5);
        case SDS_TYPE_8:
            return sizeof(struct sdshdr8);
        case SDS_TYPE_16:
            return sizeof(struct sdshdr16);
        case SDS_TYPE_32:
            return sizeof(struct sdshdr32);
        case SDS_TYPE_64:
            return sizeof(struct sdshdr64);
    }
    return 0;
}

sds _sdsnewlen(const void *init, size_t initlen, int trymalloc) {
    // 根据字符串长度判断SDS类型
    char type = sdsReqType(initlen);
    logger_debug("initlen:%d, type:%d\n", initlen, type);
    // 这里应该是为了性能考虑，空字符串通常用作拼接，直接用SDS_TYPE_8更可靠
    if (type == SDS_TYPE_5 && initlen == 0) type = SDS_TYPE_8;
    // 计算SDS头部大小
    int hdrlen = sdsHdrSize(type);

    // 这里是为了防止字符串没有溢出，但如果加上SDS头和终止符可能溢出的情况
    assert(initlen + hdrlen + 1 > initlen); /* Catch size_t overflow */

    logger_debug("hdrlen:%d, type:%d\n", hdrlen, type);
    void *sh;
    size_t usable;
    // 分配SDS内存大小为hdrlen + initlen + 1,即SDS头+char数组+终止符\0,分配的内存大小为usable的值
    sh = s_malloc_usable(hdrlen + initlen + 1, &usable);
    logger_debug("point sh[%p]\n", sh);

    if (sh == NULL) return NULL;
    logger_debug("malloc size:%d\n", usable);

    // TODO 此段代码意义未明
    if (init == SDS_NOINIT)
        init == NULL;
    else if (!init) {
        memset(sh, 0, hdrlen + initlen + 1);
    }

    // 定义SDS
    sds s;

    // SDS 指针指向char[] 第一个元素
    s = ((char*)sh) + hdrlen;
    logger_debug("point s[%p] char:%c\n", s, *s);

    // SDS头 flags指针，用来赋值SDS类型
    unsigned char *fp;
    fp = ((unsigned char*)s) - 1;
    logger_debug("point fp[%p] fp:%d\n", fp, *fp);


    // 记录已用内存
    usable = usable - hdrlen - 1;

    // 如果超出类型支持最大值，则置为最大值，这也就是为什么Redis String 最大支持512MB的原因
    if (usable > sdsTypeMaxSize(type))
        usable = sdsTypeMaxSize(type);

    // 根据不同SDS类型处理SDS结构
    switch(type) {
        case SDS_TYPE_5: {
            // 高5位存储字符串长度，低3为存储SDS类型，为了节约空间
            // 在用sds类型获取到flags后，flags&SDS_TYPE_BIT即可获取到SDS类型，右移3位后为字符串长度
            *fp = type | (initlen << SDS_TYPE_BITS);
            logger_debug("point fp[%p] fp:%d\n", fp, *fp);
            break;
        }
        case SDS_TYPE_8: {
            SDS_HDR_VAR(8,s);
            logger_debug("point sh[%p]，s[%p]\n", sh, s);
            sh->len = initlen;
            sh->alloc = usable;
            *fp = type;
            break;
        }
        case SDS_TYPE_16: {
            SDS_HDR_VAR(16,s);
            sh->len = initlen;
            sh->alloc = usable;
            *fp = type;
            break;
        }
        case SDS_TYPE_32: {
            SDS_HDR_VAR(32,s);
            sh->len = initlen;
            sh->alloc = usable;
            *fp = type;
            break;
        }
        case SDS_TYPE_64: {
            SDS_HDR_VAR(64,s);
            sh->len = initlen;
            sh->alloc = usable;
            *fp = type;
            break;
        }
    }
    // 拷贝字符数组到sds
    if (initlen && init)
        memcpy(s, init, initlen);
    // 添加终止符，兼容C字符串
    s[initlen] = '\0';
    return s;
}

sds sdsnew(const char *init) {
    size_t initlen = (init == NULL) ? 0 : strlen(init);
    return sdsnewlen(init, initlen);
}

sds sdsnewlen(const void *init, size_t initlen) {
    return _sdsnewlen(init, initlen, 0);
}






