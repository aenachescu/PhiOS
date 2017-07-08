# PhiOS Memory Management Docs

# 1. Placement Address Allocator
In the initial states of the kernel, there is not any advanced memory manager, so for some simple and permanent allocations, the placement address allocator (PAA) should be used.

The internal logic is very primitive, the PAA receives a start address and keeps increasing it when an allocation is requested. The memory areas allocated using it could not be freed as there is not any information about the size of the allocated blocks. 

In early stages, any big allocation should be done by using the PAA and not using the stack. The usage of kernel stack is for the process switching and for other internal procedures and it has a limit of 16 KB, so using it for large data structures could blow up the kernel.

After enabling the physical memory manager, the PAA should be NOT used anymore.

# 2. Physical Memory Manager
The physical memory manager (PMM) should be a generic component of the kernel, it should allow easily changing the internal algorithm of memory allocation. This becomes possible by providing two generic function types for alloc/free memory and a data structure for storing information about current algorithm.

Current PMM uses a bitmap based algorithm. The whole memory is divided in frames of X size. For each frame, there is a bit in the bitmap that let us know if the frame is free or not. (1 frame is used, 0 frame is not used)

# 3. Paging
## 1. Paging module structure
* Each paging type should be enabled/used via a generic set of functions and structures. The module should be capable of the following operations:
    * Initialization procedure
    * Alloc/free page(s)
    * Check if a page/a set of pages is free
    * Lock/unlock system for thread safety
    * Enable paging on the CPU level
    * Switch mapping
* The module should implement a generic paging structure that contains:
    * Information about free/used pages
    * Information about paging type
    * Pointer to actual architecture dependent paging structure
    
## 2. Paging types
* IA-32
    This paging type supports 4 GB of virtual memory for each process and it comes in two variants: using pages that maps 4 KB and page tables with 1024 pages that maps 4 MB or using directly pages that maps 4 MB. Each page table entry is 32-bit large using 20 bits for actual address of mapped memory and 12 bits for flags. The details about structures are in [ia32.h](https://github.com/EnachescuAlin/PhiOS/blob/devel/kernel/include/arch/x86/paging/ia32.h). To activate paging, the address of the directory page table must be loaded into CR3 register and PG and PE bits of CR0 must be set. (CR0 OR 0x80000001) To enable 4 MB pages, a special bit must be set in CR4. (CR4 OR 0x00000010)
For initialization, a page directory for kernel must be created and first 4 MB must be "identity mapped" (1:1). Then, the kernel should be mapped to 3 GB address. After enabling paging, the "identity mapping" could be deleted and all pointers should be translated to the higher-half.
* PAE
* IA-32 Extended (64-bit)

## 3. Memory mapping model
Each process will have its own page directory (maximum size of virtual memory is determined by the type of paging) and the kernel must be mapped within it. Kernel should be mapped at 3 GB and should not exceed 1 GB of physical memory. This model should allow to all 32 bit processes a full address space of 3 GB of virtual memory and also portability for 64 bit processes.

# 4. Virtual Memory Manager
The virtual memory manager (VMM) should use the paging module to interact with the MMU in a high-level manner. The VMM should not interact directly with the pages, the page tables or with any other low-level structures. The only objective of VMM is to map physical memory to virtual memory and to keep information about it.

# 5. Memory Map
PhiOS's kernel is a higher-half one, so it will be placed above 3 GB. In this section should be written all memory areas that would be used as default for any kind of kernel/user structure.

* 0x0 (memory start) - 0xc0000000 (3 GB): user space
* 0xc0000000 (3 GB) - 0x100000000 (4 GB): kernel space
* 0x100000000 (4 GB) and above: user space for 64-bit processes