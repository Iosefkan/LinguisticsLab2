#include <Utils.h>

class PurchaseLog {
private:
	Time _date;
	std::string _brand;
	double _miles;
	double _price;
	double _gallons;
	double _cost;
public:
	PurchaseLog(std::vector<std::string> data);
	Time& GetDate();
	std::string& GetBrand();
	double& GetMiles();
	double& GetPricePerGallon();
	double& GetTotalGallons();
	double& GetCost();
};