#include "../include/agrep.hpp"
#include "../include/levenshtein_distance.hpp"

#include <string>
#include <filesystem>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

void AGREP::store_path_of_file(const std::filesystem::path& path) {
	Similar_Store.path_of_file = path;
}


void AGREP::store_similar_lines(const std::string& str, uint16_t number) {
	Similar_Store.the_similar_line.push_back(str);
	Similar_Store.the_number_of_similar_line.push_back(number);
}

// to make all the vectores zero before reusing it:-
void AGREP::flush_all_similar_vectors() {
	Similar_Store.the_number_of_similar_line.clear();
	Similar_Store.the_similar_line.clear();
	Similar_Store.path_of_file.clear();
}

// to print all the stored similar lines we found in a file:-
void AGREP::print_similar() {
	print_current_path(Similar_Store.path_of_file);
	int i{0};
	for(std::string line : Similar_Store.the_similar_line) {
		std::cout << "\t" << Similar_Store.the_number_of_similar_line[i] <<
			'\t' << line << '\n';
		i++;
	}
}

std::filesystem::path AGREP::get_home() {
#ifdef _WIN32
	const char* Home = std::getenv("USERPROFILE");
#else
	const char* Home = std::getenv("HOME");
#endif
	return std::filesystem::path(Home);
}

std::filesystem::path AGREP::current_path() {
	return std::filesystem::current_path();
}

void AGREP::print_current_path(const std::filesystem::path& path) {
	std::cout << path << "\n";
}

// void AGREP::set_flags(std::string FLAGS, int i) {
// 	for(char C : FLAGS) {
// 		char c = std::tolower(C);
// 		switch (C) {
// 			case 'f': flags.searchfile	= i; break;
// 			case 't': flags.searchtext 	= i; break;
// 			case 'd': flags.depth		= i; break;
// 			case 'g': flags.glbsearch 	= i; break;
// 			case 'c': flags.curr_path	= i; break;
// 			case 'p': flags.isapath		= i; break;
// 			case 'v': flags.verbose		= i; break;
// 		}
// 	}
// }

// bool AGREP::get_flags(enum_flags flag_num) {
// 	switch (flag_num) {
// 		case enum_flags::searchfile:	return flags.searchfile;	break;
// 		case enum_flags::searchtext:	return flags.searchtext;	break;
// 		case enum_flags::depth:			return flags.depth;			break;
// 		case enum_flags::glbsearch:		return flags.glbsearch;		break;
// 		case enum_flags::curr_path:		return flags.curr_path; 	break;
// 		case enum_flags::isapath:		return flags.isapath;		break;
// 		case enum_flags::verbose:		return flags.verbose;		break;
// 		case enum_flags::search_hidden:	return flags.search_hidden;	break;
// 	}
// 	return false;
// }


// checks if the file is binary or not:-
bool is_binary_file(const std::filesystem::path& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) return false; // couldn't open — handle separately

    char buffer[1024];
    file.read(buffer, sizeof(buffer));
    std::streamsize bytesRead = file.gcount();

    for (std::streamsize i = 0; i < bytesRead; i++) {
        if (buffer[i] == '\0') return true; // found a NUL byte -> treat as binary
    }
    return false;
}

// for searching text in a file:-
// void AGREP::search_text(const std::string& path,
// 		const std::string& target) {
// 	int depth_count{0};
// 	for (auto const& dir_entry :
// 			std::filesystem::recursive_directory_iterator(path)){
// 		// checks if its a regular file and not a directory or a binary
// 		// before doing anything;
// 		if(depth_count > max_depth) {
// 			dir_entry.disable_recursion_pending();
// 		}
// 		if(dir_entry.is_regular_file() 
// 				&& !is_binary_file(dir_entry.path())) {
// 			// if the verbose flags is set it will print which file its
// 			// working on:
// 			if(flags.verbose) print_current_path(dir_entry);
// 			// search the text preset in the file:-
// 			search_in_file(dir_entry.path(), target);
// 		}
// 		depth_count++;
// 	}
// }

// // code written with the help of ai:-
void AGREP::search_text(const std::string& path, const std::string& target) {
    namespace fs = std::filesystem;
	bool is_hidden{false};

    for (auto it = fs::recursive_directory_iterator(path);
         it != fs::recursive_directory_iterator();
         ++it) 
	{
        const fs::directory_entry& dir_entry = *it;

		// to check if the file or directory is hidden or not:-
		std::string file_name = dir_entry.path().filename();
		bool is_hidden = !file_name.empty()	&& file_name[0] == '.';

        // if we've gone deeper than allowed, don't descend further

		// dont read hidden files and directory, untill flag is set:-
		if(is_hidden && !flags.search_hidden) {
			if(dir_entry.is_directory()) it.disable_recursion_pending();
			continue;
		}

		// dont read after certain depth:-
        if (it.depth() >= max_depth) {
			std::cout << it.depth() << '\n';
            it.disable_recursion_pending();
        }
	
        if (dir_entry.is_regular_file() && !is_binary_file(dir_entry.path())) {
            if (flags.verbose) print_current_path(dir_entry);
            search_in_file(dir_entry.path(), target);
        }
    }
}

void AGREP::search_in_file(const std::filesystem::path& path, const std::string& target){
	// opening the file:-
	std::ifstream file(path);
	// error handling, if the file is not open:-
	if(!file) {
		std::cout << "file could not be found (*~*)\n";
		exit(1);
	}
	
	store_path_of_file(path);
	// creating a buffer to store the line:-
	std::string line_buff;
	// a integer to keep track of the line number:-
	uint16_t line_number{};

	// bool to check if similarity exists:-
	bool similarity_buff{false};
	
	while(std::getline(file, line_buff)) {
		line_number++;

		// braking down all the words into a single string:-
		std::vector<std::string>words;
		std::string word_buff;
		std::istringstream iss(line_buff);

		while(iss >> word_buff) {
			words.push_back(word_buff);
		}

		// checking for similarity in those broken down words 
		// if it exists in the line;
		for(std::string source : words) {
			if(similarity(target, source)){ 
				similarity_buff = true;
				// store the line and line number in a vector:-
				store_similar_lines(line_buff, line_number);
			}
		}
	}
	if(similarity_buff) print_similar();
	flush_all_similar_vectors();
}

// void AGREP::search_directory_file(const std::filesystem::path& path,
// 		const std::string& tosearch) {
// 	for (auto const& dir_entry :
// 			std::filesystem::recursive_directory_iterator(path)){
// 		if(flags.verbose) print_current_path(dir_entry);
//
// 		std::string filename = dir_entry.path().filename();
// 		if (similarity(filename, tosearch)) print_current_path(dir_entry);
// 	}

// }
// for searching a specific directory or a file:-
void AGREP::search_directory_file(const std::filesystem::path& path, 
		const std::string& target) {
	for(auto it = std::filesystem::recursive_directory_iterator(path);
			it != std::filesystem::recursive_directory_iterator();
			*it++) {
		const std::filesystem::directory_entry& dir_entry = *it;
		
		std::string file_name = dir_entry.path().filename();
		bool is_hidden = !file_name.empty() && file_name[0] == '.';
	
		if(is_hidden && !flags.search_hidden) {
			it.disable_recursion_pending();
		}

		if(it.depth() >= max_depth) {
			// if(flags.verbose) std::cout << it.depth() << '\n';
			it.disable_recursion_pending();
		}
		if(similarity(file_name, target)) print_current_path(dir_entry.path());
	}
}

int char_number(std::string number_string) {
	int number{};
	int number_buffer{};
	for(char n : number_string) {
		number_buffer = n - '0';
		number = (number * 10) + number_buffer;
	}
	return number;
}

void AGREP::search(int argc,char* argv[]) {
	char** arg_ptr = argv + 1;
	int index{1};
	int args_size{0};
	for(int i = 1; i < argc; i++) {
		std::string arg = *arg_ptr; 
		
		if(arg[0] == '-') {
			// set_flags(arg, i);
			for(char C : arg) {
				char c = std::tolower(C);
				if(c == 'd') {
					flags.depth = index;
					++index;
				}	
				else if(c == 'p') {
					flags.isapath = index;
					++index;
				}
				else if(c == 'f') {
					flags.searchfile = index;
					++index;
				}
				else if(c == 't') {
					flags.searchtext = index;
					++index;
				}
				else {
					switch (c) {
						case 'g': flags.glbsearch	= true; break;
						case 'c': flags.curr_path	= true; break;
						case 'v': flags.verbose		= true; break;
						case 'h': flags.search_hidden = true; break;
					}
				}
			}
			arg_ptr++;
			continue;
		}
		// this statement is skipped if the command is flags;
		args.push_back(arg);
		args_size++;
		arg_ptr++;
	}
	int flags_index_offset = 1;
	if(flags.depth) max_depth = char_number(args[flags.depth - flags_index_offset]);
	else max_depth = max_depth_default;

	if(flags.isapath) working_path = args[flags.isapath - flags_index_offset];
	else if(flags.curr_path) working_path = current_path();
	else if(flags.glbsearch) working_path = get_home();
	else working_path = current_path();

	for (int index = 0; index < args_size; index++) {
		if(index == flags.depth - flags_index_offset) continue;
		if(index == flags.isapath - flags_index_offset) continue;
		targets.push_back(args[index]);
	}
	if(flags.searchfile) {
		for (std::string target : targets) {
			search_directory_file(working_path, target);
		}
	}	
	else if(flags.searchtext) {
		for(std::string target : targets) {
			search_text(working_path, target);
		}	
	}
}
