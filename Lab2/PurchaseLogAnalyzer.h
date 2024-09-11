#pragma once

#include <PurchaseLog.h>
#include <fstream>
#include <unordered_map>

struct BrandStatistics {
	double averageCost = 0;
	double averageMilesDiff = 0;
	double averageMilesPerGallon = 0;
	double averagePricePerMile = 0;
	double averagePricePerGallon = 0;
	double averagePricePerDay = 0;
	double averageTimePerGallon = 0;
};

std::vector<PurchaseLog*> ReadLogFromFile(std::string path);

void PrintAnalyzedLogs(std::vector<PurchaseLog*>& data);

void PrintAnalyzedLogsByBrands(std::vector<PurchaseLog*>& data);

void InitAbbreviations();

void InitStatisticsTable(TextTable& table);

void InitBrandStatisticsTable(TextTable& table);

void AddLogToTable(TextTable& table, PurchaseLog* log);

void AddBrandStatisticsToTable(TextTable& table, BrandStatistics* brandSt);