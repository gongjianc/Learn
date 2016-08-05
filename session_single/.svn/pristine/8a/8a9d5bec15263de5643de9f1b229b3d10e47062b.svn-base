#include<stdio.h>
#include <pcap.h>
#include <semaphore.h>
#include <pthread.h>
#define MAX_RING_BUFFER_COUNT 50

/**Structure of Each node for a ring buffer **/
/**Structure of Each node for a ring buffer **/
struct sfive_attr {
	uint32_t ip_src;
	uint32_t ip_dst;
	uint16_t port_src;
	uint16_t port_dst;
	uint32_t protocol;
	//uint8_t  flow_flag;
	//void     *flow;
};
typedef struct sfive_attr session_fft;

typedef struct ring_buffer_node_tag {
	int irb_node_data_size;
	struct pcap_pkthdr pkthdr;
	session_fft session_five_attr;
	void *pdata_buffer;
} RING_BUFFER_NODE;

/**Decleration of Header Structure**/

struct ring_buffer_holder {
	RING_BUFFER_NODE *rb_base;
	int irb_qlength;
	int irb_head_index;
	int irb_tail_index;
	int irb_buffer_end_index;
	sem_t empty_buffer;
	sem_t full_buffer;
	pthread_mutex_t mutex;
} rb_holder[MAX_RING_BUFFER_COUNT];

int rbuff_create(int irb_qlength);
int rbuff_putdata(int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *session_five_attr);
int rbuff_getdata(int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *session_five_attr);
int rbuff_destroy(int iringbuffer_ID);
