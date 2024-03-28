/* ************************************************************************
> File Name:     malloc.c
> Author:        niu0217
> Created Time:  Mon 19 Feb 2024 11:46:50 AM CST
> Description:
 ************************************************************************/

#include "minicrt.h"

typedef struct _heap_header {
    enum {
        HEAP_BLOCK_FREE = 0xABABABAB, //magic number of free block
        HEAP_BLOCK_USED = 0xCDCDCDCD, //magic number of used block
    }type;

    unsigned size; //block size including header
    struct _heap_header* next;
    struct _heap_header* prev;
}heap_header;

#define ADDR_ADD(a,o) (((char*)(a)) + o)
#define HEADER_SIZE (sizeof(heap_header))

static heap_header* list_head = NULL;

void free(void* ptr) {
    //释放的当前块是空闲的，直接返回
    heap_header* header = (heap_header*)ADDR_ADD(ptr, -HEADER_SIZE);
    if(header->type != HEAP_BLOCK_USED) {
        return;
    }

    //将当前块设置为空闲
    header->type = HEAP_BLOCK_FREE;

    //当前块的前一块是空闲的，和当前块合并
    if(header->prev != NULL && header->prev->type == HEAP_BLOCK_FREE) {
        //merge
        header->prev->next = header->next;
        if(header->next != NULL) {
            header->next->prev = header->prev;
        }
        header->prev->size += header->size;

        header = header->prev;
    }

    //当前块的后一块是空闲的，和当前块合并
    if(header->next != NULL && header->next->type == HEAP_BLOCK_FREE) {
        //merge
        header->size += header->next->size;
        header->next = header->next->next;
    }
}

void* malloc(unsigned size) {
    heap_header* header;

    if(size == 0) {
        return NULL;
    }

    header = list_head;
    while(header != 0) {
        if(header->type == HEAP_BLOCK_USED) {
            header = header->next;
            continue;
        }
        if(header->size > size + HEADER_SIZE &&
                header->size <= size + HEADER_SIZE * 2) {
            header->type = HEAP_BLOCK_USED;
        }
        if(header->size > size + HEADER_SIZE * 2) {
            //split
            heap_header* next = (heap_header*)ADDR_ADD(header, size + HEADER_SIZE);
            next->prev = header;
            next->next = header->next;
            next->type = HEAP_BLOCK_FREE;
            next->size = header->size - (size - HEADER_SIZE);
            header->next = next;
            header->size = size + HEADER_SIZE;
            header->type = HEAP_BLOCK_USED;
            return ADDR_ADD(header, HEADER_SIZE);
        }
        header = header->next;
    }

    return NULL;
}

#ifndef WIN32
//Linux brk system call
static int brk(void* end_data_segment) {
    int ret = 0;
    //brk system call number: 45
    //in /usr/include/asm-generic/unistd.h
    // #deine __NR_brk 45
    asm("movl $45,%%eax  \n\t"
        "movl %1,%%ebx   \n\t"
        "int $0x80       \n\t"
        "movl %%eax,%0   \n\t"
        : "=r"(ret): "m"(end_data_segment));
}
#endif

#ifdef WIN32
#include<Windows.h>
#endif

int mini_crt_heap_init() {
    void* base = NULL;
    heap_header* header = NULL;
    //32MB heap size
    unsigned heap_size = 1024 * 1024 * 32;

#ifdef WIN32
    base = VirtualAlloc(0, heap_size, MEM_COMMIT | MEM_PESERVE, PAGE_READWRITE);
    if(base == NULL) {
        return 0;
    }

#else
    base = (void*)brk(0);
    void* end = ADDR_ADD(base, heap_size);
    end = (void*)brk(end);
    if(!end) {
        return 0;
    }

#endif

    header = (heap_header*)base;

    header->size = heap_size;
    header->type = HEAP_BLOCK_FREE;
    header->prev = NULL;
    header->next = NULL;

    list_head = header;
    return 1;
}
