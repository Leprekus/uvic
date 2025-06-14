#include "../include/utils/reader.h"
#include "../include/utils/graphBuilder.h"
/*
 * takes in tokens (src) and buff (dest)
 * buff's len must be >= token's
 * tokens must start with a valid string as
 * the macro will skip any null terminated characters and break
 * otherwise it copies the string into buff
 * j is used as buff's index
 *
 * */

