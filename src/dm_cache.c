/*
  
 Copyright (c) 2008 NFG Net Facilities Group BV support@nfg.nl

 This program is free software; you can redistribute it and/or 
 modify it under the terms of the GNU General Public License 
 as published by the Free Software Foundation; either 
 version 2 of the License, or (at your option) any later 
 version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "dbmail.h"
#include "dm_cache.h"

#define THIS_MODULE "Cache"
/*
 * cached raw message data
 */
#define T Cache_T

struct T {
	u64_t id;
	u64_t size;
	Mem_T memdump;
	Mem_T tmpdump;
	int file_dumped;
};

/* */
T Cache_new(void)
{
	int serr;
	T C;
	
	C = g_malloc0(sizeof(*C));

	C->id = -1;
	if (! (C->memdump = Mem_open())) {
		serr = errno;
		TRACE(TRACE_ERROR,"Mem_open() failed [%s]", strerror(serr));
		g_free(C);
		errno = serr;
		return NULL;
	}
	
	if (! (C->tmpdump = Mem_open())) {
		serr = errno;
		TRACE(TRACE_ERROR,"Mem_open() failed [%s]", strerror(serr));
		errno = serr;
		Mem_close(&C->memdump);
		g_free(C);
		return NULL;
	}

	return C;
}

u64_t Cache_set_dump(T C, char *buf, int dumptype)
{
	u64_t outcnt = 0;
	Mem_T M;
	char *rfc;

	switch (dumptype) {
		case IMAP_CACHE_MEMDUMP:
			M = Cache_get_memdump(C);
		break;
		case IMAP_CACHE_TMPDUMP:
			M = Cache_get_tmpdump(C);
		break;
		default:
			assert(0);
		break;
	}
	
	assert(M);
	Mem_rewind(M);

	rfc = get_crlf_encoded(buf);
	outcnt = Mem_write(M, rfc, strlen(rfc));
	g_free(rfc);
	Mem_rewind(M);

	return outcnt;
}

void Cache_clear(T C)
{
	C->id = 0;
	C->size = 0;
	Mem_close(&C->memdump);
	C->memdump = Mem_open();

	Mem_close(&C->tmpdump);
	C->tmpdump = Mem_open();
}


u64_t Cache_update(T C, DbmailMessage *message, int filter)
{
	u64_t tmpcnt = 0, outcnt = 0;
	char *buf = NULL;

	if (C->id == message->id) {
		outcnt = C->size;
	} else {

		Cache_clear(C);

		buf = dbmail_message_to_string(message);
		outcnt = Cache_set_dump(C,buf,IMAP_CACHE_MEMDUMP);
		tmpcnt = Cache_set_dump(C,buf,IMAP_CACHE_TMPDUMP);
		g_free(buf);

		assert(tmpcnt==outcnt);
		
		C->size = outcnt;
		C->id = message->id;
		
	}
	
	switch (filter) {
		/* for these two update the temp MEM buffer */	
		case DBMAIL_MESSAGE_FILTER_HEAD:
			buf = dbmail_message_hdrs_to_string(message);
			outcnt = Cache_set_dump(C,buf,IMAP_CACHE_TMPDUMP);
			g_free(buf);
		break;
		case DBMAIL_MESSAGE_FILTER_BODY:
			buf = dbmail_message_body_to_string(message);
			outcnt = Cache_set_dump(C,buf,IMAP_CACHE_TMPDUMP);
			g_free(buf);
		break;
		case DBMAIL_MESSAGE_FILTER_FULL:
			/* done */
		break;

	}

//	Mem_rewind(C->memdump);
//	Mem_rewind(C->tmpdump);
	
	TRACE(TRACE_DEBUG,"cache size [%llu]", outcnt);	
	return outcnt;
}


void Cache_set_memdump(T C, Mem_T M)
{
	assert(C);
	assert(M);
	C->memdump = M;
}

u64_t Cache_get_size(T C)
{
	assert(C);
	return C->size;
}
Mem_T Cache_get_memdump(T C)
{
	return C->memdump;
}

void Cache_set_tmpdump(T C, Mem_T M)
{
	assert(C);
	assert(M);
	C->tmpdump = M;
}

Mem_T Cache_get_tmpdump(T C)
{
	return C->tmpdump;
}


/*
 * closes the msg cache
 */
void Cache_free(T *C)
{
	T c = *C;
	c->id = -1;
	Mem_close(&c->memdump);
	Mem_close(&c->tmpdump);
	g_free(c);
	
}


