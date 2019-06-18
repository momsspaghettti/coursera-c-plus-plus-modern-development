#include "spendings_xml.h"
#include "spendings_json.h"
#include "test_ini_parser.h"
#include "refactoring.h"


int main()
{
	TestSpendingsXML();
	TestSpendingsJSON();
	TestINIParser();
	TestRefactoring();

    return 0;
}