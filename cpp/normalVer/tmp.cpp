#include <iostream>
#include <vector>
#include <algorithm>

int findInsertPosition(const std::vector<int>& vec, int target) {
    int low = 0;
    int high = vec.size();

    while (low < high) {
        int mid = low + (high - low) / 2;

        if (vec[mid] < target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    return low;
}

int main() {
    std::vector<int> sortedVector = {1, 3, 5, 6, 7, 9};
    int target = 6;

    int insertPosition = findInsertPosition(sortedVector, target);
    sortedVector.insert(sortedVector.begin() + insertPosition, target);

    std::cout << "The target should be inserted at position: " << insertPosition << std::endl;

    for(int i : sortedVector){
        std::cout << i << " ";
    }

    return 0;
}
