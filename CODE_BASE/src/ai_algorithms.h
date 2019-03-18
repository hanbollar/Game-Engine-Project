#pragma once

#include "globals.h"

#include <algorithm>
#include <map>

template<class T>
void AStar(
	T& start,
	T& goal,
	unsigned int& heuristic_option,
	vector<T>(*FindNeighbors)(T& current_value),
	vector<T>* path);

