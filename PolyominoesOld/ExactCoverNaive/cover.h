#ifndef COVER_H
#define COVER_H

#include <vector>
#include <iostream>
#include <set>
#include <climits>

void cover_disp(std::set<int>& s);
void cover_vdisp(std::vector<int>& v);
void cover_matdisp(std::vector<std::vector<bool>>& mat, std::set<int>& rows, std::set<int>& cols);

// matrix / eliminated rows / eliminated cols / solution set (for external use)
bool cover_search(std::vector<std::vector<bool>>& mat, std::set<int>& rows, std::set<int>& cols, std::set<int>& solution);
#endif
