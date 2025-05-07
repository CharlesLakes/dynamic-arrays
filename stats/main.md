---
marp: true
---

# Reunión 7/5

---

### Resultados: Binary Search (CPU)

<div align="center">

<img src="binary search__cpu_user.png">

</div>

---

### Resultados: Binary Search (Memory)

<div align="center">

<img src="binary search__memory.png">

</div>

---

### Resultados: Sort (CPU)

<div align="center">

<img src="sort__cpu_user.png">

</div>

---

### Resultados: Sort (Memory)

<div align="center">

<img src="sort__memory.png">

</div>

---

### Resultados: Stack (push random then pop) (CPU)

<div align="center">

<img src="stack_push_random_then_pop_cpu_user.png">

</div>

---

### Resultados: Stack (push random then pop) (Memory)

<div align="center">

<img src="stack_push_random_then_pop_memory.png">

</div>

---

### Resultados: Stack (push then random) (CPU)

<div align="center">

<img src="stack_push_random_then_pop_cpu_user.png">

</div>

---

### Resultados: Stack (push then random) (Memory)

<div align="center">

<img src="stack_push_then_random_memory.png">

</div>

---

### Resultados: Stack (random) (CPU)

<div align="center">

<img src="stack_random_cpu_user.png">

</div>

---

### Resultados: Stack (random) (Memory)

<div align="center">

<img src="stack_random_memory.png">

</div>

---

### Resultados: Stack (random then pop) (CPU)

<div align="center">

<img src="stack_random_then_pop_cpu_user.png">

</div>

---

### Resultados: Stack (random then pop) (Memory)

<div align="center">

<img src="stack_random_then_pop_memory.png">

</div>

---

### Heap

<style scoped>
pre,code{
    font-size:15px;
}
</style>

El heap fue implementado de forma personalizada, ya que el uso de la API de la STL resultaba demasiado complejo.

```c++
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
```

---

### Heap

Ya fue testeado, pero aún no se ha ejecutado en casos de gran tamaño.

TODO: Implementar Dijkstra. Como la ejecución actual ya está en curso, puedo aprovechar el tiempo para comenzar con esta implementación.