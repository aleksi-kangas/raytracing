#pragma once

/**
 * Generate a random double in range [0, 1].
 * @return generated double
 */
double RandomDouble();

/**
 * Generate a random double in range [min, max].
 * @param min lower bound
 * @param max upper bound
 * @return generated double
 */
double RandomDouble(double min, double max);

/**
 * Generate a random integer in range [min, max].
 * @param min lower bound
 * @param max upper bound
 * @return generated integer
 */
int RandomInt(int min, int max);
