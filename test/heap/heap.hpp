#include <iostream>
#include <algorithm>

#define HEAP_PARENT(position) (position + 1)/2 - 1
#define HEAP_LEFT_CHILD(position) 2*(position + 1) - 1
#define HEAP_RIGHT_CHILD(position) 2*(position + 1)

template <class t_darray,class T> 
class heap {
    private:
        int n_size;
        t_darray arr;

        void heapify_up(int position);
        void heapify_down(int position);

    public:
        // constructor
        heap();

        // destructor
        //~heap();

        void push(T value);
        void pop();
        T top();
};

template <class t_darray,class T> 
heap<t_darray, T>::heap(){
    this->n_size = 0;
}

template <class t_darray,class T> 
void heap<t_darray, T>::heapify_up(int position){
    while(position){
        int parent = HEAP_PARENT(position);
        if(arr[parent] < arr[position]){ 
            std::swap(arr[parent],arr[position]);
        } else {
            return;
        }
        position = parent;
    }
}

template <class t_darray,class T> 
void heap<t_darray, T>::heapify_down(int position){
    while(position < this->n_size){
        int left_child = HEAP_LEFT_CHILD(position);
        int right_child = HEAP_RIGHT_CHILD(position);

        
        if(left_child >= this->n_size) 
            return;
        
        int max_child = left_child;

        if(right_child < this->n_size && arr[right_child] > arr[left_child])
            max_child = right_child;

        if(arr[max_child] <= arr[position]){
            return;
        }

        std::swap(arr[max_child],arr[position]);
        position = max_child;
    }
}

template <class t_darray,class T> 
void heap<t_darray, T>::push(T value){
    this->arr.push_back(value);
    this->heapify_up(this->n_size);
    this->n_size++;
}

template <class t_darray,class T> 
void heap<t_darray, T>::pop(){
    this->n_size--;
    std::swap(arr[n_size],arr[0]);
    this->arr.pop_back();
    this->heapify_down(0);
}

template <class t_darray,class T> 
T heap<t_darray, T>::top(){
    if(this->n_size == 0){
        std::cerr << "Error: HEAP empty." << std::endl;
        exit(1);
    }
    return this->arr[0];
}