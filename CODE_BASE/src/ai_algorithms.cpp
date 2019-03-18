#include "ai_algorithms.h"

/******************/
/***** A Star *****/
/******************/

template<class T>
float AStar_HeuristicCostEstimate(	T start,
									T goal,
									unsigned int heuristic_option ) {
	glm::vec3 fabs_diff = *start - *goal;
	fabs_diff[0] = std::fabs(fabs_diff[0]);
	fabs_diff[1] = std::fabs(fabs_diff[1]);
	fabs_diff[2] = std::fabs(fabs_diff[2]);

	switch (heuristic_option) {
	case 1:
		// Manhattan Distance - only in four directions [on 2d board] (right, left, top, bottom)
		return fabs_diff[0] + fabs_diff[1] + fabs_diff[2];
	case 2:
		// Diagonal Distance - only in eight directions [on 2d board] (similar to a move of a King in Chess)
		return glm::max(fabs_diff[0], glm::max(fabs_diff[1], fabs_diff[2]));
	case 3:
		// Euclidean Distance - only in any direction [on 2d board]
		return glm::distance(*start, *goal);
	default:
		ErrorHandler::showError("Error in AStar_HeuristicCostEstimate. Inputted invalid Heuristic option.");
		throw;
	}
}

template<class T>
void AStar( T& start,
		    T& goal,
		    unsigned int& heuristic_option,
		    std::vector<T> (*FindNeighbors)(T& current_value),
			std::vector<T>* path) { // TODO: SWITCH BACK TO USING POINTERS OR SHARED POINTERS???
	// TODO: test this
	// TODO: switch to using std::shared_ptr<T> instead of just T.

	std::vector<T> closed_set = {};
	std::vector<T> open_set = { start };
	std::map<T, T> came_from = {};

	std::map<T, float> g_score = {};
	g_score[start_index] = 0.f;
	std::map<T, float> f_score = {};
	f_score[start] = AStar_HeuristicCostEstimate(start, goal, heuristic_option);

	T val_with_min_f_score = start;
	float min_f_score = f_score[start];

	while (open_set.size() > 0) {
		T current = open_set[val_with_min_f_score];
		if (current == goal_index) {
			path->clear();
			path->push_back(start);

			while (came_from.find((*path)[0]) != came_from.end()) {
				path->insert(path->begin(), came_from.find((*path)[0]));
			}
			return;
		}
		closed_set.push_back(current);
		open_set.remove(current);
		
		std::vector<T> neighboring_values = FindNeighbors(current);
		for (T neighbor : neighboring_values) {
			if (std::find(closed_set.begin(), closed_set.end(), neighbor) != closed_set.end()) {
				continue;
			}
			if (! (std::find(open_set.begin(), open_set.end(), neighbor) != open_set.end())) {
				open_set.push_back(neighbor);
			}

			float tentative_g_score = g_score[current] + glm::distance(current, neighbor);
			if (tentative_g_score >= g_score[neighbor]) {
				continue;
			}

			came_from[neighbor] = current;
			g_score[neighbor] = tentative_g_score;
			f_score[neighbor] = g_score[neighbor] 
				+ AStar_HeuristicCostEstimate(neighbor, goal, heuristic_option);

			if (f_score[neighbor] < min_f_score) {
				min_f_score = f_score[neighbor];
				val_with_min_f_score = neighbor;
			}
		}
	}

	ErrorHandler::showError("Error in AStar. Did not find a path.");
}