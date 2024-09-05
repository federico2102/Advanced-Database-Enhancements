#ifndef TP2_CONJUNTO_AVL_H
#define TP2_CONJUNTO_AVL_H

#include <string>
#include <iostream>

template<class T>
class conjunto_avl {

public:

    /**
     * @brief Chequeo de igualdad entre dos instancias de conjunto_avl
     *
     * @param otro conjunto_avl a comparar
     *
     * @return true si ambos conjuntos son iguales: todos los elementos
     *
     * \complexity{\O(n * log(m))}
     */
    bool operator==(const conjunto_avl& otro) const;
    std::ostream& operator<<(std::ostream& os);
    /**
     * Constructor sin parámetros de la clase
     *
     * \complexity{\O(1)}
     */
    conjunto_avl();

    /**
     * Destructor
     *
     * \complexity{\O(n)}
     */
    ~conjunto_avl();

    /**
     * @brief Inserta un elemento
     *
     * @param elem elemento a agregar
     *
     * \complexity{\O(log(n))}
     */
    void insertar(const T& elem);

    /**
     *
     * @param elem elemento a buscar
     * @return elemento del conjunto_avl por referencia
     *
     * \complexity{\O(log(n))}
     */
    T& obtener(const T& elem);


    /**
     * @brief Chequeo de pertenecia
     *
     * @param elem elemento a buscar
     *
     * @return true si el elemento se encuentra en el conjunto_avl
     *
     * \complexity{\O(log(n))}
     */
    bool pertenece(const T& elem) const;

    /**
     * @brief Siguiente en orden en conjunto_avl
     *
     * @param elem Elemento al que le quiero encontrar el siguiente
     *
     *\complexity{\O(log(n))}
     */
    const T& siguiente(const T& elem) const;

    /**
     * @brief Obtiene una puntero al nodo con valor minimo, devuelve nullptr si el nodo no existe
     *
     * @return el minimo del conjunto_avl
     *
     * \complexity{\O(log(n))}
     */
    const T& minimo() const;

    /**
     * @brief Obtiene una puntero al nodo con valor maximo, devuelve nullptr si el nodo no existe
     *
     * @return el maximo del conjunto_avl
     *
     * \complexity{\O(log(n))}
     */
    const T& maximo() const;

    /**
     * @brief Cardinal de conjunto_avl
     *
     * @return devuelve la cardinalidad del conjunto_avl
     *
     * \complexity{\O(1)}
     */
    std::size_t cardinal() const;

private:

    struct Nodo {
        T elem ;
        int altura;
        Nodo* izq;
        Nodo* der ;
        Nodo(const T& e) : elem(e), izq(nullptr), der(nullptr), altura(1) {}

        ~Nodo(){
            if(izq != nullptr)
                delete izq;
            if(der != nullptr)
                delete der;
        }
    };
    Nodo* raiz;
    std::size_t lon;

    //Auxiliares

    void rotar_derecha();
    void rotar_izquierda();
    int obtener_altura(const Nodo* n) const;
};

#include "conjunto_avl.hpp"

#endif //TP2_CONJUNTO_AVL_H