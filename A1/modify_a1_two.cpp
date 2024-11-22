#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <fstream>
#include <iomanip>

double findIntersection(double x1, double y1, double r1,
  double x2, double y2, double r2,
  double x3, double y3, double r3,
  int iterations,
  bool narrow) {
  double left, right, bottom, top;

  if (narrow) {
    left = std::max({ x1 - r1, x2 - r2, x3 - r3 });
    right = std::min({ x1 + r1, x2 + r2, x3 + r3 });
    bottom = std::max({ y1 - r1, y2 - r2, y3 - r3 });
    top = std::min({ y1 + r1, y2 + r2, y3 + r3 });
  }
  else {
    left = std::min({ x1 - r1, x2 - r2, x3 - r3 });
    right = std::max({ x1 + r1, x2 + r2, x3 + r3 });
    bottom = std::min({ y1 - r1, y2 - r2, y3 - r3 });
    top = std::max({ y1 + r1, y2 + r2, y3 + r3 });
  }

  int count = 0;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> disX(left, right);
  std::uniform_real_distribution<> disY(bottom, top);

  for (int i = 0; i < iterations; ++i) {
    double x = disX(gen);
    double y = disY(gen);

    if (std::sqrt(std::pow(x - x1, 2) + std::pow(y - y1, 2)) <= r1 &&
      std::sqrt(std::pow(x - x2, 2) + std::pow(y - y2, 2)) <= r2 &&
      std::sqrt(std::pow(x - x3, 2) + std::pow(y - y3, 2)) <= r3) {
      ++count;
    }
  }

  double rectArea = (right - left) * (top - bottom);
  return static_cast<double>(count) / iterations * rectArea;
}

int main() {
  setlocale(LC_ALL, "Russian");

  double x1 = 1.0, y1 = 1.0, r1 = 1.0;
  double x2 = 1.5, y2 = 2.0, r2 = std::sqrt(5) / 2.0;
  double x3 = 2.0, y3 = 1.5, r3 = std::sqrt(5) / 2.0;

  int minPoints = 100;
  int maxPoints = 100000;
  int step = 500;

  std::ofstream outFile("results_area.csv");
  outFile << "Points,WideArea,NarrowArea\n";
  for (int points = minPoints; points <= maxPoints; points += step) {
    double wideArea = findIntersection(x1, y1, r1, x2, y2, r2, x3, y3, r3, points, false);
    double narrowArea = findIntersection(x1, y1, r1, x2, y2, r2, x3, y3, r3, points, true);

    outFile << points << "," << wideArea << "," << narrowArea << "\n";

    std::cout << "Points: " << points
      << " | Wide Area: " << std::setprecision(8) << wideArea
      << " | Narrow Area: " << std::setprecision(8) << narrowArea
      << std::endl;
  }

  outFile.close();
  std::cout << "Результаты сохранены в файл results_area.csv" << std::endl;

  return 0;
}
