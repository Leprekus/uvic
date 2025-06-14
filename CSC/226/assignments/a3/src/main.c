#include "utils/reader.h"
#include "utils/graphBuilder.h"
int main(void) {
	const size_t lineLen = 128;
	parseFile("../CC-Neuron_cci.tsv", lineLen, &processLine);
	return 0;
}
