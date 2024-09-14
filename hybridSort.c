#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>

void swap (int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int* nums, int n, int i) {
    int max = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if(left <= n && nums[left] > nums[max]) {
        max = left;
    }
    if(right <= n && nums[right] > nums[max]) {
        max = right;
    }

    if(i != max) {
        swap(&nums[i], &nums[max]);
        heapify(nums, n, max);
    }
}
void heapSort(int* nums, int n) {
    int parent = (n - 1) / 2;
    for(; parent >= 0; parent--) {
        heapify(nums, n, parent);
    }
    for (int i = n; i >= 0; i--) {
        swap(&nums[0], &nums[i]);
        heapify(nums, i - 1, 0);
    }
}

void insertionSort(int* array, int size) {
    // TODO: Implement insertion sort
    // your implementation must use the swap function above
	int i;
	for (i = 1; i < size; i++) {
		int key = array[i];
		int j = i - 1;
		while (j >= 0 && array[j] > key) {
			swap(array + j, array + j + 1);
			j--;
		}
		array[j + 1] = key;
	}
}

int partition(int arr[], int low, int high) {
	int x = arr[high]; //set x equal to the pivot value 
	int i = low - 1; //set index marker i one place left to low. e.g. if low is 0, i = -1. And i will denote the right bound for elements that are smaller than the pivot.
	int j; //delcare another marker j. j will go through each element in the array except the last one i.e. the pivot.
	for (j = low; j < high; j++) { // j starts with the first element of the array. j will stop at the element one left to the pivot
		if (arr[j] <= x) { //if arr[j] is less than or equal to pivot x
			i = i + 1; //increase the size/move the right bound one position right, for elements that are smaller than pivot by one
			int temp = arr[i]; //swap the newly identified element that is smaller than pivot to the newly allocated space
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}
	int temp = arr[i + 1]; //since arr[i] is the right bound for the elements that are smaller than the pivot, the pivot value should be place at i + 1 location. Therefore, swap arr[i + 1] with arr[high] (we didn't touch arr[high] during the entire partition function)
	arr[i + 1] = arr[high];
	arr[high] = temp;
	return i + 1; // return where the pivot is located, note: it's the index number
}

// quickSort will use low and high as index number
void quickSort(int arr[], int low, int high, int* depthTracker, int depthLimit, int size) {
// run only if there are 2 or more elements in the sublist
    if (*depthTracker > depthLimit) {
        heapSort(arr, size - 1);
        printf("depth: %d\nENGAGED heapsort", *depthTracker);
        //return;
    }else if (low < high) {
		(*depthTracker)++;
        int q = partition(arr, low, high);
		quickSort(arr, low, q - 1, depthTracker, depthLimit, size);
        if(*depthTracker > depthLimit) {
            return;
        }
		quickSort(arr, q + 1, high, depthTracker, depthLimit, size);	
	}
}

void quickSortNormal(int arr[], int low, int high) {
// run only if there are 2 or more elements in the sublist
    if (low < high) {
        int q = partition(arr, low, high);
		quickSortNormal(arr, low, q - 1);
		quickSortNormal(arr, q + 1, high);	
	}
}

void hybridSort(int* array, int size) {
    int* depthTracker = (int*)malloc(sizeof(int));
    *depthTracker = 0;
    int depthLimit = 2 * log2(size);
    if(size < 16) {
        insertionSort(array, size);
    } else {
        quickSort(array, 0, size - 1, depthTracker, depthLimit, size);
    }
    free(depthTracker);
}

int* generateRandomArray(int size) {
    srand(time(NULL));
    int* array = (int*)malloc(sizeof(int) * size);
    for(int i = 0; i < size; i++) {
        array[i] = rand() % INT_MAX;
    }
    return array;
}

int* generateReverseArray(int size) {
    int* array = (int*)malloc(sizeof(int) * size);
    for(int i = 0; i < size; i++) {
        array[i] = size - i;
    }
    return array;
}

int* generateNearlySortedArray(int size) {
    int* array = (int*)malloc(sizeof(int) * size);
    for(int i = 0; i < size; i++) {
        array[i] = i;
    }
    for(int i = 0; i < size - 1; i++) {
        if(rand() % size > size * 0.9) {
            swap(array + i, array + i + 1);
        }
    }
    return array;
}

void printArray(int* array, int size) {
    for(int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main() {
    int size[4] = {100, 1000, 10000, 100000};
    struct timespec begin, end;
    int* (*generateArray[3])(int) = {generateRandomArray, generateReverseArray, generateNearlySortedArray};
    for(int j = 0; j < 3; j++){
        char* arrayType;
        if(j == 0) {
            arrayType = "Random";
        } else if(j == 1) {
            arrayType = "Reverse";
        } else {
            arrayType = "Nearly Sorted";
        }
        for(int i = 0; i < 4; i++) {
            int* randomArray = generateArray[j](size[i]);
            clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
            hybridSort(randomArray, size[i]);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            //printArray(randomArray, size[i]);
            double time_taken = (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec - begin.tv_sec);
            printf("%s array, %d, Hybrid sort, %.9lf\n",arrayType, size[i], time_taken);
            free(randomArray);

            randomArray = generateArray[j](size[i]);
            clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
            quickSortNormal(randomArray, 0, size[i] - 1);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            time_taken = (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec - begin.tv_sec);
            printf("%s array, %d, Quicksort, %.9lf\n", arrayType, size[i], time_taken);
            free(randomArray);
        
            randomArray = generateArray[j](size[i]);
            clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
            insertionSort(randomArray, size[i]);
            /*for(int g = 0; g < size[i]; g++) {
                printf("%d ", randomArray[g]);
            }
            printf("\n");
            */
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            time_taken = (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec - begin.tv_sec);
            printf("%s array, %d, Insertion sort, %.9lf\n", arrayType, size[i], time_taken);
            free(randomArray);

            randomArray = generateArray[j](size[i]);
            /*for(int i = 0; i < size; i++) {
                printf("%d ", randomArray[i]);
            }
            printf("\n");
            */
            clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
            heapSort(randomArray, size[i] - 1);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            time_taken = (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec - begin.tv_sec);
            printf("%s array, %d, Heap sort, %.9lf\n", arrayType, size[i], time_taken);
            free(randomArray);
        }
    }

    //printf("%ld\n", time(NULL));
    return 0;
}