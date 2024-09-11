#include <PurchaseLog.h>

PurchaseLog::PurchaseLog(std::vector<std::string> data) {
	if (data.size() < 6) {
		throw std::invalid_argument("Incorrect format");
	}
	_date = GetTimePoint(data[0]);
	_brand = data[1];
	_miles = std::stod(data[2]);
	_price = std::stod(data[3]);
	_gallons = std::stod(data[4]);
	_cost = std::stod(data[5]);
}

Time& PurchaseLog::GetDate() {
	return _date;
}
std::string& PurchaseLog::GetBrand() {
	return _brand;
}
double& PurchaseLog::GetMiles() {
	return _miles;
}
double& PurchaseLog::GetPricePerGallon() {
	return _price;
}
double& PurchaseLog::GetTotalGallons() {
	return _gallons;
}
double& PurchaseLog::GetCost() {
	return _cost;
}