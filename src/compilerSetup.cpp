#include "compilerSetup.hpp"

amanises::CompilerSetup::CompilerSetup(Logger* logger, Lexer* lexer) :
	m_logger(logger),
	m_lexer(lexer)
{
}

bool amanises::CompilerSetup::process_file_for_lexer(const char* src_path, std::string& src_name, std::vector<Token>& src_tok_list)
{
	m_logger->log(log_type::INFO, "Lexing process started for " + std::string(src_path));

	// verify
	if (!verify_opening_file_path(src_path))
	{
		m_logger->log(log_type::ERROR, std::string("File path verification failed for: ") + src_path);
		return false;
	}

	// read file
	std::string src = read_file(src_path);
	if (src.empty())
	{
		m_logger->log(log_type::ERROR, std::string("Failed to read or empty file: ") + src_path);
		return false;
	}

	// get file name
	src_name = get_file_name(src_path);
	if (src_name.empty())
	{
		m_logger->log(log_type::ERROR, "Unable to determine file name for: " + std::string(src_path));
		return false;
	}

	// tokenize file
	src_tok_list = m_lexer->tokenize_source_file(std::move(src));
	if (src_tok_list.empty()) {
		m_logger->log(log_type::ERROR, src_name + " has an empty token list after tokenization");
		return false;
	}

	m_logger->log(log_type::INFO, "Lexing process finished for " + std::string(src_path));

	return true;
}

bool amanises::CompilerSetup::process_tokens_for_parser()
{
	return true;
}

std::string amanises::CompilerSetup::read_file(const std::string& file_path)
{
	std::fstream input(file_path, std::ios::in);
	if (!input)
	{
		std::cerr << "Error: Unable to open file " << file_path << std::endl;
	}

	std::stringstream c_stream;
	c_stream << input.rdbuf();
	input.close();

	return c_stream.str();
}

bool amanises::CompilerSetup::verify_opening_file_path(const char* file_path)
{
	std::fstream file(file_path);
	if (!file.is_open())
	{
		return false;
	}
	return true;
}

std::string amanises::CompilerSetup::get_file_name(const std::string& file_path)
{
	return file_path.substr(file_path.find_last_of("//\\") + 1);
}
