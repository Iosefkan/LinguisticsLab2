#include <PurchaseLogAnalyzer.h>

std::vector<PurchaseLog*> ReadLogFromFile(std::string path) {
    std::ifstream file(path);
    std::vector<PurchaseLog*> result;
    bool isCorrupted = false;
    if (!file.is_open()) {
        std::cout << "File at the path does not exist";
        return result;
    }
    std::string line;
    std::vector<std::string> buffer;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        Split(iss.str(), buffer, ' ');
        try {
            result.push_back(new PurchaseLog(buffer));
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

void PrintAnalyzedLogs(std::vector<PurchaseLog*>& data) {
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
            double timePerGallon = log1->GetTotalGallons() / dayDiff;
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
    std::cout << "Analyzed by each log entry:" << std::endl;
    std::cout << table;
}

void PrintAnalyzedLogsByBrands(std::vector<PurchaseLog*>& data) {
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
            double timePerGallon = log1->GetTotalGallons() / dayDiff;
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
    std::cout << "Analyzed by brand:" << std::endl;
    std::cout << table;

}

void InitAbbreviations() {
    std::cout << std::endl << "Due to limited space column headers are abbreviated." << std::endl
        << "Below are the abbreviated column headers and their full versions:" << std::endl;

    std::cout << "D\t=>\tDate" << std::endl
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