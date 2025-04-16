#include "etna/etna_core.hpp"
#include "y3.hpp"

constexpr uint32_t WINDOW_WIDTH{800};
constexpr uint32_t WINDOW_HEIGHT{600};

using namespace etna;

int main(int argc, char** argv) {
	uint32_t width = WINDOW_WIDTH;
	uint32_t height = WINDOW_HEIGHT;

	if (argc == 3) {
		width = std::stoi(argv[1]);
		height = std::stoi(argv[2]);
	}

	y3 app(width, height);

	try {
		app.switchScene("main");
		app.run();
	} catch (const std::exception& e) {
		std::cerr << "Error loading scene: " << e.what() << std::endl;
		return -1;
	}
}
