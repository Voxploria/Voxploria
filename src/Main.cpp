#include "Window.h"
#include "VulkanRenderer.h"

int main(int argc, char* argv[]) {
    try {
        Window window("Voxploria");
        VulkanRenderer renderer;

        renderer.Initialize();

        while (window.IsRunning()) {
            window.ProcessEvents();
        }

        renderer.Cleanup();
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
