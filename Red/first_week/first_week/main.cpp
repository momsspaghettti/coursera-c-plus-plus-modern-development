#include "max_element_if.h"
#include "logger.h"
#include "airline_ticket.h"
#include "print_values.h"
#include <iomanip>
#include "uniq_id.h"
#include "UnitTestsFramework.h"
#include "table.h"
#include "my_deque.h"
#include <ctime>


int main()
{
	TestAllMaxElemetIfTests();
	TestSortBy();
	TestUpdates();
	TestPrintValues();
	TestUniqId();
	TestTable();
	TestMyDeque();

	getchar();
	getchar();

    return 0;
}
