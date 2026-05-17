#include "types.h"
#include <stddef.h>

typedef struct Stream Stream;

Stream *stream_init(size_t nbytes);
void stream_push(Stream *s, const u8 data[], size_t len);
int stream_read_from(Stream *s, int FD);
u8 *stream_data(Stream *s);
size_t stream_length(Stream *s);
