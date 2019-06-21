#include "print_stats.h"
#include "lazy_value.h"
#include "synchronized.h"
#include "concurrent_map.h"


int main()
{
	TestPrintStats();
	TestLazyValue();
	TestSynchronized();
	TestConcurrentMap();

	return 0;
}