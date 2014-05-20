#include <stdio.h>
#include <string.h>

/* Memory block */
typedef
struct
{
    int is_available;
    int size;
} MCB, *MCB_P;


char *mem_start_p; /* pointer to the start of the allocation linked list */
int max_mem; /* Memory allocation threshold */
int allocated_mem; /* this is the memory in use. */
int mcb_count; /* no. of memory blocks allocated */

char *heap_end; /* pointer to the end of allocation linked list */

/* header data-structure to denote state of allocated memory block */
enum {NEW_MCB=0,NO_MCB,REUSE_MCB};
enum {FREE,IN_USE};


/* initialises start/end pointers & allocation threshold */
void
InitMem(char *ptr, int size_in_bytes)
{
    max_mem = size_in_bytes;
    mem_start_p = ptr;
    mcb_count = 0;
    allocated_mem = 0;
    heap_end = mem_start_p + size_in_bytes;
}


/* custom allocator that maintains linked list */
void *
myalloc(int elem_size)
{
    /* check whether any chunk (allocated before) is free first */

    MCB_P p_mcb;
    int flag = NO_MCB;

    p_mcb = (MCB_P)mem_start_p;

    int sz;

    sz = sizeof(MCB);

    if( (elem_size + sz)  > (max_mem - (allocated_mem + mcb_count * sz ) ) )
    {
        printf("Max size Excedded!!!!!");
        return NULL;
    }
    while( heap_end > ( (char *)p_mcb + elem_size + sz)   )
    {
        if ( p_mcb->is_available == 0)
        {
            if( p_mcb->size == 0)
            {
                flag = NEW_MCB;
                break;
            }
            if( p_mcb->size > (elem_size + sz) )
            {
                printf("Running inside this loop\n");
                flag = REUSE_MCB;
                break;
            }
        }
        printf("Executing after the break\n");
        p_mcb = (MCB_P) ( (char *)p_mcb + p_mcb->size);
    }

    if( flag != NO_MCB)
    {
        p_mcb->is_available = IN_USE;

        if( flag == NEW_MCB)
        {
            p_mcb->size = elem_size + sizeof(MCB);
            mcb_count++;
        }
        allocated_mem += elem_size;
        return ( (char *) p_mcb + sz);
    }

    printf(" Returning as we could not allocate any MCB \n");
    return NULL;

    /* if size of the available chunk is equal to greater than required size, use that chunk */
}

int
MemEfficiency()
{
    /* keep track of number of MCBs in a global variable */
    return mcb_count;

}

void
myfree(void *p)
{
    MCB_P ptr = (MCB_P)p;
    ptr--;

    mcb_count--;
    ptr->is_available = FREE;
    printf("\nptr->size: %d ",ptr->size);
    allocated_mem -= (ptr->size - sizeof(MCB));
    printf("\nAllocated mem: %d ",allocated_mem);
    printf("\nMemory Freed...\n");
}

int main()
{
    char buf[1024];
    memset(buf,0,1024);

    InitMem(buf,1024);

    char *str, *str1, *str2;

    str=myalloc(200);
    printf("\nMemory address: %p",str);
    printf("\nMCB count: %-3d \tAllocated Memory: %-10d\n\n",mcb_count,allocated_mem);
    str1=myalloc(300);
    printf("\nMemory address: %p",str1);
    printf("\nMCB count: %-3d \tAllocated Memory: %-10d\n\n",mcb_count,allocated_mem);
    myfree(str);
    str2=myalloc(100);
    printf("\n\nMemory address: %p",str2);
    printf("\nMCB count: %-3d \tAllocated Memory: %-10d\n",mcb_count,allocated_mem);
}
