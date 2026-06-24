#include "../include/levenshtein_distance.hpp"

#include <iostream>
#include <algorithm>
#include <vector>

double min_similarity(int max_lenght) {
	if (max_lenght <= 3) return 50.0;
	// else if (max_lenght >= 3 && max_lenght <= 5) return 60.0;
	return 70.0;
}

int levenshtein_dist(const std::string& target, const std::string& source, int target_size, int source_size) {
	int m = source_size;
	int n = target_size;

	if (m==0) return n;
	if (n==0) return m;

	std::vector<int> prev(n + 1), curr(n + 1);
	
	for (int col = 0; col <= n; col++) prev[col] = col;

	for(int row = 1; row <= m; row++) {
		curr[0] = row;
		for (int col = 1; col <=n; col++) {
			curr[col] = (source[row-1] == target[col-1]) ?
				(prev[col-1]) : 
				(1 + (std::min( prev[col], std::min({prev[col-1], curr[col-1]}))));
		}
		std::swap(prev, curr);
	}
	return prev[n];
} 

bool similarity(const std::string& target, const std::string& source) {
	int target_size = target.size();
	int source_size = source.size();

	double max_lenght = std::max(target.size(), source.size());
	double dist = levenshtein_dist(target, source, target_size, source_size);

	// double minimum_similarity = min_similarity(max_lenght);
	double percent_similarity = (1.0 - (dist/max_lenght)) * 100.0;
	
	// check the flags property and perform accordingly:-
	// if(agrep.get_flags(enum_flags::verbose)) std::cout << target 
	// 	<< " similar to " << source << " by " 
	// 	<< percent_similarity << "\n";
	
	return (percent_similarity >= min_similarity(max_lenght));
}
