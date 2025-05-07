import heapq


class MaxHeap:
    def __init__(self):
        self.heap_array = []

    def push(self, value):
        heapq.heappush(self.heap_array, -value)

    def pop(self):
        if not self.heap_array:
            raise IndexError("pop from empty heap")
        return -heapq.heappop(self.heap_array)

    def top(self):
        if not self.heap_array:
            raise IndexError("top from empty heap")
        return -self.heap_array[0]

    def is_empty(self):
        return not self.heap_array

    def size(self):
        return len(self.heap_array)
