#ifndef _HEADER_LEVENSHTEIN_DISTANCE_
#define _HEADER_LEVENSHTEIN_DISTANCE_

#include <string>

	// for fuzzy search:- 
	double min_similarity(int max_lenght);
	int levenshtein_dist(const std::string& target, const std::string& source, int target_lenght,
			int source_lenght);
	bool similarity(const std::string& target, const std::string& source);

#endif
