#include <PurchaseLogAnalyzer.h>
#include <Windows.h>

#define EXIT 0x1B
#define CONSOLE_INPUT 0x31
#define SAVE_TO_FILE 0x31
#define FILE_INPUT 0x32

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::cout << "Sedletskii G., 415" << std::endl
		<< "This program allows the user to see the statistics of gas purchase logs" << std::endl;

	while (true) {
		std::cout << "Press 1 to analyze console inputted gas purchase logs" << std::endl
			<< "Press 2 to analyze gas purchase logs from a .txt file" << std::endl
			<< "Press ESC to exit the program" << std::endl << std::endl;
		int option = _getch();
		std::vector<PurchaseLog*> logs;
		if (option == CONSOLE_INPUT) {
			logs = ReadLogFromConsole();
		}
		else if (option == FILE_INPUT) {
			std::cout << "Please input the path to the text than needs to be analyzed" << std::endl;
			std::string path = GetString();
			logs = ReadLogFromFile(path);
		}
		else if (option == EXIT) {
			break;
		}
		else {
			std::cout << "Pressed wrong key" << std::endl;
			continue;
		}

		if (logs.size() <= 0) {
			continue;
		}
		std::cout << std::endl << "Due to limited space column headers are abbreviated." << std::endl
			<< "Below are the abbreviated column headers and their full versions:" << std::endl;
		InitAbbreviations(std::cout);
		std::cout << std::endl;

		auto analyzed = GetAnalyzedLogs(logs);
		std::cout 
			<< "Analyzed by each log entry:" << std::endl
			<< analyzed << std::endl;

		auto analyzedByBrand = GetAnalyzedLogsByBrands(logs);
		std::cout 
			<< "Analyzed by brand:" << std::endl
			<< analyzedByBrand << std::endl;

		std::cout
			<< "Press 1 if you want to save result to the file, otherwise press anything else" << std::endl << std::endl;

		option = _getch();
		if (option == SAVE_TO_FILE) {
			SaveToFile(analyzed, analyzedByBrand);
		}

		for (auto& log : logs) {
			delete log;
		}
		logs.clear();
	}
}