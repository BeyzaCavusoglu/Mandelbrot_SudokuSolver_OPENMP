#include "mandelbrot.hpp"
#include <iostream>
#include <thread>
#include <vector>

#ifndef KUACC
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#endif

#define RUN_TEST(img, test) zoom_in_test(img, test, #test)

#define window_width 1000
#define window_height 1000

std::vector<std::pair<int, int>> zoom_in_test_all_white = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
};

std::vector<std::pair<int, int>> zoom_in_test_all_black = {
    {window_width / 2, window_height / 2},
    {window_width / 2, window_height / 2},
    {window_width / 2, window_height / 2},
    {window_width / 2, window_height / 2},
};

std::vector<std::pair<int, int>> zoom_in_test_mixed = {
    {469, 311},
    {560, 685},
    {924, 557},
    {787, 581},
};

void zoom_in_test(mandelbrot &img, std::vector<std::pair<int, int>> test_coords,
                  std::string test_name) {
  // Make sure the img is reset before performing the zoom in test
  img.zoomToTop();
  img.join();

  std::cout << "Starting Test (" << test_name << ")" << std::endl;

  auto start_all = std::chrono::high_resolution_clock::now();
  for (std::pair<int, int> coord : test_coords) {
    auto start = std::chrono::high_resolution_clock::now();

    img.zoomIn(coord.first, coord.second);
    img.join();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "\tDuration for zoom_in at (" << coord.first << ", "
              << coord.second << "): " << duration.count() << " ms"
              << std::endl;
  }
  auto end_all = std::chrono::high_resolution_clock::now();

  auto duration_all = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_all - start_all);

  img.save(test_name + ".ppm");

  std::cout << "Finished test ( " << test_name
            << ") in: " << duration_all.count() << " ms" << std::endl;
  std::cout << "Saved result to file (" << test_name << ".ppm)" << std::endl;
  std::cout << "===============================================" << std::endl;
}

int main(int, char const **) {
  mandelbrot img(window_width, window_height, 5000, 15);

#ifdef KUACC
  RUN_TEST(img, zoom_in_test_mixed);
  RUN_TEST(img, zoom_in_test_all_white);
  RUN_TEST(img, zoom_in_test_all_black);
#else
  sf::RenderWindow window(sf::VideoMode(window_width, window_height),
                          "SFML window");

  window.setFramerateLimit(60);

  while (window.isOpen()) {
    // Process events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Escape) {
        window.close();
      }

      if (event.type == sf::Event::KeyPressed && img.isFinished()) {

        switch (event.key.code) {
        case sf::Keyboard::Left:
          img.panImg(left);
          break;

        case sf::Keyboard::Up:
          img.panImg(up);
          break;

        case sf::Keyboard::Right:
          img.panImg(right);
          break;

        case sf::Keyboard::Down:
          img.panImg(down);
          break;

        case sf::Keyboard::T:
          img.zoomToTop();
          break;

        case sf::Keyboard::E:
          img.zoomIn(352, 503);
          img.zoomIn(490, 15);
          img.zoomIn(470, 434);
          img.zoomIn(490, 480);
          break;

        default:
          break;
        }
      }

      if (event.type == sf::Event::MouseButtonPressed && img.isFinished()) {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        if (event.mouseButton.button == sf::Mouse::Button::Left)
          img.zoomIn(mouse.x, mouse.y);
        else if (event.mouseButton.button == sf::Mouse::Button::Right)
          img.zoomOut();
        else if (event.mouseButton.button == sf::Mouse::Button::Middle)
          img.zoomToTop();
      }
    }
    window.clear();
    window.draw(vertexPixels);
    window.display();
  }
#endif

  return EXIT_SUCCESS;
}
