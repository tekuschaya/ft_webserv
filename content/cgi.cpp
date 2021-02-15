#include <iostream>
#include <unistd.h>

std::string decode(std::string const &str)
{
	std::string out;
	int i ;
	std::string::const_iterator it = str.begin();
	std::string::const_iterator ite = str.end();
	while (it != ite)
	{
		if (*it == '%')
		{
			it++;
			//std::cout << ">>>" << std::strtol(std::string(it, ite).c_str(), 0, 16) << "<<<";
			out += static_cast<char>(std::strtol(std::string(it, it + 2).c_str(), 0, 16));
			//std::cout << "int: " << i << "Char: " << (char)i << std::endl;
			it += 2;
		}
		else if (*it == '+')
		{
			out += ' ';
			it++;
		}
		else
		{
			out += *it;
			it++;
		}
	}
	return out;
}

int main(int argc, char **argv, char **env)
{
	int len = -1;
	{
		std::string tmp;
		std::string key = "CONTENT_LENGTH=";
		while (*env)
		{
			tmp = *env;
			if (tmp.find(key) != std::string::npos)
			{
				std::string content_len = tmp.erase(0, key.size());
				len = stoi(content_len);
				break ;
			}
			env++;
		}
		if (len == -1)
			return EXIT_FAILURE;
	}
	std::string name;
	std::string surname;
	{
		int ret;
		char buf[len];
		std::string str;
		while ((ret = read(0, &buf, len)))
		{
			if (ret == -1)
				return EXIT_FAILURE;
			buf[ret] = 0;
			str += std::string(buf, ret);
			len -= ret;
		}
		// name=123&surname=456+%260
		if (str.substr(0, 5) != "name=")
			return EXIT_FAILURE;
		str.erase(0, 5);
		if (str.find('&') == std::string::npos || std::count(str.begin(), str.end(), '&') != 1)
			return EXIT_FAILURE;
		name = str.substr(0, str.find('&'));
		if (str.find('=') == std::string::npos || std::count(str.begin(), str.end(), '=') != 1)
			return EXIT_FAILURE;
		str.erase(0, str.find('=') + 1);
		name = decode(name);
		surname = decode(str);
	}
	{
		std::string header;
		std::string body;

		body = "<html>\n<head>\n<title>First CGI Program</title>\n</head>\n<body>\n<h2>This is our first CGI program</h2>\n";
		body += "<body>\nYour name is \'<b><big>" ;
		body += name;
		body += "</big></b>\' and it consists of <b><big>";
		body += std::to_string(name.size());
		body += "</big></b> symbols, your surname is \'<b><big>";
		body += surname;
		body += "</b></big>\' and it consists of <b><big>";
		body += std::to_string(surname.size());
		body += "</b></big> symbols and your surname is very beautiful! =)\n</body>\n</html>\n";
		body += "<body>\n<p><img src=\"parrot.gif\" width=\"200\" height=\"200\" alt=\"parrot\"></p>\n</body>\n</html>\n";
		//body += "<body>\n<p><img src=\"https://i.gifer.com/origin/f9/f90fc85cf18e351c565692dcb1c0feeb_w200.gif\" width=\"200\" height=\"200\" alt=\"parrot\"></p>\n</body>\n</html>\n";
		//body += env[0];
		//body += str;

		//body += "!!!";
		//body += str;
		header = "Status: 200 OK\r\nContent-type:text/html;charset=utf-8\r\nContent-Length: ";
		header += std::to_string(body.size());
		header += "\r\n\r\n";

		std::cout << header << body << std::endl;
	}
	return 0;
}
