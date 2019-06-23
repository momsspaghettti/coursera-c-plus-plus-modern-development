#include "print_stats.h"
#include "lazy_value.h"
#include "synchronized.h"
#include "concurrent_map.h"
#include "polynomial.h"
#include "object_pool.h"


int main()
{
    TestPrintStats();
	TestLazyValue();
	TestSynchronized();
	TestConcurrentMap();
	TestPolynomial();
	TestObjectPool();

	return 0;
}