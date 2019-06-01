#include "parser.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "util/stringext.h"

std::pair<std::string, std::string> Parser::parseShader(std::ifstream& stream, const std::unordered_map<std::string, int>& properties)
{
	std::unordered_map<std::string, unsigned int> vertexAttribLocations =
	{
		{ "position",	0 },
		{ "normal",		1 },
		{ "colour",		2 },
		{ "texcoord",	3 },
		{ "skin",		4 }
	};

	enum class ShaderType
	{
		None = 0,
		Vertex = 1,
		Fragment = 2
	};


	std::stringstream ss[2];

	ShaderType type = ShaderType::None;

	std::vector<bool> scopeStatementResults;
	bool reading = false;

	// TODO :
	// This is a problem area right now, but I've spent too much
	// time already on this. The "check(statement)" function works
	// perfectly but checking scopes has got the best of me.
	// Only this section below requires rewriting!!!
	//
	// Continue at later date.
	//
	// Today's date : 07-04-2019
	//
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	// Holy shit...
	// I think it works...
	//
	// I don't want to jinx it but it works on the standard shader
	// with the only property that is equal to 1 being "TEX".
	// 
	// The code is ugly as h*ck and can be probably be shortened, but
	// I don't dare touch it right now.
	//
	// Today's date : 13-04-2019

	std::streampos initial = stream.tellg();


	int debug_lineIndex = 0;
	std::string line;
	while (stream.peek() != '\0' && std::getline(stream, line))
	{
		debug_lineIndex++;
		line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

		size_t p;

		p = line.find("//");
		if (p != std::string::npos)
		{
			line = line.substr(0, p);
		}

		p = line.find("#line");
		if (p != std::string::npos)
		{
			switch (type)
			{
			case ShaderType::None:
				type = ShaderType::Vertex;
				reading = true;
				break;
			case ShaderType::Vertex:
				type = ShaderType::Fragment;
				reading = true;
				break;
			}

			line = "#version 330 core";
		}

		p = line.find("#if");
		if (p != std::string::npos)
		{
			bool b = parseShaderStatement(line.substr(p + 4, line.size() - 1), properties);

			if (scopeStatementResults.size() == 0)
			{
				reading = b;
			}
			else
			{
				reading = scopeStatementResults[scopeStatementResults.size() - 1] && b;
			}

			scopeStatementResults.push_back(b);
			continue;
		}

		p = line.find("#elif");
		if (p != std::string::npos)
		{
			if (scopeStatementResults[scopeStatementResults.size() - 1])
			{
				reading = false;
			}
			else
			{
				reading = parseShaderStatement(line.substr(p + 6, line.size() - 1), properties);
			}

			continue;
		}

		p = line.find("#else");
		if (p != std::string::npos)
		{
			bool allPrevious = true;
			for (int i = 0; i < scopeStatementResults.size() - 1; i++)
			{
				allPrevious = scopeStatementResults[i] && allPrevious;
			}

			bool b = allPrevious && !scopeStatementResults[scopeStatementResults.size() - 1];

			reading = b;
			continue;
		}

		p = line.find("#endif");
		if (p != std::string::npos)
		{
			scopeStatementResults.erase(scopeStatementResults.begin() + scopeStatementResults.size() - 1);

			if (scopeStatementResults.size() > 0)
			{
				reading = scopeStatementResults[scopeStatementResults.size() - 1];
			}
			else
			{
				reading = true;
			}

			continue;
		}

		// Replace Krome's proprietary shader language with OpenGL.
		p = line.find("V_IN");
		if (p != std::string::npos)
		{
			int location = -1;
			for (auto& it : vertexAttribLocations)
			{
				p = line.find(it.first);
				if (p != std::string::npos)
				{
					// Replace "V_IN" with "layout(location = x) in" where x is attribute location.
					p = line.find("V_IN");
					line.replace(p, 4, "layout(location = " + std::to_string(it.second) + ") in");

					// No need to search for others.
					break;
				}
			}
		}

		p = line.find("V_OUT");
		if (p != std::string::npos)
		{
			line.replace(p, 5, "out");
		}

		p = line.find("P_IN");
		if (p != std::string::npos)
		{
			line.replace(p, 4, "in");
		}

		p = line.find("SHADOWTEXTURE");
		if (p != std::string::npos)
		{
			line.replace(p, 13, "texture");
		}

		p = line.find("TEXTURE");
		if (p != std::string::npos)
		{
			line.replace(p, 7, "texture");
		}

		p = line.find("P_OUT_COLOR");
		if (p != std::string::npos)
		{
			line.replace(p, 11, "layout(location = 0) out vec4 colour;");
		}

		// We'll replace lowp (low precision) qualifiers cause we aren't using OpenGL ES
		// and they don't compile on anything else. If you're using ES, feel free to
		// experiment with these.
		p = line.find("lowp");
		if (p != std::string::npos)
		{
			line.replace(p, 5, "");
		}

		p = line.find("FINAL_COLOR");
		if (p != std::string::npos)
		{
			line.replace(p, 11, "colour");
		}

		if (reading)
			ss[static_cast<int>(type) - 1] << line << '\n';
	}

	return std::pair<std::string, std::string>(ss[0].str(), ss[1].str());
}

bool Parser::parseShaderStatement(const std::string& statement, const std::unordered_map<std::string, int>& properties)
{
	enum class Operator
	{
		None,

		LessThan,
		LessThanOrEqualTo,
		EqualTo,
		MoreThanOrEqualTo,
		MoreThan,
		NotEqualTo,

		// Only used in statements.
		Or,
		And
	};

	enum class StatementCount
	{
		Single,
		Double
	};

	// Only supports two expressions, cause that's all that's needed.
	Operator statement_operator = Operator::None;
	StatementCount count;
	std::string expressions[2];


	size_t find_or = statement.find("||");
	size_t find_and = statement.find("&&");

	if (find_or != std::string::npos)
	{
		statement_operator = Operator::Or;
		count = StatementCount::Double;
		expressions[0] = statement.substr(0, find_or);
		expressions[1] = statement.substr(find_or + 2, statement.size() - (find_or + 2));

		expressions[0].erase(std::remove(expressions[0].begin(), expressions[0].end(), '('), expressions[0].end());
		expressions[0].erase(std::remove(expressions[0].begin(), expressions[0].end(), ')'), expressions[0].end());
		expressions[0].erase(std::remove(expressions[0].begin(), expressions[0].end(), ' '), expressions[0].end());

		expressions[1].erase(std::remove(expressions[1].begin(), expressions[1].end(), '('), expressions[1].end());
		expressions[1].erase(std::remove(expressions[1].begin(), expressions[1].end(), ')'), expressions[1].end());
		expressions[1].erase(std::remove(expressions[1].begin(), expressions[1].end(), ' '), expressions[1].end());
	}
	else if (find_and != std::string::npos)
	{
		statement_operator = Operator::And;
		count = StatementCount::Double;
		expressions[0] = statement.substr(0, (find_and));
		expressions[1] = statement.substr(find_and + 2, statement.size() - (find_and + 2));

		expressions[0].erase(std::remove(expressions[0].begin(), expressions[0].end(), '('), expressions[0].end());
		expressions[0].erase(std::remove(expressions[0].begin(), expressions[0].end(), ')'), expressions[0].end());
		expressions[0].erase(std::remove(expressions[0].begin(), expressions[0].end(), ' '), expressions[0].end());

		expressions[1].erase(std::remove(expressions[1].begin(), expressions[1].end(), '('), expressions[1].end());
		expressions[1].erase(std::remove(expressions[1].begin(), expressions[1].end(), ')'), expressions[1].end());
		expressions[1].erase(std::remove(expressions[1].begin(), expressions[1].end(), ' '), expressions[1].end());
	}
	// We have to check for this because "#if 0" exists...
	else if (
		statement.find("<") != std::string::npos ||
		statement.find("<=") != std::string::npos ||
		statement.find("==") != std::string::npos ||
		statement.find(">=") != std::string::npos ||
		statement.find(">") != std::string::npos ||
		statement.find("!=") != std::string::npos)
	{
		count = StatementCount::Single;
		expressions[0] = statement;



		expressions[0].erase(std::remove(expressions[0].begin(), expressions[0].end(), '('), expressions[0].end());
		expressions[0].erase(std::remove(expressions[0].begin(), expressions[0].end(), ')'), expressions[0].end());
		expressions[0].erase(std::remove(expressions[0].begin(), expressions[0].end(), ' '), expressions[0].end());
	}

	// We struck a "#if 0" !
	else
	{
		return std::atoi(statement.c_str());
	}

	bool result[2];
	for (unsigned int i = 0; i < ((count == StatementCount::Double) ? 2 : 1); i++)
	{
		int index = i;

		std::string name;
		Operator expression_operator = Operator::None;
		std::string value;

		bool b;


		size_t find_l = expressions[index].find("<");
		size_t find_loe = expressions[index].find("<=");
		size_t find_e = expressions[index].find("==");
		size_t find_moe = expressions[index].find(">="); // Where is Moe anyway...?
		size_t find_m = expressions[index].find(">");
		size_t find_ne = expressions[index].find("!=");

		size_t p = -1;
		size_t operatorSize = 0;

		// Single-character operators
		if (find_l != std::string::npos)
		{
			p = find_l;
			operatorSize = 1;
			expression_operator = Operator::LessThan;
		}
		else if (find_m != std::string::npos)
		{
			p = find_m;
			operatorSize = 1;
			expression_operator = Operator::MoreThan;
		}

		// Double-character operators
		else if (find_loe != std::string::npos)
		{
			p = find_loe;
			operatorSize = 2;
			expression_operator = Operator::LessThanOrEqualTo;
		}
		else if (find_e != std::string::npos)
		{
			p = find_e;
			operatorSize = 2;
			expression_operator = Operator::EqualTo;
		}
		else if (find_moe != std::string::npos)
		{
			p = find_moe;
			operatorSize = 2;
			expression_operator = Operator::MoreThanOrEqualTo;
		}
		else if (find_ne != std::string::npos)
		{
			p = find_ne;
			operatorSize = 2;
			expression_operator = Operator::NotEqualTo;
		}

		name = expressions[index].substr(0, p);
		value = expressions[index].substr(p + (operatorSize), expressions[index].size() - (p + (operatorSize)));

		switch (expression_operator)
		{
		case Operator::LessThan:
		{
			b = (properties.at(name) < std::atoi(value.c_str()));
			break;
		}

		case Operator::LessThanOrEqualTo:
		{
			b = (properties.at(name) <= std::atoi(value.c_str()));
			break;
		}
		case Operator::EqualTo:
		{
			b = (properties.at(name) == std::atoi(value.c_str()));
			break;
		}
		case Operator::MoreThanOrEqualTo:
		{
			b = (properties.at(name) >= std::atoi(value.c_str()));
			break;
		}
		case Operator::MoreThan:
		{
			b = (properties.at(name) > std::atoi(value.c_str()));
			break;
		}
		case Operator::NotEqualTo:
		{
			b = (properties.at(name) != std::atoi(value.c_str()));
			break;
		}
		}

		result[i] = b;
	}

	if (count == StatementCount::Single)
	{
		return result[0];
	}
	else
	{
		switch (statement_operator)
		{
		case Operator::Or:
		{
			return (result[0] || result[1]);
			break;
		}
		case Operator::And:
		{
			return (result[0] && result[1]);
			break;
		}
		}
	}
}
