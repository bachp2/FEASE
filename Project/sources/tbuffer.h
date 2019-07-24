#pragma once
#include <vector>
#include <string>

class TBuffer {
	typedef std::string line;
	std::vector<std::string> SplitString(const std::string& s, char delim)
	{
		std::vector<std::string> elems;

		const char* cstr = s.c_str();
		unsigned int strLength = s.length();
		unsigned int start = 0;
		unsigned int end = 0;

		while (end <= strLength)
		{
			while (end <= strLength)
			{
				if (cstr[end] == delim)
					break;
				end++;
			}

			elems.push_back(s.substr(start, end - start));
			start = end + 1;
			end = start;
		}
		return elems;
	}

public:
	std::vector<line> rows;
	std::vector<line> scratch;
	TBuffer() {
		rows.reserve(10);
	}

	TBuffer(std::string text) {
		auto vlines = SplitString(text, '\n');
		rows.reserve(vlines.size());
		for (auto& l : vlines) {
			rows.emplace_back(l);
		}
	}

	void pop_char() {
		if (rows.empty()) return;
		if (rows.back().empty()) rows.pop_back();
		rows.back().pop_back();
	}

	bool empty() {
		return rows.empty() || rows.back() == "";
	}

	int insert(line& l, int at) {
		if (at >= rows.size() || at < 0) return -1;
		rows.insert(rows.begin() + at, l);
		return 0;
	}

	int insert(std::string str, int at_row, int at_col) {
		/*bool nl_front{false}, nl_back{ false };
		if (str.front() == '\n') nl_front = true;
		if (str.back() == '\n') nl_back = true;*/
		//auto vlines = SplitString(str, '\n');
		//if (at_row >= rows.size() || at_col >= rows[at_row].size()) {
		//	return -1;
		//}

		//if (at_col == rows[at_row].size() - 1)
		//{
		//	this->at(at_row).push_back(vlines.front());
		//	for (auto i = 1; i < vlines.size() - 1; ++i)
		//	{
		//		// can be optimized with better data structure
		//		this->insert(line(vlines[i]), at_row++);
		//	}

		//	/*this->insert_line(line(vlines[i]), at_row++);*/
		//	this->at(at_row).push_back(vlines.back());
		//}
		//else
		//{
		//	auto trimmed = this->at(at_row).trim(at_col);
		//	this->at(at_row).push_back(vlines.front());
		//	for (auto i = 1; i < vlines.size() - 1; ++i)
		//	{
		//		// can be optimized with better data structure
		//		this->insert(line(vlines[i]), at_row++);
		//	}
		//	this->at(at_row).push_back(vlines.back().append(trimmed));
		//}
		//return 0;
	}

	void append(unsigned int& c) {
		if (rows.empty()) {
			rows.push_back("");
		}
		if (c == '\n') {
			rows.push_back("");
			return;
		}
		rows.back() += c;
	}


	line& at(const int r) {
		return rows[r];
	}

	void append(std::string& s)
	{
		if (rows.empty()) {
			rows.push_back("");
		}
		rows.back() += s;
	}

	std::string str() {
		std::string mstr = "";
		for (auto i = 0; i < rows.size(); ++i) {
			mstr += rows[i];
			mstr += "\n";
		}
		return mstr;
	}
};