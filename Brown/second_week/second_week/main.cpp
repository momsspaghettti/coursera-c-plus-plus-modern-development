#include "spendings_xml.h"
#include "spendings_json.h"
#include "test_ini_parser.h"
#include "refactoring.h"
#include "comment_server.h"


int main()
{
    TestSpendingsXML();
	TestSpendingsJSON();
	TestINIParser();
	TestRefactoring();
	TestCommentServer();

    return 0;
}