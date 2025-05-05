#include <iostream>
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
        ~heap();

        void push(T value);
        void pop();
        T top();
};

template <class t_darray,class T> 
heap<class t_darray,class T>::heap(){
    this->n_size = 0;
}

template <class t_darray,class T> 
void heap<class t_darray,class T>::heapify_up(int position){
    while(poistion){
        int parent = HEAP_PARENT(position);
        if(arr[parante] < arr[position]){ 
            swap(arr[parent],arr[position]);
        } else {
            return;
        }
        position = parentl
    }
}

template <class t_darray,class T> 
void heap<class t_darray,class T>::heapify_down(int position){
    while(poistion < n){
        int left_child = HEAP_LEFT_CHILD(position);
        int right_child = HEAP_RIGHT_CHILD(position);

        
        if(left_child >= n) 
            return;
        
        int max_child = left_child;

        if(right_child < n && arr[right_child] > arr[left_child])
            max_child = right_child;

        if(arr[max_child] <= arr[position]){
            return;
        }

        swap(arr[right_child],arr[position]);
        position = max_child;
    }
}

template <class t_darray,class T> 
void heap<class t_darray,class T>::push(T value){
    arr.push_back(value);
    heapify_up(n_size);
    n_size++;
}

template <class t_darray,class T> 
void heap<class t_darray,class T>::pop(){
    n_size--;
    swap(arr[n_size],arr[0]);
    arr.pop_back();
    heapify_down(0);
}

template <class t_darray,class T> 
T heap<class t_darray,class T>::top(){
    if(n_size == 0){
        std::cerr << "Error: HEAP empty." << std::endl;
        exit(1);
    }
    return arr[0];
}