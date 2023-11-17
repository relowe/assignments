#include "types.h"
#include "memlayout.h"
#include "elf.h"
#include "riscv.h"
#include "mem.h"
#include "string.h"
#include "console.h"
#include "trap.h"
#include "proc.h"

// Extern Globals
extern pagetable_t kernel_pagetable; // mem.c
extern char trampoline[]; // trampoline.S
extern char _binary_user_init_start; // The user init code

////////////////////////////////////////////////////////////////////////////////
// Static Definitions and Helper Function Prototypes
////////////////////////////////////////////////////////////////////////////////
static int nextpid = 1;
static pagetable_t proc_pagetable(struct proc*);
static void proc_free_pagetable(pagetable_t pagetable, uint64 sz);
static void proc_freewalk(pagetable_t pagetable);
static uint64 proc_shrink(pagetable_t pagetable, uint64 oldsz, uint64 newsz);
static int proc_loadseg(pagetable_t pagetable, uint64 va, void *bin, uint offset, uint sz);


////////////////////////////////////////////////////////////////////////////////
// Global Definitions
////////////////////////////////////////////////////////////////////////////////
struct cpu cpu;
struct proc proc[NPROC];




////////////////////////////////////////////////////////////////////////////////
// Process API Functions 
////////////////////////////////////////////////////////////////////////////////

// Initialize the proc table, and allocate a page for each process's 
// kernel stack. Map the stacks in high memory, followed by an invalid guard page.
void 
proc_init()
{
    // You need to loop over all the proc structs and set up their stacks
    // This setup requires two steps:
    //   1.) Use the KSTACK macro to set up the kstack field in the struct
    //   2.) Allocate a new physical page for the stack and insert it
    //       into the kernel's page table at the virtual address referred
    //       to by kstack.
    // HINTS: This function is a combination of two functions in xv6. 
    //        I used the following memory functions:
    //           vm_page_alloc
    //           vm_page_insert
    // YOUR CODE HERE
}



// Set up the first user process. Return the process it was allocated to.
struct proc*
proc_load_user_init()
{
    void *bin = &_binary_user_init_start;
    struct proc *p=0;

    // Allocate a new process. If there is no process avaialble, panic.
    // Use proc_load_elf to load up the elf string. 
    // As an additional hint, I have defined the variables you need 
    // for you. The bin pointer points to the embedded BLOB which
    // contains the program image for init.
    // YOUR CODE HERE

    return p;
}


// Look in the process table for an UNUSED proc.
// If found, initialize state required to run in the kernel,
// and return with p->lock held.
// If there are no free procs, or a memory allocation fails, return 0.
struct proc* 
proc_alloc()
{
    // Search for an unused process in the proc array. If you do 
    // not find one, return 0. If you do find one, do the following:
    //   1.) Set the pid field to the next available pid. 
    //       (be sure to update nextpid)
    //   2.) Allocate a trapframe page for the proces.
    //   3.) Set the trapframe page to all zeroes.
    //   4.) Allocate an empty page table for the process.
    //   5.) Set the return address in the processe's context to 
    //       return to usertrapret.
    //   6.) Set the process stack pointer to one address past the end of the
    //       kstack page.
    // HINTS: This function combines several ideas from xv6 function, but it 
    //        does require adaptation. 
    //        I used the following functions:
    //          vm_pagealloc
    //          proc_free
    //          memset
    //          proc_pagetable
    // YOUR CODE HERE

    return 0;
}


// free a proc structure and the data hanging from it,
// including user pages.
void 
proc_free(struct proc *p)
{
    // Free the process's trapframe, empty its pagetable,
    // and reset all fields to zero. The state of the process
    // should be "UNUSED".
    // HINT: Functions I used
    //         vm_page_free
    //         proc_free_pagetable
    // YOUR CODE HERE
}


// Load the ELF program image stored in the binary string bin
// into the specified process. This operation will destroy the 
// pagetable currently in p, and replace it with a page table
// as indicated by the segments of the elf formatted binary.
int
proc_load_elf(struct proc *p, void *bin)
{
    struct elfhdr elf;
    struct proghdr ph;
    int i, off;
    uint64 sz=0, sp=0;
    pagetable_t pagetable=0;

    // get the elf header from bin
    elf = *(struct elfhdr*) bin;

    // check the elf magic
    if(elf.magic != ELF_MAGIC)
        goto bad;

    // We need to load the process from the binary string pointed to
    // by bin. This is similar to xv6's exec function, but with 
    // several key differences:
    //   - We are loading from bin, not from the disk.
    //   - Offsets give the number of bytes from the beginning of bin.
    //   - We will not be putting program arguments on the stack.
    // The basic steps we need to perform are as follows:
    //   1.) Create a new pagetable for the process
    //   2.) Loop over all of the program headers in the elf object
    //       - Check the validity of the header, goto bad if invalid
    //       - Use proc_resize to increase the size of the process to 
    //         hold this segment's data
    //       - Use proc_loadseg to load this segment into user memory
    //   3.) Set up the user stack
    //   4.) Destroy the old page table
    //   5.) Commit to the user image
    //   6.) Mark the process as runnable
    //   7.) Return 0
    //
    // If we have bad elf image, do the following:
    //   1.) Free the page table if it exists
    //   2.) return -1
    // HINT: The key to this function is to fully understand how the 
    //       elf object looks in the binary object. I have given you
    //       my variables as well as how you obtain the elf header
    //       as a hint. You will also need to fully understand how
    //       exec works in xv6. Happy reading!
    // YOUR CODE HERE
    return 0;

bad:
    // YOUR CODE HERE
    return -1;
}


// Resize the process so that it occupies newsz bytes of memory.
// If newsz > oldsz
//   Allocate PTEs and physical memory to grow process from oldsz to
// If newsz < oldsz
//   Use proc_shrink to decrease the zie of the process.
// newsz, which need not be page aligned.  Returns new size or 0 on error.
uint64 proc_resize(pagetable_t pagetable, uint64 oldsz, uint64 newsz) 
{
    // Make this behave as above. This is a little bit different from the
    // xv6 equivalent. What did I change? 
    //
    // YOUR CODE HERE
    return newsz;
}


////////////////////////////////////////////////////////////////////////////////
// Static Helper Functions
////////////////////////////////////////////////////////////////////////////////

// Create a user page table for a given process,
// with no user memory, but with trampoline pages.
static pagetable_t 
proc_pagetable(struct proc *p)
{
    // Create a new pagetable for the process. Do not assign it yet, just
    // return the pagetable after you create it.
    // The page table should contain the following entries:
    //   - Map the trampoline physical address to the TRAMPOLINE virtual address.
    //     trampoline should be readable and executable.
    //   - Map the p->trapframe physical address to the TRAPFRAME virtual address.
    //     The trapframe page should be readable and writable.
    // The functions I used here were:
    //    vm_create_pagetable
    //    vm_page_insert
    //    vm_page_free
    //    vm_page_remove
    // YOUR CODE HERE
    return 0;
}



// Free a process's page table, and free the
// physical memory it refers to.
static void 
proc_free_pagetable(pagetable_t pagetable, uint64 sz)
{
    // 1.) Remove the TRAMPOLINE and TRAPFRAME pages
    // 2.) Remove all the user memory pages, freeing their 
    //    physical memory.
    // 3.) Free the user page table.
    // Functions Used: vm_page_remove, proc_freewalk
    // YOUR CODE HERE
}



// Recursively free page-table pages.
// All leaf mappings must already have been removed.
static void 
proc_freewalk(pagetable_t pagetable)
{
  // there are 2^9 = 512 PTEs in a page table.
  for(int i = 0; i < 512; i++){
    pte_t pte = pagetable[i];
    if((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0){
      // this PTE points to a lower-level page table.
      uint64 child = PTE2PA(pte);
      proc_freewalk((pagetable_t)child);
      pagetable[i] = 0;
    } else if(pte & PTE_V){
      panic("freewalk: leaf");
    }
  }
  vm_page_free((void*)pagetable);
}


// Deallocate user pages to bring the process size from oldsz to
// newsz.  oldsz and newsz need not be page-aligned, nor does newsz
// need to be less than oldsz.  oldsz can be larger than the actual
// process size.  Returns the new process size.
static uint64 
proc_shrink(pagetable_t pagetable, uint64 oldsz, uint64 newsz)
{
  if(newsz >= oldsz)
    return oldsz;

  if(PGROUNDUP(newsz) < PGROUNDUP(oldsz)){
    int npages = (PGROUNDUP(oldsz) - PGROUNDUP(newsz)) / PGSIZE;
    vm_page_remove(pagetable, PGROUNDUP(newsz), npages, 1);
  }

  return newsz;
}


// Load a program segment into pagetable at virtual address va.
// va must be page-aligned
// and the pages from va to va+sz must already be mapped.
// Returns 0 on success, -1 on failure.
static int
proc_loadseg(pagetable_t pagetable, uint64 va, void *bin, uint offset, uint sz)
{
  uint i, n;
  uint64 pa;

  // Load the program segment from the bin array. Note that 
  // offset could be thought of as the offset into bin. There 
  // is an equivalent xv6 function which does this using inode 
  // loading. The secret to converting it is you are going to 
  // have a line that uses memmove and the following expression:
  //   bin+offset+i
  // As an added hint, I have included my variable declarations 
  // above.
  // YOUR CODE HERE
  return 0;
}


