#include <PurchaseLogAnalyzer.h>

#define SAVE_TO_FILE 0x31

std::vector<PurchaseLog*> ReadLogFromConsole() {
    std::vector<PurchaseLog*> result;
    std::vector<std::string> buffer;
    std::vector<std::string> logs;
    std::cout << "Input how many rows will be in your log" << std::endl;
    int rows = GetInt();
    std::cout << "Consecutively input each log row in separate line in the following format:" << std::endl
        << "Purchase date YYYY-MM-DD SPACE Brand name SPACE Mileage SPACE Price per gallon SPACE Number of gallons SPACE Total cost" << std::endl;
    for (int i = 0; i < rows;) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        Split(iss.str(), buffer, ' ');
        try {
            auto purchase = new PurchaseLog(buffer);
            result.push_back(purchase);
        }
        catch (...) {
            std::cout << "Inputted string contains incorrect data, input again" << std::endl;
            continue;
        }
        logs.push_back(line);
        i++;
    };

    std::cout
        << "Press 1 if you want to save gas purchas logs to the file, otherwise press anything else" << std::endl;
    auto option = _getch();
    if (option == SAVE_TO_FILE) {
        SaveToFileLogs(logs);
    }

    return result;
}

std::vector<PurchaseLog*> ReadLogFromFile(std::string path) {
    std::ifstream file(path);
    std::vector<PurchaseLog*> result;
    bool isCorrupted = false;
    if (!file.is_open()) {
        std::cout << "File at the path does not exist" << std::endl;
        return result;
    }
    std::string line;
    std::vector<std::string> buffer;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        Split(iss.str(), buffer, ' ');
        try {
            auto purchase = new PurchaseLog(buffer);
            result.push_back(purchase);
        }
        catch (...) {
            isCorrupted = true;
            continue;
        }
    }
    if (isCorrupted) {
        std::cout << std::endl << "Some of the file data is corrupted." << std::endl;
    }
    return result;
}

TextTable GetAnalyzedLogs(std::vector<PurchaseLog*>& data) {
    size_t size = data.size();
    TextTable table('-', '|', '+');
    InitStatisticsTable(table);
    double prevMilesDiff = 0;
    double prevMilesPerGallon = 0;
    double prevPricePerMile = 0;
    double prevPricePerGallon = size == 1 ? data[0]->GetPricePerGallon() : 0;
    double prevPricePerDay = 0;
    double prevTimePerGallon = 0;

    for (size_t i = 0; i < size; i++) {
        auto& log1 = data[i];
        if (i == size - 1) {
            AddLogToTable(table, log1);
            table.add("");
            table.add("");
            table.add("");
            table.add("");
            table.add("");
        }
        else {
            auto& log2 = data[i + 1];
            AddLogToTable(table, log1);
            int dayDiff = GetDaysBetweenTimeStamps(log2->GetDate(), log1->GetDate());
            double milesDiff = log2->GetMiles() - log1->GetMiles();
            double milesPerGallon = milesDiff / log1->GetTotalGallons();
            double pricePerMile = log1->GetCost() / milesDiff;
            double pricePerDay = log1->GetCost() / dayDiff;
            double timePerGallon = dayDiff / log1->GetTotalGallons();
            table.add(Round(milesDiff));
            table.add(Round(milesPerGallon));
            table.add(Round(pricePerMile));
            table.add(Round(pricePerDay));
            table.add(Round(timePerGallon));
            if (i == 0) {
                prevMilesDiff = milesDiff;
                prevMilesPerGallon = milesPerGallon;
                prevPricePerMile = pricePerMile;
                prevPricePerGallon = log1->GetPricePerGallon();
                prevPricePerDay = pricePerDay;
                prevTimePerGallon = timePerGallon;
            }
            else {
                prevMilesDiff = (milesDiff + prevMilesDiff) / 2;
                prevMilesPerGallon = (milesPerGallon + prevMilesPerGallon) / 2;
                prevPricePerMile = (pricePerMile + prevPricePerMile) / 2;
                prevPricePerGallon = (log1->GetPricePerGallon() + prevPricePerGallon) / 2;
                prevPricePerDay = (pricePerDay + prevPricePerDay) / 2;
                prevTimePerGallon = (timePerGallon + prevTimePerGallon) / 2;
            }
        }
        table.add(Round(prevMilesDiff));
        table.add(Round(prevMilesPerGallon));
        table.add(Round(prevPricePerMile));
        table.add(Round(prevPricePerGallon));
        table.add(Round(prevPricePerDay));
        table.add(Round(prevTimePerGallon));
        table.endOfRow();
    }
    return table;
}

TextTable GetAnalyzedLogsByBrands(std::vector<PurchaseLog*>& data) {
    size_t size = data.size();
    TextTable table('-', '|', '+');
    std::unordered_map<std::string, BrandStatistics*> statistics;
    InitBrandStatisticsTable(table);

    for (size_t i = 0; i < size; i++) {
        auto& log1 = data[i];
        if (i == size - 1) {
            auto entry = statistics.find(log1->GetBrand());
            if (entry == statistics.end()) {
                auto brandSt = new BrandStatistics();
                brandSt->averagePricePerGallon = log1->GetPricePerGallon();
                brandSt->averageCost = log1->GetCost();
                statistics[log1->GetBrand()] = brandSt;
            }
            else {
                auto existedBrandSt = entry->second;
                existedBrandSt->averageCost = (log1->GetCost() + existedBrandSt->averageCost) / 2;
                existedBrandSt->averagePricePerGallon = (log1->GetPricePerGallon() + existedBrandSt->averagePricePerGallon) / 2;
            }
        }
        else {
            auto& log2 = data[i + 1];
            int dayDiff = GetDaysBetweenTimeStamps(log2->GetDate(), log1->GetDate());
            double milesDiff = log2->GetMiles() - log1->GetMiles();
            double milesPerGallon = milesDiff / log1->GetTotalGallons();
            double pricePerMile = log1->GetCost() / milesDiff;
            double pricePerDay = log1->GetCost() / dayDiff;
            double timePerGallon = dayDiff / log1->GetTotalGallons();
            auto entry = statistics.find(log1->GetBrand());
            if (entry == statistics.end()) {
                auto newBrandSt = new BrandStatistics();
                newBrandSt->averageCost = log1->GetCost();
                newBrandSt->averagePricePerGallon = log1->GetPricePerGallon();
                newBrandSt->averageMilesDiff = milesDiff;
                newBrandSt->averageMilesPerGallon = milesPerGallon;
                newBrandSt->averagePricePerMile = pricePerMile;
                newBrandSt->averagePricePerDay = pricePerDay;
                newBrandSt->averageTimePerGallon = timePerGallon;
                statistics[log1->GetBrand()] = newBrandSt;
            }
            else {
                auto existedBrandSt = entry->second;
                existedBrandSt->averageCost = (log1->GetCost() + existedBrandSt->averageCost) / 2;
                existedBrandSt->averagePricePerGallon = (log1->GetPricePerGallon() + existedBrandSt->averagePricePerGallon) / 2;
                existedBrandSt->averageMilesDiff = (milesDiff + existedBrandSt->averageMilesDiff) / 2;
                existedBrandSt->averageMilesPerGallon = (milesPerGallon + existedBrandSt->averageMilesPerGallon) / 2;
                existedBrandSt->averagePricePerMile = (pricePerMile + existedBrandSt->averagePricePerMile) / 2;
                existedBrandSt->averagePricePerDay = (pricePerDay + existedBrandSt->averagePricePerDay) / 2;
                existedBrandSt->averageTimePerGallon = (timePerGallon + existedBrandSt->averageTimePerGallon) / 2;
            }
        }
    }

    for (auto& entry : statistics) {
        table.add(entry.first);
        AddBrandStatisticsToTable(table, entry.second);
        table.endOfRow();
    }

    return table;
}

void SaveToFileLogs(std::vector<std::string>& logs) {
    std::cout << "Input file name" << std::endl;
    while (true)
    {
        std::string fileName = GetString();
        if (fileName.find(".txt") >= std::string::npos)
        {
            std::cout << "Incorrect file name, try again" << std::endl;
            continue;
        }
        std::ifstream check(fileName);
        if (check.is_open())
        {
            std::cout
                << "File with this name already exists" << std::endl
                << "Do you want to rewrite this file?" << std::endl
                << "If you do, then input 'yes', otherwise input anything else" << std::endl;
            std::string rewrite = GetString();
            if (rewrite == "yes")
            {
                check.close();
                std::ofstream input(fileName);
                if (!input.is_open())
                {
                    std::cout << "This file is read-only" << std::endl;
                    std::cout << "Input file name" << std::endl;
                    continue;
                }
                if (!std::filesystem::is_regular_file(fileName))
                {
                    std::cout << "This file name is not allowed" << std::endl;
                    input.close();
                    remove(fileName.c_str());
                }
                {
                    for (auto& line : logs) {
                        input << line << std::endl;
                    }
                }
                std::cout << "Data is saved" << std::endl;
                break;
            }
            else {
                std::cout << "Input another filename" << std::endl;
            }
        }
        else
        {
            check.close();
            std::ofstream input(fileName);
            if (!input.is_open())
            {
                std::cout << "This file is read-only" << std::endl;
                std::cout << "Input file name" << std::endl;
                continue;
            }
            if (!std::filesystem::is_regular_file(fileName))
            {
                std::cout << "This file name is not allowed" << std::endl;
                input.close();
                remove(fileName.c_str());
            }
            {
                for (auto& line : logs) {
                    input << line << std::endl;
                }
            }
            std::cout << "Data is saved" << std::endl;
            break;
        }
    }
}

void SaveToFile(TextTable& analyzed, TextTable& analyzedByBrand) {
    std::cout << "Input file name" << std::endl;
    while (true)
    {
        std::string fileName = GetString();
        if (fileName.find(".txt") >= std::string::npos)
        {
            std::cout << "Incorrect file name, try again" << std::endl;
            continue;
        }
        std::ifstream check(fileName);
        if (check.is_open())
        {
            std::cout
                << "File with this name already exists" << std::endl
                << "Do you want to rewrite this file?" << std::endl
                << "If you do, then input 'yes', otherwise input anything else" << std::endl;
            std::string rewrite = GetString();
            if (rewrite == "yes")
            {
                check.close();
                std::ofstream input(fileName);
                if (!input.is_open())
                {
                    std::cout << "This file is read-only" << std::endl;
                    std::cout << "Input file name" << std::endl;
                    continue;
                }
                if (!std::filesystem::is_regular_file(fileName))
                {
                    std::cout << "This file name is not allowed" << std::endl;
                    input.close();
                    remove(fileName.c_str());
                }
                {
                    InitAbbreviations(input);
                    input
                        << std::endl
                        << "Analyzed by each log entry:" << std::endl
                        << analyzed << std::endl;
                    input
                        << "Analyzed by brand:" << std::endl
                        << analyzedByBrand << std::endl;
                }
                std::cout << "Data is saved" << std::endl;
                break;
            }
            else {
                std::cout << "Input another filename" << std::endl;
            }
        }
        else
        {
            check.close();
            std::ofstream input(fileName);
            if (!input.is_open())
            {
                std::cout << "This file is read-only" << std::endl;
                std::cout << "Input file name" << std::endl;
                continue;
            }
            if (!std::filesystem::is_regular_file(fileName))
            {
                std::cout << "This file name is not allowed" << std::endl;
                input.close();
                remove(fileName.c_str());
            }
            {
                InitAbbreviations(input);
                input
                    << std::endl
                    << "Analyzed by each log entry:" << std::endl
                    << analyzed << std::endl;
                input
                    << "Analyzed by brand:" << std::endl
                    << analyzedByBrand << std::endl;
            }
            std::cout << "Data is saved" << std::endl;
            break;
        }
    }
}

void InitAbbreviations(std::ostream& stream) {
    stream << "D\t=>\tDate" << std::endl
        << "B\t=>\tBrand" << std::endl
        << "M\t=>\tMiles" << std::endl
        << "PPG\t=>\tPrice per gallon" << std::endl
        << "G\t=>\tGallons" << std::endl
        << "C\t=>\tCost" << std::endl
        << "MD\t=>\tMiles on the tank" << std::endl
        << "MPG\t=>\tMiles per gallon" << std::endl
        << "PPM\t=>\tPrice per mile" << std::endl
        << "PPD\t=>\tPrice per day" << std::endl
        << "DPG\t=>\tDays per gallon" << std::endl
        << "AC\t=>\tAverage cost" << std::endl
        << "AMD\t=>\tAverage miles on the tank" << std::endl
        << "AMPG\t=>\tAverage miles per gallon" << std::endl
        << "APPM\t=>\tAverage price per mile" << std::endl
        << "APPG\t=>\tAverage price per gallon" << std::endl
        << "APPD\t=>\tAverage price per day" << std::endl
        << "ADPG\t=>\tAverage days per gallon" << std::endl;
}

void InitStatisticsTable(TextTable& table) {
    table.add("D");
    table.add("B");
    table.add("M");
    table.add("PPG");
    table.add("G");
    table.add("C");
    table.add("MD");
    table.add("MPG");
    table.add("PPM");
    table.add("PPD");
    table.add("DPG");
    table.add("AMD");
    table.add("AMPG");
    table.add("APPM");
    table.add("APPG");
    table.add("APPD");
    table.add("ADPG");
    table.endOfRow();
}

void InitBrandStatisticsTable(TextTable& table) {
    table.add("");
    table.add("AC");
    table.add("AMD");
    table.add("AMPG");
    table.add("APPM");
    table.add("APPG");
    table.add("APPD");
    table.add("ADPG");
    table.endOfRow();
}

void AddLogToTable(TextTable& table, PurchaseLog* log) {
    table.add(GetLocalDateString(log->GetDate()));
    table.add(log->GetBrand());
    table.add(Round(log->GetMiles()));
    table.add(Round(log->GetPricePerGallon()));
    table.add(Round(log->GetTotalGallons()));
    table.add(Round(log->GetCost()));
}

void AddBrandStatisticsToTable(TextTable& table, BrandStatistics* brandSt) {
    table.add(Round(brandSt->averageCost));
    table.add(Round(brandSt->averageMilesDiff));
    table.add(Round(brandSt->averageMilesPerGallon));
    table.add(Round(brandSt->averagePricePerMile));
    table.add(Round(brandSt->averagePricePerGallon));
    table.add(Round(brandSt->averagePricePerDay));
    table.add(Round(brandSt->averageTimePerGallon));
}