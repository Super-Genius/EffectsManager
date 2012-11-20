#include "IEffectManager.h"
#include "SourceCode.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

namespace GLSLEFX
{

SourceCode::SourceCode(const std::string& source)
{
	//int line_number = 1;
	m_SourceCode = source.c_str();
}

SourceCode::~SourceCode()
{

}

std::string &SourceCode::LoadSource(const std::string& source, int &linen)
{
	// get substr, of possibly file:, http:, https:, zip:
	std::string sourceType = source.substr(0, source.find_first_of(':'));

	std::istream *inputStream;
	std::ifstream file;

	std::string filename_path = source.substr( 0, source.find_last_of( '/' ) +1 );
	std::string filename_file = source.substr( source.find_last_of( '/' ) +1 );
	
	if (!sourceType.compare("http") || !(sourceType.compare("https")))
	{
	}
	else if (sourceType.compare("zip"))
	{
	}
	else		// assume it is a file name
	{
		file.open(source.data(), std::ios::in);

		if (!file)
		{
			std::cerr << "Could not open file " << source << std::endl;
		}

		inputStream = &file;
	}

	m_FileNames.push_back(filename_path);
	LoadSource(filename_path, source, *inputStream, linen);
	
	file.close();  //

	return m_SourceCode;

}

std::string  &SourceCode::LoadSource(std::string &filename_path, const std::string &sourceName, std::istream& sourceStream, int &linen)
{
	int current_line = 1;
	lines.push_back(linen);
	locallines.push_back(current_line);

	while(!sourceStream.eof())
	{
		std::string line;
		getline(sourceStream, line);
		int pos = line.find("#include");
		if(pos >= 0)
		{
			int pos_begin = line.find("\"", pos) + 1;
			int pos_end = line.find("\"", pos_begin) - 1;
			if (pos_begin >= pos_end)
			{
				std::cout << "Error in file "<< sourceName << " at line " << current_line << " : include failed." << std::endl;
				return m_SourceCode;
			}

			std::string include_name = line.substr(pos_begin, pos_end-pos_begin+1);
			m_SourceCode += LoadSource(filename_path+include_name, linen);
			lines.push_back(linen);
			current_line++;
			locallines.push_back(current_line);
		}
		else
		{
			m_SourceCode += line;
			m_SourceCode += "\n";
			linen++;
			current_line++;
		}
	}

	return m_SourceCode;
}

char * SourceCode::GetCode()
{
	char * code_char = new char[m_SourceCode.size()+1];
	for(unsigned int i=0 ; i<m_SourceCode.size() ; ++i)
		code_char[i] = m_SourceCode.data()[i];
	code_char[m_SourceCode.size()] = '\0';

    return code_char;
}

void SourceCode::displayOpenglLog(const std::string& openGLlog)
{
	std::cout << "GLSL ERROR LOG" << std::endl;

	std::istringstream log(openGLlog, std::istringstream::in);

	while(!log.eof())
	{
		std::string linelog;
		std::getline(log, linelog);

		int pos_begin = linelog.find("0(", 0) + 2;
		int pos_end = linelog.find(") : error", pos_begin) - 1;

		if(pos_end > pos_begin)
		{
			char * line_number_char = new char[pos_end-pos_begin+2];
			for(int i=pos_begin ; i <= pos_end ; ++i)
				line_number_char[i-pos_begin] = linelog.data()[i];
			line_number_char[pos_end-pos_begin+1] = '\0';
			int line_number = atoi(line_number_char);

			int v = 0;

			for(unsigned int i=0 ; i < lines.size() ; ++i)
			{
				// on the last one
				if( i == lines.size()-1 )
				{
					v = i;
					break;
				}
				
				if(line_number < lines[i+1])
				{
					v = i;
					break;
				}
			}

			std::cout << linelog << std::endl;
			std::cout << "File " << m_FileNames[v] << " line " << line_number-lines[v]+locallines[v] << std::endl;
			line_number = line_number;
		}
		else
		{
			std::cout << linelog << std::endl;
		}
	}
}

}
