#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>

void get_files_in_dir(const char *dir, int depth, std::vector<std::string> &files);

std::string upper_to_lower(std::string &word);
std::string &cut_space_front_back(std::string &word);


#endif
