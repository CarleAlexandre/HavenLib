#include <string>
#include <vector>
#include "../include/haven_def.h"

typedef enum {
	parse_context_none			= 0,
	parse_context_bracket		= 1,
	parse_context_simple_quote	= 2,
	parse_context_double_quote	= 3,
	parse_context_error			= 4,
} parse_context_e;

typedef struct s_token {
	std::string	key;
	std::string value;
	uint32_t	identifier;
} t_token;


void getParseContext(const char data, parse_context_e *context) {
	switch (data){
		case('{'): {
			if (*context == parse_context_none) {
				*context = parse_context_bracket;
			}
			//else error
		}
		break;
		case('}'): {
			if (*context == parse_context_bracket) {
				*context = parse_context_none;
			}
			//else error
		}
		break;
		case('\''): {
			if (*context == parse_context_none) {
				*context = parse_context_simple_quote;
			} else if (*context == parse_context_simple_quote) {
				*context = parse_context_none;
			}
			//else error
		}
		break;
		case('\"'): {
			if (*context == parse_context_none) {
				*context = parse_context_double_quote;
			} else if (*context == parse_context_double_quote) {
				*context = parse_context_none;
			}
			//else error
		}
		break;
		//case(): {
		//}
		//break;
		default:
			break;
	};
}

const char *getNextDelim(const char *str, const char *delim, size_t str_size, size_t delim_size) {
	parse_context_e context = parse_context_none;
	for (u32 i = 0; i < str_size; i++) {
		getParseContext(str[i], &context);
		if (context == parse_context_error) {
			return (delim);
		}
		if (context == parse_context_none) {
			for (u32 k = 0; k < delim_size; k++) {
				if (str[i] == delim[k]) {
					return (str + i);
				}
			}
		}
	}
	return (0x00);
}

const char *stringSpliter(const char *str, const char *delim, size_t str_size, u32 delim_size) {
	const char *iterator = 0x00;

	iterator = str;
	while (iterator - str < str_size) {
		for (int i = 0; i < delim_size; i++) {
			if (*iterator == delim[i]) {
				return (iterator);
			}
		}
		iterator++;
	}
	iterator = 0x00;
	return (iterator);
}

void eraseFromString(std::string &str, const char *delim, u32 delim_size) {
	for (int i = 0; i < str.size(); i++) {
		for (int k = 0; k < delim_size; k++) {
			if (str[i] == delim[k]) {
				str.erase(str.begin() + i);
			}
		}
	}
}

void clearToken(std::vector<t_token> &token_list) {
	for (size_t i = 0; i < token_list.size(); i++) {
		token_list[i].key.clear();
		token_list[i].value.clear();
	}
	token_list.clear();
}
