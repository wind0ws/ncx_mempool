ncx_mempool
======================
>This repo fork from  [reed-lau](https://github.com/reed-lau/ncx_mempool), thanks to him for make it more better!

Description
===========

a nginx-based memory pool . <br/>
both share-memory and private-memory are supported. it is more efficient than malloc.<br/>
it is a very light weight and do not rely on any third-party library.

Examples
========

```c
    #include "ncx_slab.h"

    int main(int argc, char **argv)
    {
        char *p;
        size_t  pool_size = 4096000;  //4M
        ncx_slab_stat_t stat;
        u_char  *space;
        space = (u_char *)malloc(pool_size);

         ncx_slab_pool_t *sp;
         sp = (ncx_slab_pool_t*) space;

         sp->addr = space;
         sp->min_shift = 3;
         sp->end = space + pool_size;

         /*
          * init
          */
         ncx_slab_init(sp);

         /*
          * alloc
          */
         p = ncx_slab_alloc(sp, 128);

         /*
          *  memory usage api.
		  *  note: this is optional. call it if you want to see memory usage status.
          */
         ncx_slab_stat(sp, &stat);

         /*
          * free
          */
         ncx_slab_free(sp, p);

         free(space);

         return 0;
    }
 ```

API
===
```c
void ncx_slab_init(ncx_slab_pool_t *pool)
```
>**Description**: 初始化内存池结构；

<br/>
```c
void *ncx_slab_alloc(ncx_slab_pool_t *pool, size_t size)
```
>**Description**: 内存分配

<br/>
```c
void ncx_slab_free(ncx_slab_pool_t *pool, void *ptr)
```
>**Description**: 释放内存

<br/>
```c
void ncx_slab_stat(ncx_slab_pool_t *pool, ncx_slab_stat_t *stat)
```
>**Description**: 查看内存池使用情况,（通过log接口打印）

Customization
=============
正如example所示，内存池内存是由应用层先分配，ncx_mempool是在给定的内存基础上进行分配和回收管理。 <br/>
所以内存池本身不关心所管理的内存是私有内存 or 共享内存;

ncx_lock.h 是锁接口；根据实际需要重定义: <br/>
 1. 多线程共享内存池，可参考pthread_spin_lock <br/>
 2. 多进程共享内存池，可参考nginx的ngx_shmtx.c实现spin lock <br/>
 3. 单进程单线程使用内存池，无锁编程..

ncx_log.h 是日志接口，根据实际需要重定义.

ncx_slab.c.orz 是 ncx_slab.c的详细注释，方便理解.
>注意： 在使用的时候请务必添加-O2或或-Os或-O3编译参数进行代码优化，否则效率可能还不如malloc/free.
            在调试阶段，可以使用-O0编译参数，方便跟踪调试。


Copyright and License
=====================
This module is licensed under the BSD license.
Copyright (C) 2013, by dcshi(施俊伟). <dcshi@qq.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and 
      the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
