#include <stdlib.h>
#include "beap_hash.h"
//#include "pcap.h"

hash_table * create_hash(int size, \
			t_hash_func hash_func, \
			t_comp_func comp_func)
{
  hash_table 	*tbl;
  int		i;
  
 //  printf("\n create_hash");
  if (size < 0 || size > MAX_HASH_SIZE)
  {
  
  	return NULL;
  }
  if (hash_func == 0)
  {

  	return NULL;
  }
	
  tbl = calloc(1, sizeof(hash_table));
  if ((tbl->hashtable = calloc(size, sizeof(hash_bucket *))) == NULL)
  {
  	return NULL;
  }

  memset(tbl->hashtable, 0, size * sizeof(hash_bucket *));
  
  tbl->nr_entries = 0;
  tbl->tbl_size = size;
  tbl->hash_func = hash_func;
  tbl->comp_func = comp_func;

  //printf("tbl_address: %08x\n", tbl);
  return tbl;
}

void delete_hash(hash_table *tbl)
{
  int i;
  hash_bucket *prev,*next;
  
//   printf("\n delete_hash");
//   printf("\n delete_hash");
  
  for (i = 0; i < tbl->tbl_size; i ++)
	if ((prev = tbl->hashtable[i]) == NULL)
		continue;
	else
		while(prev){
			next = prev->ptr.next_hash;
			LS_MEM_FREE(prev->content);
			LS_MEM_FREE(prev);
			prev = next;
		}
  tbl->nr_entries = 0;
  LS_MEM_FREE(tbl->hashtable);
}

void delete_hash_keep_content(hash_table *tbl)
{
  int i;
  hash_bucket *prev,*next;
  
  for (i = 0; i < tbl->tbl_size; i ++)
	if ((prev = tbl->hashtable[i]) == NULL)
		continue;
	else
		while(prev){
			next = prev->ptr.next_hash;
			LS_MEM_FREE(prev);
			prev = next;
		}
  tbl->nr_entries = 0;
  LS_MEM_FREE(tbl->hashtable);
}


void insert_hash(hash_table * tbl, hash_bucket * elem,int len)
{
  int depth;

//   printf("\n insert_hash");
//   printf("\n insert_hash");
  int ix = tbl -> hash_func(elem->key,len);
 //printf("\n insert_hash pos sssssssssssssssssssssssssssssssssssssssssssssssssssss%d",ix);
  
  hash_bucket ** base = &tbl->hashtable[ix];
  hash_bucket * ptr = *base;
  hash_bucket * prev = NULL;
  depth = 0;
  tbl->nr_entries++;
  while(ptr && tbl -> comp_func(ptr->key, elem->key,len)){
	base = &ptr->ptr.next_hash;
	prev = ptr;
	ptr = *base;
	depth ++;
  }
  elem->ptr.next_hash = ptr;
  elem->ptr.prev_hash = prev;
  if(ptr){
	ptr->ptr.prev_hash = elem;
  }
  *base = elem;
//   printf("bucket %d depth %d\n", ix, depth);
}

void remove_hash(hash_table * tbl, hash_bucket * elem,int len)
{
//   printf("\n remove_hash");
//   printf("\n remove_hash");
  hash_bucket * next = elem->ptr.next_hash;
  hash_bucket * prev = elem->ptr.prev_hash;

  tbl->nr_entries--;
  if(next)
	next->ptr.prev_hash = prev;
  if(prev)
	prev->ptr.next_hash = next;
  else {
	int ix = tbl -> hash_func(elem->key,len);
	tbl->hashtable[ix] = next;
  }
}

hash_bucket * find_hash(hash_table * tbl, void* pos,int len)
{
//   printf("\n find_hash");
//   printf("\n find_hash");
	int ix = tbl -> hash_func(pos,len);
 //  printf("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n find_hash %d",ix);
	hash_bucket * ptr = tbl->hashtable[ix];
	while(ptr){
		if(tbl -> comp_func(ptr->key, pos,len) == 0)
			break;
		ptr = ptr->ptr.next_hash;
	}
	return ptr;
}
////////////////////////////////////////////////////////////////////////////////////////

unsigned int session_hash(void *key,int len)
{
  int     i;
  UCHAR8 ret;
  ret = 0;
  for (i = 0; i < len; i ++)
  ret += *((UCHAR8 *)(key + i));
  return (ret & 0xff);
}


SCHAR8 session_comp(void *key1, void *key2,int len)
{
  if (!memcmp(key1, key2, len))
    return 0;
  else
    return 1;
}

