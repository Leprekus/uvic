#include "types.h"
#include <stddef.h>

typedef struct Stream Stream;

Stream *streamInit(size_t nbytes);
void streamPush(Stream *s, const u8 data[], size_t len);
int streamReadFrom(Stream *s, int FD);
u8 *streamData(Stream *s);
size_t streamLength(Stream *s);
