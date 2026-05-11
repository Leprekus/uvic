#include "io_buf.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

struct WriteBuf {
	size_t size;
	size_t count;
	u8 data[BUF_LEN];
};

WriteBuf *writeBufInit() {
	WriteBuf *w = malloc(sizeof(WriteBuf));
	if(!w) return NULL;
	w->size = BUF_LEN;
	w->count = 0;
	memset(w->data, 0, BUF_LEN);
	return w;

	//WriteBuf outBuf = { .size = BUF_LEN, .count = 0, .data = {0} };
}
// returns the new size of the buffer 
void bufferedWrite(WriteBuf *buf, const void *data, size_t size){
	if(buf->size == buf->count) flush(buf);

	u8 *temp = (u8 *)data;
	size_t capacity = buf->size - buf->count;
	while(size > capacity) {
		memcpy(buf->data + buf->count, temp, capacity);
		buf->count += capacity;
		temp += capacity;

		flush(buf);

		size -= capacity; 
		capacity = buf->size;
		buf->count = 0;
	}
	if(size > 0) {
		memcpy(buf->data, temp, size);
		buf->count = size;
	}
}

void flush(WriteBuf *buf) {
	if(buf->count == 0) return;
	write(1, buf->data, buf->count);
	buf->count = 0;
}
