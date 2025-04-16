#include "etna/etna_core.hpp"
#include "y3.hpp"

constexpr uint32_t WINDOW_WIDTH{800};
constexpr uint32_t WINDOW_HEIGHT{600};

using namespace etna;

int main(int argc, char** argv) {
	y3 app(WINDOW_WIDTH, WINDOW_HEIGHT);

	try {
		app.switchScene("main");
		app.run();
	} catch (const std::exception& e) {
		std::cerr << "Error loading scene: " << e.what() << std::endl;
		return -1;
	}
}
