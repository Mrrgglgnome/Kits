// Kits.cpp: определяет точку входа для приложения.
//

#include "Kits.h"

using namespace std;


struct item {
	int pos;
	int quant;
	vector<string> kits;
};

pair<bool, vector<item>> findMatch(vector<item> items, map<string, int> kit) {
	vector<item> result = {};
	for (int i = 0; i < items.size(); i++) {
		if (items[i].kits.size() < 1)
			continue;
		else if (items[i].kits.size() > 1) {
			for (int j = 0; j < items[i].kits.size(); j++) {
				vector<item> slicedItems = vector<item>(items.begin() + i, items.end());
				slicedItems[0].kits.erase(slicedItems[0].kits.begin() + j);
				auto tryMatch = findMatch(slicedItems, kit);
				if (tryMatch.first) {
					for (const auto& element : tryMatch.second) {
						result.push_back(element);
					}
					return { true, result };
				}
			}
		}
		else {
			string catalog = items[i].kits[0];
			if (kit.contains(catalog) && kit[catalog] > 0) {
				if (items[i].quant >= kit[catalog]) {
					result.push_back({ items[i].pos, kit[catalog], { catalog } });
					items[i].quant -= kit[catalog];
					kit[catalog] = 0;
				}
				else {
					result.push_back({ items[i].pos, items[i].quant, { catalog } });
					kit[catalog] -= items[i].quant;
					items[i].quant = 0;
				}
			}
		}
		for (int checkCount = 0; auto check : kit) {
			if (check.second == 0)
				checkCount += 1;
			else
				break;
			if (checkCount == kit.size())
				return { true, result };
		}
	}
	return { false, {} };
}

int main()
{
	setlocale(LC_ALL, "rus");
	ifstream inDoc("document.txt");
	ifstream inKit("kit.txt");

	vector<item> items;

	map<string, int> kit;
	if (inKit.is_open()) {
		string name;
		int quant;
		while (inKit >> name >> quant) {
			kit[name] = quant;
		}
	}
	inKit.close();

	if (inDoc.is_open()) {
		string line;
		while (getline(inDoc, line)) {
			int tmpPos;
			int tmpQuant;
			string tmpStringKits;
			vector<string> tmpKits;
			stringstream ss(line);
			ss >> tmpPos >> tmpQuant >> tmpStringKits;
			size_t start{ tmpStringKits.find_first_not_of(",") };
			while (start != string::npos) {
				size_t end = tmpStringKits.find_first_of(",", start + 1);
				if (end == string::npos)
					end = tmpStringKits.length();
				tmpKits.push_back(tmpStringKits.substr(start, end - start));
				start = tmpStringKits.find_first_not_of(",", end + 1);
			}
			items.push_back({ tmpPos, tmpQuant, tmpKits});
		}
	}
	inDoc.close();
	auto tryMatch = findMatch(items, kit);
	if (tryMatch.first) {
		cout << endl << "Нбор содержится в документе" << endl 
			<< "Состав набора" << endl
			<< "Поз.    Кол-во    Каталог" << endl;
		for (item i : tryMatch.second) {
			cout << format("{:<8}{:<10}{:<7}", i.pos, i.quant, i.kits[0]) << endl;
		}
	}
	else {
		cout << endl << "Набор не содержится в документе" << endl;
	}
	return 0;
}
