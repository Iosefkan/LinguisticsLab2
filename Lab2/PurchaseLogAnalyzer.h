#pragma once

#include <PurchaseLog.h>
#include <unordered_map>
#include <conio.h>

struct BrandStatistics {
	double averageCost = 0;
	double averageMilesDiff = 0;
	double averageMilesPerGallon = 0;
	double averagePricePerMile = 0;
	double averagePricePerGallon = 0;
	double averagePricePerDay = 0;
	double averageTimePerGallon = 0;
};

void SaveToFileLogs(std::vector<std::string>& logs);

std::vector<PurchaseLog*> ReadLogFromConsole();

std::vector<PurchaseLog*> ReadLogFromFile(std::string path);

TextTable GetAnalyzedLogs(std::vector<PurchaseLog*>& data);

TextTable GetAnalyzedLogsByBrands(std::vector<PurchaseLog*>& data);

void InitAbbreviations(std::ostream& stream);

void InitStatisticsTable(TextTable& table);

void InitBrandStatisticsTable(TextTable& table);

void AddLogToTable(TextTable& table, PurchaseLog* log);

void AddBrandStatisticsToTable(TextTable& table, BrandStatistics* brandSt);

void SaveToFile(TextTable& analyzed, TextTable& analyzedByBrand);