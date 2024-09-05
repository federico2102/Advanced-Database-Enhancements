#include "conjunto_avl.h"
#include "conjunto_avl.h"
using namespace std;

template <typename T>
bool conjunto_avl<T>::operator==(const conjunto_avl &otro) const{
    if(lon != otro.lon) return false;

    const T* actual = &minimo();
    for(int i=0; i<lon; i++){
        if(not otro.pertenece(*actual)) return false;
        actual = &siguiente(*actual);
    }

    return true;
}

template <typename T>
std::ostream &conjunto_avl<T>::operator<<(std::ostream &os) {
    os << "[";
    if(raiz != nullptr){
        const T* actual = &minimo();
        for(int i=0; i<lon; i++) {
            os << *actual;
            i != lon-1 ? os << ", " : os << "]" << endl;
            actual = &siguiente(*actual);
        }
    }
}

template <typename T>
conjunto_avl<T>::conjunto_avl() {
    raiz = nullptr;
    lon = 0;
}

template <typename T>
conjunto_avl<T>::~conjunto_avl() {
    if(raiz != nullptr){
        lon = 0;
        delete raiz;
    }

}

template <typename T>
void conjunto_avl<T>::insertar(const T &elem) {
    Nodo* actual = raiz;
    if(raiz == nullptr) {
        raiz = new Nodo(elem);
        lon++;
    }else {
        if (elem < actual->elem) {
            raiz = raiz->izq;
            insertar(elem);
            actual->izq = raiz;
            raiz = actual;
        } else {
            raiz = raiz->der;
            insertar(elem);
            actual->der = raiz;
            raiz = actual;
        }

        actual->altura = 1 + max(obtener_altura(actual->izq), obtener_altura(actual->der));

        int balance = obtener_altura(actual->izq) - obtener_altura(actual->der);
        if (balance > 1 and elem < actual->izq->elem) {
            rotar_derecha();
        } else {
            if (balance < -1 and actual->der->elem < elem) {
                rotar_izquierda();
            } else {
                if (balance > 1 and actual->izq->elem < elem) {
                    raiz = raiz->izq;
                    rotar_izquierda();
                    actual->izq = raiz;
                    raiz = actual;
                    rotar_derecha();
                } else {
                    if (balance < -1 and elem < actual->der->elem) {
                        raiz = raiz->der;
                        rotar_derecha();
                        actual->der = raiz;
                        raiz = actual;
                        rotar_izquierda();
                    }
                }
            }
        }
    }
}

template <typename T>
T& conjunto_avl<T>::obtener(const T& elem) {
    Nodo* actual = raiz;

    while(actual->elem != elem)
        elem < actual->elem ? actual = actual->izq : actual = actual->der;

    return actual->elem;
}

template <typename T>
bool conjunto_avl<T>::pertenece(const T& elem) const {
    if(raiz == nullptr)
        return false;

    Nodo* actual = raiz;
    while(actual != nullptr){
        if(actual->elem == elem){
            return true;
        }else{
            actual->elem < elem ? actual = actual->der : actual = actual->izq;
        }
    }

    return false;
}

template <typename T>
const T &conjunto_avl<T>::siguiente(const T &elem) const{
    bool listo = false;
    Nodo* actual = raiz;
    Nodo* padre;
    Nodo* abuelo;
    while(not listo){
        if(actual->elem == elem){
            listo = true;
            if(actual->der != nullptr) {
                actual = actual->der;
                while(actual->izq != nullptr)
                    actual = actual->izq;
                return actual->elem;
            } else{
                return padre->izq == actual ? padre->elem : abuelo->elem;
            }
        } else{
            abuelo = padre;
            padre = actual;
            actual->elem < elem ? actual = padre->der : actual = padre->izq;
        }
    }
}

template <typename T>
const T &conjunto_avl<T>::minimo() const {
    Nodo* actual = raiz;
    while(actual->izq != nullptr) //Me voy moviendo hacia la izquierda hasta el ultimo elemento
        actual = actual->izq;

    return actual->elem;
}

template <typename T>
const T &conjunto_avl<T>::maximo() const {
    Nodo* actual = raiz;
    while(actual->der != nullptr) //Me muevo hacia la derecha hasta el ultimo elemento
        actual = actual->der;

    return actual->elem;
}

template <typename T>
std::size_t conjunto_avl<T>::cardinal() const {
    return lon;
}

template <typename T>
void conjunto_avl<T>::rotar_derecha() {
    Nodo* hijo = raiz->izq;
    Nodo* nieto = hijo->der;

    hijo->der = raiz;
    raiz->izq = nieto;

    raiz->altura = 1 + max(obtener_altura(raiz->izq), obtener_altura(raiz->der));
    hijo->altura = 1 + max(obtener_altura(hijo->izq), obtener_altura(hijo->der));

    raiz = hijo;
}

template <typename T>
void conjunto_avl<T>::rotar_izquierda() {
    Nodo* hijo = raiz->der;
    Nodo* nieto = hijo->izq;

    hijo->izq = raiz;
    raiz->der = nieto;

    raiz->altura = 1 + max(obtener_altura(raiz->izq), obtener_altura(raiz->der));
    hijo->altura = 1 + max(obtener_altura(hijo->izq), obtener_altura(hijo->der));

    raiz = hijo;
}

template <typename T>
int conjunto_avl<T>::obtener_altura(const Nodo* n) const{
    int ret;
    n == nullptr ? ret = 0 : ret = n->altura;
    return ret;
}