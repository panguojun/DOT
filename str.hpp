/*
*		�ַ����Ľ���
*/
#include <iostream>
#include <string>

using namespace std;
namespace STR
{
#define NAME		0x01FF
#define NUMBER		0x02FF
#define OPR			0x03FF
#define LGOPR		0x04FF

	static inline bool checkline(char c) {
		return (c == '\n' || c == '\r');
	}
	static inline bool checkspace(char c) {
		return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
	}
	static inline bool checkspace2(char c) {
		return (c == ' ' || c == '\t');
	}
	static inline bool iscalc(char c) {
		return c == '+' || c == '-' || c == '*' || c == '/' || c == '.';
	}
	static inline bool islogic(char c) {
		return c == '>' || c == '<' || c == '=' || c == '&' || c == '|' || c == '^' || c == '!';
	}
	static inline bool isname(char c) {
		return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
	}
	static inline bool isnum(char c) {
		return c >= '0' && c <= '9';
	}
	static inline bool isbracket(char c) {
		return c == '(';
	}

	struct code_t
	{
		const char* ptr;

		code_t() {}
		code_t(const char* buf) {
			ptr = buf;
		}
		char next() {
			while (!eoc(++ptr) && checkspace(*(ptr)));
			return (*ptr);
		}
		char nextline() {
			while (!eoc(++ptr) && !checkline(*(ptr)));
			return (*++ptr);
		}
		char getnext() {
			const char* p = ptr;
			while (!eoc(++p) && checkspace(*(p)));
			return (*p);
		}
		bool eoc(const char* p = 0) {
			p == 0 ? p = ptr : 0;
			return (*p) == '\0';
		}
		char cur() {
			return *ptr;
		}
		const char* get_last_seg(char dem)
		{
			const char* p = ptr + strlen(ptr);
			while (ptr != p) {
				if (*p == dem) {
					return (p + 1);
				}
				p--;
			}
			return 0;
		}
		const char* get_seg(char dem, int index)
		{
			const char* p = ptr;
			int cnt = 0;
			while (*p != '\n') {
				if (*p == dem) {
					if ((cnt++) == index)
						return (p + 1);
				}
				p++;
			}
			return 0;
		}
	};

	// get char
	short get(code_t& cd)
	{
		for (; !cd.eoc(); cd.next()) {
			char c = cd.cur();
			if (checkspace(c))
				continue;
			else if (isdigit(c)) {
				return NUMBER;
			}
			else if (iscalc(c)) {
				return OPR;
			}
			else if (islogic(c)) {
				return LGOPR;
			}
			else if (isname(c)) {
				return NAME;
			}
			else
				return c;
		}
		return 0;
	}
	string getname(code_t& cd)
	{
		// todo
		return "";
	}
	inline string parse_filename(const string& fullpath)
	{
		code_t pr(fullpath.c_str());
		const char* p = pr.get_last_seg('/');
		if (!p) p = pr.get_last_seg('\\');

		return p;
	}
	// Split a String into a string array at a given token
	inline void split(const std::string& in,
		std::vector<std::string>& out,
		std::string token)
	{
		out.clear();

		std::string temp;

		for (int i = 0; i < int(in.size()); i++)
		{
			std::string test = in.substr(i, token.size());

			if (test == token)
			{
				if (!temp.empty())
				{
					out.push_back(temp);
					temp.clear();
					i += (int)token.size() - 1;
				}
				else
				{
					out.push_back("");
				}
			}
			else if (i + token.size() >= in.size())
			{
				temp += in.substr(i, token.size());
				out.push_back(temp);
				break;
			}
			else
			{
				temp += in[i];
			}
		}
	}

	// Get tail of string after first token and possibly following spaces
	inline std::string tail(const std::string& in)
	{
		size_t token_start = in.find_first_not_of(" \t");
		size_t space_start = in.find_first_of(" \t", token_start);
		size_t tail_start = in.find_first_not_of(" \t", space_start);
		size_t tail_end = in.find_last_not_of(" \t");
		if (tail_start != std::string::npos && tail_end != std::string::npos)
		{
			return in.substr(tail_start, tail_end - tail_start + 1);
		}
		else if (tail_start != std::string::npos)
		{
			return in.substr(tail_start);
		}
		return "";
	}

	// Get first token of string
	inline std::string firstToken(const std::string& in)
	{
		if (!in.empty())
		{
			size_t token_start = in.find_first_not_of(" \t");
			size_t token_end = in.find_first_of(" \t", token_start);
			if (token_start != std::string::npos && token_end != std::string::npos)
			{
				return in.substr(token_start, token_end - token_start);
			}
			else if (token_start != std::string::npos)
			{
				return in.substr(token_start);
			}
		}
		return "";
	}
}