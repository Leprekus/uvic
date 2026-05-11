#include "types.h"
#include <stddef.h>

typedef struct bArray bArray;
typedef enum {
	SUCCESS,
	OUT_OF_BOUNDS,
} bStatus;
bArray *bArrayInit(size_t numBytes);
bStatus bArrayPush(bArray *a, u8 bit);
void bArrayPrint(bArray *a);
u8 reverse(u8 b);
size_t theOneAndOnlyRuleYouShouldNeverForgetWhenPushingNumericalValuesIntoAGZIPContainer(size_t num);
