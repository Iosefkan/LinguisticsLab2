#include <PurchaseLogAnalyzer.h>

int main()
{
	std::cout << "Please input the path to the text than needs to be analyzed" << std::endl;
	std::string path = GetString();
	auto logs = ReadLogFromFile(path);
	if (logs.size() <= 0) {
		return EXIT_FAILURE;
	}
	InitAbbreviations();
	std::cout << std::endl;
	PrintAnalyzedLogs(logs);
	std::cout << std::endl;
	PrintAnalyzedLogsByBrands(logs);
	std::cin.get();
}