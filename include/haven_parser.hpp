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
void eraseFromString(std::string &str, const char *delim, u32 delim_size);
const char *getNextDelim(const char *str, const char *delim, size_t str_size, size_t delim_size);
const char *stringSpliter(const char *str, const char *delim, size_t str_size, u32 delim_size);

template <typename T>
static std::vector<t_token> tokenizer(std::string str, const char *delim, size_t delim_size, std::unordered_map<std::string, T> &dictionnary) {
	std::vector<t_token> token_list;
	t_token tok;
	const char *span;
	const char *data = str.c_str();

	for (size_t i = 0; i < str.size(); i++) {
		span = getNextDelim(data + i, delim, str.size() - i, delim_size);
		if (!span) {
			break;
		}
		if (span == delim) {
			token_list.clear();
			break;
		}
		const char *tmp = stringSpliter(data + i, ":", span - data + i, 1);
		for (int k = i; data + k < tmp; k++) {
			tok.key += *(data + k);
			i++;
		}
		if (data[i] == ':')
			i++;
		for (int k = i; data + k < span; k++){
			tok.value += *(data + k);
			i++;
		}
		tok.identifier = dictionnary[tok.key];
		if (!tok.value.empty()) {
			token_list.push_back(tok);
			tok.value.clear();
			tok.key.clear();
			tok.identifier = 0;
		}
		i++;
	}
	return (token_list);
}

#endif
