#include<stdio.h>
#include <pcap.h>
#include <semaphore.h>
#include <pthread.h>
#define MAX_RING_BUFFER_COUNT 50

/**Structure of Each node for a ring buffer **/

typedef struct ring_buffer_node_tag
{
    int irb_node_data_size;
    struct pcap_pkthdr pkthdr;
    struct tuple5 five_attr;
    void *pdata_buffer;
}RING_BUFFER_NODE;


/**Decleration of Header Structure**/

struct ring_buffer_holder
{
    RING_BUFFER_NODE *rb_base;
    int irb_qlength;
    int irb_head_index;
    int irb_tail_index;
    int irb_buffer_end_index;
    sem_t empty_buffer;
    sem_t full_buffer; 
    pthread_mutex_t mutex;
}rb_holder[MAX_RING_BUFFER_COUNT];

 int ibuff_create (int irb_qlength);
 int ibuff_putdata(int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *five_attr);
 int ibuff_getdata(int iringbuffer_ID, void *pData, struct pcap_pkthdr hdr, struct tuple5 *five_attr);
 int ibuff_destroy(int iringbuffer_ID);
