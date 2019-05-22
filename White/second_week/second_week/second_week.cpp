#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>


using namespace std;


int factorial(int num)
{
	if (num <= 1) {
		return 1;
	}
	else
	{
		return num * factorial(num - 1);
	}
}


bool IsPalindrom(string str)
{
	int n = str.size() / 2;

	bool flag = true;

	for (int i = 0; i <= n; ++i) {
		flag *= (str[i] == str[(str.size() - 1) - i]);
	}

	return flag;
}


vector<string> PalindromFilter(vector<string> words, int min_length)
{
	vector<string> out_vector;

	for (auto w : words) {
		int n = w.size() / 2;

		bool flag = true;

		for (int i = 0; i <= n; ++i) {
			flag *= (w[i] == w[(w.size() - 1) - i]);
		}

		if (flag && w.size() >= min_length) {
			out_vector.push_back(w);
		}
	}

	return out_vector;
}


void update_if_grater(int first, int& second)
{
	if (first > second) {
		second = first;
	}
}


void MoveStrings(vector<string>& source, vector<string>& destination)
{
	for (auto w : source) {
		destination.push_back(w);
	}

	source.clear();
}


void Reverse(vector<int>& num_vec)
{
	vector<int> tmp_vec;

	for (int i = num_vec.size() - 1; i >= 0; --i) {
		tmp_vec.push_back(num_vec[i]);
	}

	num_vec = tmp_vec;
}


vector<int> reversed(const vector<int>& num_vec)
{
	vector<int> tmp_vec;

	for (int i = num_vec.size() - 1; i >= 0; --i) {
		tmp_vec.push_back(num_vec[i]);
	}

	return tmp_vec;
}


void get_over_middle()
{
	int n;

	cin >> n;

	vector<int> nums(n);

	int sum = 0;

	for (int& num : nums) {
		cin >> num;
		sum += num;
	}

	int mid = sum / n;

	int k = 0;

	string inds = "";

	int i = 0;

	for (auto num : nums) {
		if (num > mid) {
			inds += to_string(i) + " ";

			k += 1;
		}
		++i;
	}

	if (inds.size() != 0) {
		inds.pop_back();
	}

	cout << k << endl;
	cout << inds;
}


void queue()
{
	struct Person
	{
		bool worry;
		bool quiet;
	};

	int q;

	cin >> q;

	vector<Person> queue_vec;

	for (int i = 0; i < q; i++) {
		
		string command;

		cin >> command;

		int com_num;

		if (command == "WORRY") {
			cin >> com_num;

			queue_vec[com_num].worry = true;
		}

		if (command == "QUIET") {
			cin >> com_num;

			queue_vec[com_num].quiet = true;
			queue_vec[com_num].worry = false;
		}

		if (command == "COME") {
			cin >> com_num;

			if (com_num > 0) {
				for (int k = 0; k < com_num; k++) {
					queue_vec.push_back(Person{ false, true });
				}
			}
			
			if (com_num < 0) {
				for (int k = com_num; k < 0; k++) {
					queue_vec.pop_back();
				}
			}
		}

		if (command == "WORRY_COUNT") {

			int worry_count = 0;

			for (const auto& person : queue_vec) {
				if (person.worry) {
					worry_count += 1;
				}
			}

			cout << worry_count << endl;
		}
	}
}


void monthly_affairs()
{
	struct Month
	{
		int days_count;
		vector<vector<string>> affairs;
	};

	vector<Month> months = { {31, vector<vector<string>>(31)},  {28, vector<vector<string>>(28)}, {31, vector<vector<string>>(31)}, 
	{30, vector<vector<string>>(30)}, {31, vector<vector<string>>(31)}, {30, vector<vector<string>>(30)}, {31, vector<vector<string>>(31)},
	{31, vector<vector<string>>(31)}, {30, vector<vector<string>>(30)}, {31, vector<vector<string>>(31)}, {30, vector<vector<string>>(30)}, 
	{31, vector<vector<string>>(31)} };

	int current_month = 0;

	string command;
	int day;
	string affair;

	int q;

	cin >> q;

	for (int i = 0; i < q; i++) {
		cin >> command;

		if (command == "ADD") {
			cin >> day;
			cin >> affair;

			months[current_month].affairs[day - 1].push_back(affair);
		}

		if (command == "NEXT") {
			int tmp_month_num = current_month;

			current_month = (current_month + 1) % 12;

			if (months[current_month].days_count >= months[tmp_month_num].days_count) {
				for (int j = 0; j < months[tmp_month_num].days_count; j++) {
					months[current_month].affairs[j] = months[tmp_month_num].affairs[j];
				}

				months[tmp_month_num].affairs = vector<vector<string>>(months[tmp_month_num].days_count);
			}
			else {
				for (int j = 0; j < months[current_month].days_count; j++) {
					months[current_month].affairs[j] = months[tmp_month_num].affairs[j];
				}

				for (int j = months[current_month].days_count; j < months[tmp_month_num].days_count; j++) {
					months[current_month].affairs[months[current_month].days_count - 1].insert(end(months[current_month].affairs[months[current_month].days_count - 1]),
						begin(months[tmp_month_num].affairs[j]), end(months[tmp_month_num].affairs[j]));
				}

				months[tmp_month_num].affairs = vector<vector<string>>(months[tmp_month_num].days_count);
			}
		}

		if (command == "DUMP") {
			cin >> day;

			cout << months[current_month].affairs[day - 1].size() << " ";

			for (const auto& affair : months[current_month].affairs[day - 1]) {
				cout << affair << " ";
			}

			cout << endl;
		}
	}
}


void check_anagramms()
{
	int n;

	cin >> n;

	struct Words
	{
		string first;
		string second;
	};

	vector<Words> words_couples(n);

	string answer_str = "";

	for (Words& words : words_couples) {
		cin >> words.first >> words.second;

		map<char, int> first_word, second_word;

		for (const char& letter : words.first) {
			first_word[letter] += 1;
		}

		for (const char& letter : words.second) {
			second_word[letter] += 1;
		}

		if (first_word == second_word) {
			answer_str += "YES\n";
		}
		else {
			answer_str += "NO\n";
		}
	}

	cout << answer_str;
}


void countries_capitals()
{
	map<string, string> capitals_map;

	int q;

	cin >> q;

	for (int i = 0; i < q; i++) {
		string command;

		cin >> command;

		if (command == "CHANGE_CAPITAL") {
			string country;
			string new_capital;

			cin >> country >> new_capital;

			if (capitals_map.count(country) == 0) {
				capitals_map[country] = new_capital;

				cout << "Introduce new country " << country << " with capital " << new_capital << endl;
			}
			else
			{
				if (capitals_map[country] == new_capital) {
					cout << "Country " << country << " hasn't changed its capital" << endl;
				}
				else {
					string old_capital = capitals_map[country];
					capitals_map[country] = new_capital;

					cout << "Country " << country << " has changed its capital from " << old_capital << " to " << new_capital << endl;
				}
			}
		}

		if (command == "RENAME") {
			string old_country_name, new_country_name;

			cin >> old_country_name >> new_country_name;

			if (capitals_map.count(old_country_name) == 0 || capitals_map.count(new_country_name) == 1 || old_country_name == new_country_name) {
				cout << "Incorrect rename, skip" << endl;
			}
			else {
				string tmp_capital = capitals_map[old_country_name];

				capitals_map.erase(old_country_name);
				capitals_map[new_country_name] = tmp_capital;

				cout << "Country " << old_country_name << " with capital " << tmp_capital << " has been renamed to " << new_country_name << endl;
			}
		}

		if (command == "ABOUT") {
			string country;

			cin >> country;

			if (capitals_map.count(country) == 0) {
				cout << "Country " << country << " doesn't exist" << endl;
			}
			else {
				cout << "Country " << country << " has capital " << capitals_map[country] << endl;
			}
		}

		if (command == "DUMP") {
			if (capitals_map.size() == 0) {
				cout << "There are no countries in the world" << endl;
			}
			else {
				for (const auto& couple : capitals_map) {
					cout << couple.first << "/" << couple.second << " ";
				}
				cout << endl;
			}
		}
	}
}


void bus_stations()
{
	map<string, vector<string>> bus_dict;

	vector<string> busses;

	int q;

	cin >> q;

	for (int i = 0; i < q; i++) {
		string command;

		cin >> command;

		if (command == "NEW_BUS") {
			string bus;
			int stop_count;

			cin >> bus >> stop_count;

			busses.push_back(bus);
			bus_dict[bus].resize(stop_count);

			for (string& stop : bus_dict[bus]) {
				cin >> stop;
			}
		}

		if (command == "BUSES_FOR_STOP") {
			string stop;

			cin >> stop;

			bool search_flag = false;
			string out_str = "";

			for (const auto& bus : busses) {
				if (count(bus_dict[bus].begin(), bus_dict[bus].end(), stop) != 0) {
					search_flag += true;
					out_str += bus + " ";
				}
			}

			if (search_flag) {
				cout << out_str << endl;
			}
			else {
				cout << "No stop" << endl;
			}
		}

		if (command == "STOPS_FOR_BUS") {
			string bus;

			cin >> bus;

			if (count(busses.begin(), busses.end(), bus) != 0) {
				for (const auto& stop : bus_dict[bus]) {
					bool search_flag = false;
					string out_str = "";

					for (const auto& other_bus : busses) {
						if (count(bus_dict[other_bus].begin(), bus_dict[other_bus].end(), stop) != 0 && other_bus != bus) {
							search_flag += true;
							out_str += other_bus + " ";
						}
					}

					if (search_flag) {
						cout << "Stop " << stop << ": " << out_str << endl;
					}
					else
					{
						cout << "Stop " << stop << ": no interchange" << endl;
					}
				}
			}
			else {
				cout << "No bus" << endl;
			}
		}

		if (command == "ALL_BUSES") {
			if (bus_dict.size() != 0) {
				for (const auto& bus : bus_dict) {
					string out_str = "";

					for (const auto& stop : bus_dict[bus.first]) {
						out_str += stop + " ";
					}

					cout << "Bus " << bus.first << ": " << out_str << endl;
				}
			}
			else {
				cout << "No buses" << endl;
			}
		}
	}
}


void bus_stations2()
{
	map<vector<string>, int> stations_dict;

	int q;

	cin >> q;

	for (int i = 0; i < q; i++) {
		int num;

		cin >> num;

		vector<string> tmp_vec(num);

		for (auto& station : tmp_vec) {
			cin >> station;
		}

		if (stations_dict.count(tmp_vec) != 0) {
			cout << "Already exists for " << stations_dict[tmp_vec] << endl;
		}
		else {
			int tmp_val = stations_dict.size() + 1;
			stations_dict[tmp_vec] = tmp_val;

			cout << "New bus " << tmp_val << endl;
		}
	}
}


void uniq_strings()
{
	int n;

	cin >> n;

	set<string> string_set;

	for (int i = 0; i < n; i++) {
		string tmp_str;

		cin >> tmp_str;

		string_set.insert(tmp_str);
	}

	cout << string_set.size();
}


set<string> BuildMapValuesSet(const map<int, string>& m) {
	set<string> out_set;

	for (const auto& el : m) {
		out_set.insert(el.second);
	}

	return out_set;
}


void synonyms()
{
	unordered_map<string, set<string>> synonyms_dict;

	int q;

	cin >> q;

	for (int i = 0; i < q; i++) {
		string command;

		cin >> command;

		if (command == "ADD") {
			string word1, word2;

			cin >> word1 >> word2;

			synonyms_dict[word1].insert(word2);
			synonyms_dict[word2].insert(word1);
		}

		if (command == "COUNT") {
			string word;

			cin >> word;

			try
			{
				cout << synonyms_dict.at(word).size() << endl;
			}
			catch (out_of_range ex)
			{
				cout << 0 << endl;
			}
		}

		if (command == "CHECK") {
			string word1, word2;

			cin >> word1 >> word2;

			if (synonyms_dict[word1].find(word2) != synonyms_dict[word1].end() || synonyms_dict[word2].find(word1) != synonyms_dict[word2].end()) {

				cout << "YES" << endl;
			}
			else {
				cout << "NO" << endl;
			}
		}
	}
}


void bus_stations3()
{
	map<set<string>, int> stations_dict;

	int q;

	cin >> q;

	for (int i = 0; i < q; i++) {
		int num;

		cin >> num;

		set<string> tmp_set;

		for (int j = 0; j < num; j++) {
			string tmp_str;

			cin >> tmp_str;

			tmp_set.insert(tmp_str);
		}

		if (stations_dict.count(tmp_set) != 0) {
			cout << "Already exists for " << stations_dict[tmp_set] << endl;
		}
		else {
			int tmp_val = stations_dict.size() + 1;
			stations_dict[tmp_set] = tmp_val;

			cout << "New bus " << tmp_val << endl;
		}
	}
}


void guess_number()
{
	int left = 1;
	int right = 1000;

	int number = (right + left) / 2;

	string command = "";

	while (command != "YES")
	{
		cout << number << endl;

		cin >> command;

		if (command == "LARGER") {
			left = number;

			number = (right + left) / 2;
		}

		if (command == "LESS") {
			right = number;

			number = (right + left) / 2;
		}
	}
}


int main()
{
	bus_stations3();

	return 0;
}