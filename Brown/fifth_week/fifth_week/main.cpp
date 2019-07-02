#include "stats.h"
#include "ebook.h"
#include "trains.h"
#include "tasks_tracker.h"
#include "hotels_booking.h"
#include "forbidden_domains.h"
#include "personal_budget.h"


int main()
{
	TestPrintStats();
	TestEBook();
	TestTrains();
	TestTasksTracker();
	TestHotelsBooking();
	TestForbiddenDomains();
	TestPersonalBudget();

    return 0;
}