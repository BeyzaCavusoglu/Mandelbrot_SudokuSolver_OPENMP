#include "mandelbrot.hpp"
#include <cmath>
#include <iostream>
#include <omp.h>
#include <vector>

#ifdef KUACC
std::vector<Point> vertexPixels;
#endif

#ifndef KUACC
sf::VertexArray vertexPixels = sf::VertexArray(sf::Points, 1);
#endif
panDir pan[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

mandelbrot::mandelbrot(){};

mandelbrot::mandelbrot(int width, int height, int max_iteration,
                       int zoomScalar) {
  this->width = width;
  this->height = height;
  this->max_iteration = max_iteration;
  zoom = 1;
  this->zoomScalar = zoomScalar;

  man_Wid = 5;
  man_Height = 5;
  offsetX = -2.5;
  offsetY = 2.5;

  centerX = 0;
  centerY = 0;

  pixelCount = 0;

#ifdef KUACC
  vertexPixels.reserve(width * height);
  for (int i = 0; i < width * height; i++) {
    vertexPixels.push_back(Point());
  }
#else
  vertexPixels.resize(width * height);
  vertexPixels.setPrimitiveType(sf::Points);
#endif

  finished = false;

  th = std::thread(&mandelbrot::loop, this);
}

void mandelbrot::zoomIn(int x, int y) {
  join();

  centerX = picToMand_x(x);
  centerY = picToMand_y(y);

  zoom *= zoomScalar;
  man_Wid /= zoomScalar;
  man_Height /= zoomScalar;

  offsetX = (centerX - man_Wid / 2);
  offsetY = centerY + man_Height / 2;

  th = std::thread(&mandelbrot::loop, this);
}

void mandelbrot::save(std::string file_name) {
  join();

  std::ofstream stream;
  stream.open(file_name);

  stream << "P3\n" << width << " " << height << "\n255\n";
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      uint8_t red = vertexPixels[y * height + x].color.r;
      uint8_t green = vertexPixels[y * height + x].color.g;
      uint8_t blue = vertexPixels[y * height + x].color.b;
      stream << red % 256 << " " << green % 256 << " " << blue % 256 << " ";
    }
    stream << "\n";
  }

  stream.close();
}

void mandelbrot::zoomOut() {
  join();
  zoom /= zoomScalar;
  man_Wid *= zoomScalar;
  man_Height *= zoomScalar;

  offsetX = (centerX - man_Wid / 2);
  offsetY = centerY + man_Height / 2;

  th = std::thread(&mandelbrot::loop, this);
}

void mandelbrot::zoomToTop() {
  join();
  man_Wid = 5;
  man_Height = 5;
  offsetX = -2.5;
  offsetY = 2.5;

  centerX = 0;
  centerY = 0;
  zoom = 1;
  th = std::thread(&mandelbrot::loop, this);
}

void mandelbrot::panImg(pan_dir dir) {
  join();

  int step = 100; // In window pixels, not plot units
  int xCenter = width / 2;
  int yCenter = height / 2;

  panDir cord = pan[dir];

  xCenter += cord.x * step;
  yCenter += cord.y * step;

  centerX = picToMand_x(xCenter);
  centerY = picToMand_y(yCenter);

  offsetX = (centerX - man_Wid / 2);
  offsetY = centerY + man_Height / 2;

  th = std::thread(&mandelbrot::loop, this);
}

void mandelbrot::loop() {
  finished = false;
  pixelCount = 0;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      calculatePixel(x, y);
      ++pixelCount;
    }
  }

  finished = true;
}

void mandelbrot::calculatePixel(int pix_X, int pix_Y) {
  LD x0 = picToMand_x(double(pix_X));
  LD y0 = picToMand_y(double(pix_Y));

  LD x = 0;
  LD y = 0;
  LD xtemp = 0;
  LD iter = 0;

  while (((x * x + y * y) <= 4) && iter <= max_iteration) {
    xtemp = x * x - y * y + x0;
    y = 2 * x * y + y0;
    x = xtemp;
    iter = iter + 1;
  }

  setColor(pix_X, pix_Y, iter);
}

void mandelbrot::setColor(int x, int y, int iterations) {

  int color = 0;
  if (iterations >= max_iteration) {
    color = 0;
  } else {
    color = 1;
  }

  // You can play with the following values to get different coloring patterns
  int red = 256 - (iterations % 256);
  int green = 256 - (iterations % 256);
  int blue = 256 - (iterations % 256);

  vertexPixels[pixelCount].position = sf::Vector2f(x, y);
  vertexPixels[pixelCount].color =
      color == 1 ? sf::Color(red, green, blue % 256, 255)
                 : sf::Color(0, 0, 0, 255);
}
