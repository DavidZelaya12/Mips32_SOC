#include "vga/VGATextWindow.h"
#include "vga/VGAFramebuffer.h"
#include "vga/VGAFont.h"
#include "vga/Keypad.h"

#include "CLI_args/CliArgs.h"

#include <array>
#include <cstdint>
#include <format>
#include <iostream>

inline constexpr std::array<uint8_t, 16> kDefaultVGAPalette = {
    0x00,
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x0A,
    0x0B,
    0x0C,
    0x0D,
    0x0E,
    0x0F,
};

int main(int argc, const char *argv[])
{
    CliArgs cliArgs(argc, argv);

    if (auto err = cliArgs.parse(); err.has_value())
    {
        std::cerr << "Error parsing arguments: " << *err << "\n";
        cliArgs.printUsage();
        return EXIT_FAILURE;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << std::format("SDL_Init failed: '{}'\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    struct SDLGuard
    {
        ~SDLGuard() { SDL_Quit(); }
    } sdlGuard;

    Keypad keypad;
    VGAFramebuffer framebuffer;
    VGATextWindow window(VGA_WINDOW_WIDTH, VGA_WINDOW_HEIGHT, keypad);

    if (!window.initDisplay(framebuffer, (cliArgs.getFontPath().c_str()), kDefaultVGAPalette))
    {
        std::cerr << "Failed to initialize VGA display.\n";
        return EXIT_FAILURE;
    }

    framebuffer.clear(0x07, 0x00);
    constexpr const char *message = "VGA OK";
    for (size_t i = 0; message[i]; ++i)
        framebuffer.writePixel(12, 37 + i, message[i], 0x0A, 0x00);
    framebuffer.commitFrame();

    SDL_Event event;
    while (!window.quitRequested())
    {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
            break;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)
            break;
        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_EXPOSED)
            window.redraw();
    }

    return EXIT_SUCCESS;
}