#pragma once

/**
 * This is a straight(ish) translation of Ken Perlin's Improved Noise reference implementation
 * You can find it here:
 *     https://cs.nyu.edu/~perlin/noise/
 *
 * Notable changes:
 * - 1D and 2D optimized versions
 * - Un-nested some of the calls to help readability
 */

#include <array>

namespace capra {

class perlin {
public:
  static double noise1d(double x);
  static double noise2d(double x, double y);
  static double noise3d(double x, double y, double z);

  static double octave1d(double x, int octaves, double persistence);
  static double octave2d(double x, double y, int octaves, double persistence);
  static double octave3d(double x, double y, double z, int octaves, double persistence);

private:
  static double grad_(int hash, double x);
  static double grad_(int hash, double x, double y);
  static double grad_(int hash, double x, double y, double z);

  static double fade_(double t);

  static double lerp_(double t, double a, double b);

  static std::array<int, 512> p_;
};

} // namespace capra