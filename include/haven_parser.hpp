#ifndef HAVEN_PARSER_HPP
# define HAVEN_PARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <haven_def.h>

typedef struct s_token {
	std::string	key;
	std::string value;
	uint32_t	identifier;
} t_token;

void clearToken(std::vector<t_token> &token_list);
template <typename T>std::vector<t_token> tokenizer(std::string str, const char *delim, size_t delim_size, std::unordered_map<std::string, T> &dictionnary);
void eraseFromString(std::string &str, const char *delim, u32 delim_size);


#endif
