#ifndef mandelbrot_hpp
#define mandelbrot_hpp

#ifndef KUACC
#include <SFML/Graphics.hpp>
#endif

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <thread>
#include <vector>

typedef double LD;

#ifdef KUACC
namespace sf {
////////////////////////////////////////////////////////////
/// \brief Utility template class for manipulating
///        2-dimensional vectors
///
////////////////////////////////////////////////////////////
template <typename T> class Vector2 {
public:
  Vector2(){};
  Vector2(T X, T Y) {
    this->x = X;
    this->y = Y;
  };

  template <typename U> explicit Vector2(const Vector2<U> &vector);

  T x; ///< X coordinate of the vector
  T y; ///< Y coordinate of the vector
};

template <typename T> Vector2<T> operator-(const Vector2<T> &right);

template <typename T>
Vector2<T> &operator+=(Vector2<T> &left, const Vector2<T> &right);

template <typename T>
Vector2<T> &operator-=(Vector2<T> &left, const Vector2<T> &right);

template <typename T>
Vector2<T> operator+(const Vector2<T> &left, const Vector2<T> &right);

template <typename T>
Vector2<T> operator-(const Vector2<T> &left, const Vector2<T> &right);

template <typename T> Vector2<T> operator*(const Vector2<T> &left, T right);

template <typename T> Vector2<T> operator*(T left, const Vector2<T> &right);

template <typename T> Vector2<T> &operator*=(Vector2<T> &left, T right);

template <typename T> Vector2<T> operator/(const Vector2<T> &left, T right);

template <typename T> Vector2<T> &operator/=(Vector2<T> &left, T right);

template <typename T>
bool operator==(const Vector2<T> &left, const Vector2<T> &right);

template <typename T>
bool operator!=(const Vector2<T> &left, const Vector2<T> &right);

typedef Vector2<float> Vector2f;

class Color {
public:
  uint8_t r, g, b, a;
  Color(){};
  Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    this->r = red;
    this->g = green;
    this->b = blue;
    this->a = alpha;
  }
};

} // namespace sf

class Point {
public:
  sf::Vector2f position;
  sf::Color color;

  Point(){};
};

extern std::vector<Point> vertexPixels;
#else
extern sf::VertexArray vertexPixels;
#endif

extern int pixels[400][600];

struct panDir {
  int x;
  int y;
};

enum pan_dir { left, up, right, down };

extern panDir pan[4];

class mandelbrot {

private:
  int width;
  int height;

  LD man_Wid;
  LD man_Height;

  LD centerX;
  LD centerY;

  LD offsetX;
  LD offsetY;

  LD zoom;
  LD zoomScalar;

  int max_iteration;

  //int pixelCount;

  std::thread th;

  bool finished;

public:
  mandelbrot();
  ~mandelbrot() {}
  void join() {
    if (!th.joinable()) {
      return;
    }

    th.join();
  }
  mandelbrot(int width, int height, int max_iteration, int zoomScalar);

  void zoomIn(int x, int y);
  void zoomOut();
  void zoomToTop();
  void panImg(pan_dir dir);
  void loop();

  void save(std::string file_name);

  void calculatePixel(int x, int y);

  void setColor(int x, int y, int iterations);

  LD picToMand_x(double x);
  LD picToMand_y(double y);

  bool isFinished();
};

inline LD mandelbrot::picToMand_x(double x) {
  return (x * (man_Wid / width)) + offsetX;
}

inline LD mandelbrot::picToMand_y(double y) {
  return (-y * (man_Height / height)) + offsetY;
}

inline bool mandelbrot::isFinished() { return finished; }

#endif /* mandelbrot_hpp */
