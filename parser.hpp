namespace RNA
{
	std::map<string, string> mapdat;
	void bracket_big(STR::code_t& cd)
	{
		while (char c = cd.next())
		{
			if (c == '}') {
				cd.next();
				return;
			}
			if(STR::get(cd) == NAME)
			{
				STR::getname(cd);
			}
		}
	}
	void parser(crstr code)
	{
		STR::code_t cd;
		cd.ptr = code.c_str();

		while (char c = cd.next())
		{
			if (c == '`')
			{
				cd.nextline();
				continue;
			}
			else if (c == '{')
			{
				bracket_big(cd);
			}

			PRINTV(c)
		}
	}
	void parser_file(crstr filename)
	{
		std::fstream f(filename);
		std::stringstream buf;
		buf << f.rdbuf();
		parser(buf.str());
	}
}