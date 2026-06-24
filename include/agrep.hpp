#ifndef HEADER_AGREP
#define HEADER_AGREP

#include <cstdint>
#include <string> 
#include <vector>
#include <filesystem> 

enum class enum_flags {
	searchfile,
	searchtext,
	depth,
	glbsearch,
	curr_path,
	isapath,
	verbose,
	search_hidden
};

class AGREP {
private: 
	// a max depth to recurcively iterate to finde something:-
	const int max_depth_default = 15;
	int max_depth; 

private: 
	// some needed vectors:-
	std::vector<std::string>args;
	// target to search:-
	std::vector<std::string>targets;

private:
	// to store all the similar line found in a file:-
	struct similar_store{
		std::filesystem::path path_of_file;
		std::vector<std::string> the_similar_line;
		std::vector<uint16_t> the_number_of_similar_line;
	};
	similar_store Similar_Store;
public: 
	void store_path_of_file(const std::filesystem::path& path);
	void store_similar_lines(const std::string& str, uint16_t number);
	void print_similar();
	void flush_all_similar_vectors();

private:
	// a structure to organise all the flags:-
	struct struct_flags {
		int searchfile{0};
		int searchtext{0};
		int depth{0};
		int glbsearch{0}; // global search form home directory;
		int curr_path{0}; // in this directory only
	    int isapath{0}; // search the file name {something}
		int verbose{0};
		int search_hidden{0};
	};

	// a flag object based on the structure:-
	struct_flags flags;

public:
	std::string working_path;
	// to get the home/defaut path of search;
	std::filesystem::path get_home();
	// to get the path of the working directory
	std::filesystem::path current_path();

public:
	// to set and check the used flags;
	void set_flags(std::string FLAGS, int i);
	// to get the flags value outside of the class; 
	bool get_flags(enum_flags flag_num);

public:	
	// to print the current working path;
	void print_current_path(const std::filesystem::path& path);
	// to iterate to all the possible directorirs;
	void search_directory_file(const std::filesystem::path& path, const std::string& tosearch);
	// to search text in a file:-
	void search_text(const std::string& path, const std::string& target);
	// search in the iterated or not iterated directory;
	void search_directory(const std::filesystem::path& path);
	// to open the working direcory for search text/pattern;
	void search_in_file(const std::filesystem::path& path, const std::string& target);

public:
	// actual search progam;
	void search(int argc,char* argv[]);
};

// a global agrep object so it use one single object all the places,
// while still allowing for creation of other objects:-
inline AGREP agrep;

#endif 
