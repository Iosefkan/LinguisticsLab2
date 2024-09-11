#include <Utils.h>

std::string GetString()
{
	std::string result;
	while (!(std::cin >> result))
	{
		std::cout << "Invalid input, try again\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return result;
}

std::string GetLocalDateString(std::chrono::time_point<std::chrono::system_clock>& time_point) {
	auto in_time_t = std::chrono::system_clock::to_time_t(time_point);
	std::stringstream ss;
	std::tm tm = {};
	localtime_s(&tm, &in_time_t);
	ss << std::put_time(&tm, "%Y-%m-%d");
	return ss.str();
}

std::chrono::time_point<std::chrono::system_clock> GetTimePoint(const std::string& value) {
	std::tm tm = {};
	std::stringstream ss(value);
	ss >> std::get_time(&tm, "%Y-%m-%d");
	return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

size_t Split(const std::string& txt, std::vector<std::string>& strs, char ch)
{
	size_t pos = txt.find(ch);
	size_t initialPos = 0;
	strs.clear();

	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1 != 0 ? pos + 1 : pos;
		pos = txt.find(ch, initialPos);
	}

	strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos));

	return strs.size();
}

int GetDaysBetweenTimeStamps(Time& tp1, Time& tp2) {
	return std::chrono::duration_cast<std::chrono::days>(tp1 - tp2).count();
}

std::string GetTimeDifferenceHHMMSS(Time& tp1, Time& tp2) {
	auto diff = std::chrono::hh_mm_ss{ tp1 - tp2 };
	std::stringstream ss;
	ss << diff.hours().count() << ":" << diff.minutes().count() << ":" << diff.seconds().count();
	return ss.str();
}

std::string Round(double value, int precision)
{
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(precision) << value;
	std::string s = ss.str();
	if (precision > 0 && s[s.find_last_not_of('0')] == '.') {
		s.erase(s.size() - precision + 1);
	}
	return s;

	//std::string rounded = std::to_string(std::round(value / precision) * precision);
	//rounded.erase(rounded.find_last_not_of('0') + 1, std::string::npos);
	//rounded.erase(rounded.find_last_not_of('.') + 1, std::string::npos);
	//return rounded;
}