#include "priority_collection.h"
#include "hash_set.h"
#include "set_iterator_next.h"
#include "hash_point.h"
#include "hash_person.h"
#include "secondary_index.h"


int main()
{
	TestPriorityCollection();
	TestHashSet();
	TestSetIteratorNext();
	TestHashPoint();
	TestHashPerson();
	TestSecondaryIndex();

    return 0;
}