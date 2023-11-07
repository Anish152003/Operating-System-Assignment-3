#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define PAGE_SIZE 4096

typedef struct MemorySegment {
    int sta_addr;
    size_t size;
    void*addr;
    int type; // 0 for HOLE, 1 for PROCESS
    struct MemorySegment* next;
    struct MemorySegment* prev;
    int enda_addr;
} MS;

typedef struct MainNode {
    int *st_addr;
    void*memory;
    struct MemorySegment* sub_chain;
    struct MainNode* down;
    struct MainNode* up;
    int *end_addr;
} MainNode;
MainNode * headNode = NULL;
MainNode * headNode2 = NULL;
int prince = 0;
long int count = 1000;
void mems_init(){
    headNode = NULL;
    headNode2 = NULL;
}
int yo = 0;
MainNode *m1;
int toh = 0;//number of pages
void *mems_malloc(size_t size)
{
    size_t page_aligned_size;
    MainNode *previousNode = NULL;
    MainNode *head = headNode;
    while (head)
    {

        if (head->sub_chain != NULL)
        {
            MS *segment = head->sub_chain;
            while (segment != NULL)
            {
                if (segment->type == 0 && segment->size >= size)
                {
                    if (segment->size > size)
                    {
                        int store = segment->size;
                        segment->size = size;
                        segment->type = 1;
                        segment->enda_addr = segment->sta_addr + segment->size - 1;
                        // printf("MS2 sta: %d\n", segment->sta_addr);
                        // printf("MS2 end: %d\n", segment->enda_addr);

                        MS *hole = mmap(NULL, sizeof(MS), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                        hole->sta_addr = segment->enda_addr + 1;
                        hole->size = store - size;
                        hole->addr = (char *)segment->addr + size;
                        hole->type = 0;
                        hole->prev = segment;
                        hole->next = segment->next;
                        hole->enda_addr = hole->sta_addr + hole->size - 1;
                        // printf("Hole size: %d\n", hole->size);
                        // printf("Hole sta: %d\n", hole->sta_addr);
                        // printf("Hole end: %d\n", hole->enda_addr);

                        if (segment->next)
                        {
                            segment->next->prev = hole;
                        }
                        segment->next = hole;
                    }
                    else if (segment->type == 0 && segment->size == size)
                    {
                        segment->type = 1;
                    }
                    // return (void *)(segment->addr);
                    void *ptr = (void *)(intptr_t)segment->sta_addr;
                    return ptr;
                }
                segment = segment->next;
            }
        }
        previousNode = head;
        head = head->down;
    }

    int i = 1;
    while (size > (i * PAGE_SIZE))
    {
        i++;
    }
    page_aligned_size = i * PAGE_SIZE; //
    toh += i;
    MainNode*m1;
    m1 = mmap(NULL, sizeof(MainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    m1->memory = mmap(NULL, i * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    m1->down = NULL;
    m1->up = previousNode;
    m1->sub_chain = NULL;
    m1->st_addr = count;
    // count += page_aligned_size;
    m1->end_addr = count + page_aligned_size-1;
    if(previousNode){
        previousNode->down=m1;
    }
    if(prince==0){
        headNode=m1;
        prince=1;
    }
    // printf("MainNode1 sta: %d\n", m1->st_addr);
    // printf("MainNode1 end: %d\n", m1->end_addr);

    MS *ms1 = mmap(NULL, sizeof(MS), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ms1->size = size;
    ms1->addr = m1->memory;
    ms1->type = 1;
    ms1->next = NULL;
    ms1->prev = NULL;
    ms1->sta_addr = count;
    ms1->enda_addr = ms1->sta_addr + ms1->size - 1;
    count=ms1->sta_addr+ms1->size;
    m1->sub_chain = ms1;
    // printf("ms1 sta: %d\n", ms1->sta_addr);
    // printf("ms1 end: %d\n", ms1->enda_addr);
    // printf("ms1 size %d\n", size);
    // if (size == i * PAGE_SIZE)
    // {
    //     void *ptr = (void *)(intptr_t)ms1->sta_addr;
    //     return ptr;
    //     // return (void *) ms1->addr;
    //     // void ptr = (void *)(int ()(int));
    //     // (int ()(int))(&ptr) = ms1->sta_addr;
    //     // return ptr;
    // }
    MS *ms2 = mmap(NULL, sizeof(MS), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ms2->sta_addr = count;
    ms2->size = page_aligned_size - size;
    ms2->addr = (char *)ms1->addr + size;
    ms2->next = NULL;
    ms2->prev = ms1;
    ms2->type = 0;
    ms1->next = ms2;
    ms2->enda_addr =ms2->sta_addr+ms2->size-1;
    count=ms2->sta_addr+ms2->size;
    
    // printf("ms2: sta %d\n",ms2->sta_addr);
    // printf("ms2: end %d\n",ms2->enda_addr);
    // return (void *) ms1->addr;
    // void ptr = (void *)(int ()(int));
    // (int ()(int))(&ptr) = ms1->sta_addr;
    // return ptr;
    void *ptr = (void *)(intptr_t)ms1->sta_addr;
    return ptr;
}
void* mems_get(void*v_ptr){
    long int a = (long int)(intptr_t)v_ptr;
    MainNode* head = headNode;
    while(head){
        if(head->sub_chain){
            MS *segment = headNode->sub_chain;
            while(segment){
                if(segment->sta_addr <= a && a<=segment->enda_addr){
                    if(segment->type==1){
                        void*m_p_a=(char*) segment->addr + (a-segment->sta_addr);
                        return m_p_a;
                    }
                }
                segment=segment->next;
            }
        }
        head=head->down;
    }
    return NULL;  
}
// void *mems_get(void*v_ptr){
//     long a retrievedValue = (long int)(intptr_t)v_ptr;
//     printf("%d\n", a);
//     void * ptr;
//     while(headNode2){
//         MS* sub = headNode2->sub_chain;
//         while(sub){
//             printf("inside sub\n");
//             if(sub->sta_addr<= a && sub->enda_addr>= a){
//                 // void* yo = (char*)headNode2->memory + a;
//                 printf("inside if\n");
//                 long int yo = (intptr_t)headNode2->memory + a - count2;
//                 ptr = &yo;
//                 printf("%d\n", count2);
//                 return ptr;
//             }
//             sub = sub->next;
//         }
//         headNode2 = headNode2->down;
//     }
//     return NULL;
// }
void mems_print_stats(){
    long long int unused = 0;
    int elements = 0;
    int num_seg = 0;
    int eleArr[30];
    MainNode* head = headNode;
    while(head){
        printf("MAIN[%d,%d]->", head->st_addr, head->end_addr);
        yo += head->end_addr - head->st_addr + 1;
        if(head->sub_chain){
            MS *segment = head->sub_chain;
            while(segment){
                if(segment->type == 0){
                // segment=segment->next;
                    printf("H[%d,%d]<->", segment->sta_addr, segment->enda_addr);
                    unused += segment->size;
                    num_seg++;
                }else if (segment->type == 1)
                {
                    printf("P[%d,%d]<->", segment->sta_addr, segment->enda_addr);
                    num_seg++;
                }
                segment=segment->next;
                if(segment == NULL){
                    printf("NULL");
                }
            }
            printf("\n");
        }
        head=head->down;
        eleArr[elements] = num_seg;
        elements++;
        num_seg = 0;
    }
    printf("Pages used:\t%d\n", toh);
    printf("space unused: \t%lld\n", unused);
    printf("Main chain length:\t%d\n", elements);
    printf("sub-chain length array: [");
    for (int i = 0; i < elements; i++)
    {
        printf("%d, ", eleArr[i]);
    }
    printf("]");
    printf("\n");
}
void mems_free(void*po_ptr){
long int a = (long int)(intptr_t)po_ptr;
    MainNode* head = headNode;
    while(head){
        if(head->sub_chain){
            MS *segment = head->sub_chain;
            while(segment){
                if(segment->sta_addr <= a && a <= segment->enda_addr){
                    if(segment->next->type == 0){
                        MS* segment2 = segment->next;
                        size_t combined_size = segment->size + segment2->size;
                        segment->size = combined_size;
                        segment->next = segment2->next;
                        segment->enda_addr = segment2->enda_addr;
                        segment->type = 0;
                        if(segment2->next != NULL){
                        segment2->next->prev = segment2->prev;
                        }
                        munmap(segment2, sizeof(MS));
                        break;
                    }else if(segment->prev->type == 0){
                        MS* segment2 = segment->prev;
                        size_t combined_size = segment->size + segment2->size;
                        segment2->size = combined_size;
                        segment2->next = segment->next;
                        segment2->enda_addr = segment->enda_addr;
                        segment2->type = 0;
                        if(segment->next != NULL){
                        segment->next->prev = segment->prev;
                        }
                        munmap(segment, sizeof(MS));
                        break;
                    }
                }
                segment=segment->next;
            }
        }
        head=head->down;
    }
    return NULL;  
}
// mems_finish(){
//     MainNode* head = headNode;
//     while(head){
//         int yonda = head->end_addr - head->st_addr + 1;
//         munmap(head->memory, yonda);
//         head = head->down;
//     }
//     printf("\n------- memory_unmaped_successfully------- \n");
//     return NULL;  
// }
void mems_finish(){
    MainNode* head = headNode;
    MainNode* next;
    while(head){
        int yonda = head->end_addr - head->st_addr + 1;
        munmap(head->memory, yonda);
        MS* head2 = head->sub_chain;
        MS* next2;
        while(head2){
            next2 = head2->next;
            munmap(head2, sizeof(MS));
            head2 = next2;
        }
        next = head->down;
        munmap(head2, sizeof(MS));
        head = next;
    }
    printf("\n------- memory_unmaped_successfully------- \n");
}