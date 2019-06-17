#include "UnitTestsFramework.h"
#include "secondary_index.h"
#include <string>


void TestRangeBoundaries() 
{
	const int good_karma = 1000;
	const int bad_karma = -10;

	Database db;
	db.Put({ "id1", "Hello there", "master", 1536107260, good_karma });
	db.Put({ "id2", "O>>-<", "general2", 1536107260, bad_karma });

	int count = 0;
	db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) 
	{
		++count;
		return true;
	});

	ASSERT_EQUAL(2, count);

	ASSERT(db.Erase("id1"));
	ASSERT(db.Put({ "id1", "Hello there", "master", 1536107260, good_karma }));
	ASSERT(!db.Put({ "id1", "Hello there", "master", 1536107260, good_karma }));
	ASSERT(!db.Erase("id3"));
}


void TestSameUser() 
{
	Database db;
	db.Put({ "id1", "Don't sell", "master", 1536107260, 1000 });
	db.Put({ "id2", "Rethink life", "master", 1536107260, 2000 });
	db.Put({ "id3", "Don't sell", "slave", 1536107260, 1000 });
	db.Put({ "id4", "Rethink life", "slave", 1536107260, 2000 });

	int count = 0;
	
    db.AllByUser("master", [&count](const Record&) 
	{
		++count;
		return true;
	});

	ASSERT_EQUAL(2, count);
}


void TestReplacement() 
{
	const std::string final_body = "Feeling sad";

	Database db;
	db.Put({ "id", "Have a hand", "not-master", 1536107260, 10 });
	db.Erase("id");
	db.Put({ "id", final_body, "not-master", 1536107260, -10 });

	auto record = db.GetById("id");
	ASSERT(record != nullptr);
	ASSERT_EQUAL(final_body, record->title);
}


void TestSecondaryIndex()
{
	TestRunner tr;
	RUN_TEST(tr, TestRangeBoundaries);
	RUN_TEST(tr, TestSameUser);
	RUN_TEST(tr, TestReplacement);
}
