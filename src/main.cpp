#include "../include/agrep.hpp"

int main(int argc, char* argv[]) {	
	agrep.search(argc, argv);
	// agrep.search_in_file(std::filesystem::path("/home/xefura/games/game.txt"), "abc");
	// agrep.search_text("/home/xefura/cpp/grep/src/", "grass");
	return 0;
}
