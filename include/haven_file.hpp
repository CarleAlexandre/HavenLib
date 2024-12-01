#ifndef HAVEN_FILE_HPP
# define HAVEN_FILE_HPP

#include <fstream>
#include <sstream>

static char *readFile(const char *filepath) {
	std::ifstream file;
	std::stringstream str;
	char *ret;

	file.open(filepath);
	if (file.is_open()) {
		str << file.rdbuf();
		ret = (char *)malloc(str.str().size() + 1);
		ret[str.str().size()] = 0;
		str.str().copy(ret, str.str().size());
		//need to free returned value after use;
		file.close();
		return (ret);
	}
	return (NULL);
}

static void writeFile(const char *filepath, const char *data, size_t n) {
	std::fstream file(filepath, std::fstream::trunc | std::fstream::out);

	file.write(data, n);
	file.close();
}

#endif
