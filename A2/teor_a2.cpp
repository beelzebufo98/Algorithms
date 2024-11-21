#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <numeric>
class ArrayGenerator {
private:
	const int max_size = 10000;
	const int min_value = 0;
	const int max_value = 6000;
	std::vector<int> array;

	void generateArray() {
		std::mt19937 generator(static_cast<unsigned>(std::time(nullptr)));
		std::uniform_int_distribution<int> distribution(min_value, max_value);
		array.resize(max_size);
		for (int& value : array) {
			value = distribution(generator);
		}
	}

public:
	ArrayGenerator() {
		generateArray();
	}

	std::vector<int> RandomArray(int size) {
		return std::vector<int>(array.begin(), array.begin() + size);
	}


	std::vector<int> DecreaseArray(int size) {
		std::vector<int> result(array.begin(), array.begin() + size);
		std::sort(result.rbegin(), result.rend());
		return result;
	}

	std::vector<int> NearSorted(int size, int swaps = 10) {
		std::vector<int> result(array.begin(), array.begin() + size);
		std::sort(result.begin(), result.end());
		std::mt19937 generator(static_cast<unsigned>(std::time(nullptr)));
		std::uniform_int_distribution<int> index_dist(0, size - 1);
		for (int i = 0; i < swaps; ++i) {
			int idx1 = index_dist(generator);
			int idx2 = index_dist(generator);
			std::swap(result[idx1], result[idx2]);
		}
		return result;
	}
};


std::vector<int> merge(std::vector<int>& left, std::vector<int>&
	right) {
	std::vector<int> finish;
	int i = 0;
	int j = 0;
	while (i < left.size() && j < right.size()) {
		if (left[i] < right[j]) {
			finish.push_back(left[i]);
			i++;
		}
		else {
			finish.push_back(right[j]);
			j++;
		}
	}
	while (i < left.size()) {
		finish.push_back(left[i]);
		i++;
	}
	while (j < right.size()) {
		finish.push_back(right[j]);
		j++;
	}
	return finish;
}
std::vector<int> insertionSort(std::vector<int>& vect, int n) {
	for (int k = 1;k < n;k++) {
		for (int i = k;i > 0 && vect[i - 1] > vect[i];--i) {
			std::swap(vect[i - 1], vect[i]);
		}
	}
	return vect;
}


std::vector<int> modify_mergeinsertSort(std::vector<int>& start) {
	if (start.size() <= 15) {
		return insertionSort(start, start.size());
	}
	int mid = start.size() / 2;
	std::vector<int> left(start.begin(), start.begin() + mid);
	std::vector<int> right(start.begin() + mid, start.end());
	left = modify_mergeinsertSort(left);
	right = modify_mergeinsertSort(right);
	return merge(left, right);
}



std::vector<int> classic_mergeSort(std::vector<int>& start) {
	if (start.size() == 1) {
		return start;
	}
	int mid = start.size() / 2;
	std::vector<int> left(start.begin(), start.begin() + mid);
	std::vector<int> right(start.begin() + mid, start.end());
	left = classic_mergeSort(left);
	right = classic_mergeSort(right);
	return merge(left, right);
}


class SortTester {
public:
	long long measureMergeSortTime(std::vector<int> array, int repetitions = 10) {
		long long totalTime = 0;
		for (int i = 0; i < repetitions; ++i) {
			auto start = std::chrono::high_resolution_clock::now();
			classic_mergeSort(array);
			auto elapsed = std::chrono::high_resolution_clock::now() - start;
			totalTime += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
		}
		return totalTime / repetitions;
	}

	long long measureHybridSortTime(std::vector<int> array, int repetitions = 10) {
		long long totalTime = 0;
		for (int i = 0; i < repetitions; ++i) {
			auto start = std::chrono::high_resolution_clock::now();
			modify_mergeinsertSort(array);
			auto elapsed = std::chrono::high_resolution_clock::now() - start;
			totalTime += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
		}
		return totalTime / repetitions;
	}
};

int main() {
	ArrayGenerator generator;
	SortTester tester;
	std::vector<int> sizes;
	for (int size = 500; size <= 10000; size += 100) {
		sizes.push_back(size);
	}

	std::ofstream csvFileMerge("merge_results.csv");
	std::ofstream csvFileHybrid("hybrid_results.csv");

	csvFileMerge << "Size,Random,Decrease,NearSorted\n";
	csvFileHybrid << "Size,Random,Decrease,NearSorted\n";
	for (int size : sizes) {
		std::vector<int> randomArray = generator.RandomArray(size);
		std::vector<int> decreaseArray = generator.DecreaseArray(size);
		std::vector<int> nearSortedArray = generator.NearSorted(size);

		long long randomTimeMerge = tester.measureMergeSortTime(randomArray);
		long long decreaseTimeMerge = tester.measureMergeSortTime(decreaseArray);
		long long nearSortedTimeMerge = tester.measureMergeSortTime(nearSortedArray);

		long long randomTimeHybrid = tester.measureHybridSortTime(randomArray);
		long long decreaseTimeHybrid = tester.measureHybridSortTime(decreaseArray);
		long long nearSortedTimeHybrid = tester.measureHybridSortTime(nearSortedArray);

		csvFileMerge << size << "," << randomTimeMerge << "," << decreaseTimeMerge << "," << nearSortedTimeMerge << "\n";
		csvFileHybrid << size << "," << randomTimeHybrid << "," << decreaseTimeHybrid << "," << nearSortedTimeHybrid << "\n";

		std::cout << "Size: " << size
			<< " Random Merge: " << randomTimeMerge << " ms"
			<< " Decrease Merge: " << decreaseTimeMerge << " ms"
			<< " NearSorted Merge: " << nearSortedTimeMerge << " ms\n";

		std::cout << "Size: " << size
			<< " Random Hybrid: " << randomTimeHybrid << " ms"
			<< " Decrease Hybrid: " << decreaseTimeHybrid << " ms"
			<< " NearSorted Hybrid: " << nearSortedTimeHybrid << " ms\n";
	}

	csvFileMerge.close();
	csvFileHybrid.close();

	return 0;
}