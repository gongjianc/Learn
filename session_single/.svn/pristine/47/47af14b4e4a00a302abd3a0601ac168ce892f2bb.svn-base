#include <stdio.h>
#include <pcap.h>
#include <semaphore.h>
#include <pthread.h>
#define MAX_SRING_BUFFER_COUNT 50

/**Structure of Each node for a ring buffer **/
struct five_attr {
    uint32_t ip_src;
    uint32_t ip_dst;
    uint16_t port_src;
    uint16_t port_dst;
    uint32_t protocol;
};
typedef struct five_attr session_ft;

typedef struct session_buffer_node_tag
{
//    int irb_node_data_size;
    session_ft  session_five_attr;
//    void *pdata_buffer;
}SESSION_BUFFER_NODE;


/**Decleration of Header Structure**/

struct session_buffer_holder
{
    SESSION_BUFFER_NODE *rb_base;
    int irb_qlength;
    int irb_head_index;
    int irb_tail_index;
    int irb_buffer_end_index;
    sem_t empty_buffer;
    sem_t full_buffer; 
    pthread_mutex_t mutex;
}sb_holder[MAX_SRING_BUFFER_COUNT];

 int sbuff_create (int irb_qlength);
 int sbuff_putdata(int iringbuffer_ID, session_ft ftt);
 int sbuff_getdata(int iringbuffer_ID, session_ft *ftt);
 int sbuff_destroy(int iringbuffer_ID);
