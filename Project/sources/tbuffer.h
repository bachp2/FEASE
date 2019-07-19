#pragma once
#include <vector>
#include <string>

struct line {
	friend class TBuffer;
private:
	std::string write;
public:
	line(std::string str) {
		write.reserve(str.size());
		write.append(str);
	}
	
	int insert(const std::string& val, int at) {
		if (at >= write.size()) return -1;
		/*scratch += val;
		scri = at;*/
		return 0;
	}
	
	void push_back(std::string str) {
		write += str;
	}

	void push_back(char c) {
		write += c;
	}

	//line& operator+=(const char& rhs) {
	//	write += rhs;
	//	return *this;
	//}

	int size() {
		return write.size();
	}
	
	std::string str() { 
		printf("%s", write.c_str());
		return write; }

	std::string trim(int at) {
		std::string trimmed;
		for(int size = write.size() - 1, i = size; i >= 0 ; --i)
		{
			trimmed += write[i];
			if (i == at) break;
		}
		return trimmed;
	}
};

class TBuffer {
	
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
		/*rows.emplace_back("asdasdasd");
		printf("rows %s", rows[0].str().c_str());*/
	}

	TBuffer(std::string text) {
		auto vlines = SplitString(text, '\n');
		rows.reserve(vlines.size());
		for (auto& l : vlines) {
			rows.emplace_back(l);
		}
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
		auto vlines = SplitString(str, '\n');
		if (at_row >= rows.size() || at_col >= rows[at_row].size()) {
			return -1;
		}

		if (at_col == rows[at_row].size() - 1)
		{
			this->at(at_row).push_back(vlines.front());
			for (auto i = 1; i < vlines.size() - 1; ++i)
			{
				// can be optimized with better data structure
				this->insert(line(vlines[i]), at_row++);
			}

			/*this->insert_line(line(vlines[i]), at_row++);*/
			this->at(at_row).push_back(vlines.back());
		}
		else
		{
			auto trimmed = this->at(at_row).trim(at_col);
			this->at(at_row).push_back(vlines.front());
			for (auto i = 1; i < vlines.size() - 1; ++i)
			{
				// can be optimized with better data structure
				this->insert(line(vlines[i]), at_row++);
			}
			this->at(at_row).push_back(vlines.back().append(trimmed));
		}
		return 0;
	}

	void append(int& c) {
		printf("asdasd");
		if (rows.empty()) {
			rows.emplace_back("");
		}
		auto last = rows.back();
		last.push_back(c);
	}

	void append(char& c) {
		if (rows.empty()) {
			rows.emplace_back("");
		}
		auto last = rows.back();
		last.push_back(c);
	}

	line& at(const int r) {
		return rows[r];
	}

	/*void append(std::string& s)
	{
		rows.emplace_back(s);
	}*/

	std::vector<line>& container() {
		return rows;
	}
};