//
// Created by Federico Hofmann on 10/12/17.
//

#ifndef TP2_STRING_MAP_ITERATORS_H
#define TP2_STRING_MAP_ITERATORS_H

#include "string_map.h"

using namespace std;


/*******************************************************************************************
 * * * * * * * * * * * * * * * * *  CONST_ITERADOR   * * * * * * * * * * * * * * * * * * * *
 *******************************************************************************************/

template<class T>
class string_map<T>::const_iterator {
public:

    using value_type = string_map::value_type;
    using iterator_category = std::forward_iterator_tag;
    using reference = const value_type&;
    using pointer = const value_type*;
    using difference_type = std::ptrdiff_t;

    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(1)}
     */
    const_iterator(const typename string_map<T>::iterator& otro);

    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(1)}
     */
    const_iterator(const typename string_map<T>::const_iterator& otro);

    /**
     * @brief Avanza el iterador una posición.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
     * siguiente.
     *
     * \complexity{\O(?)}
     */
    string_map::const_iterator &operator++();

    /**
     * @brief Desreferencia el puntero
     *
     * El valor devuelto tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es una referencia al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    const value_type &operator*() const;

    /**
     * @brief Operador flechita
     *
     * El valor devuelvo tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es un puntero al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    const value_type* operator->() const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores apuntan al mismo elemento
     *
     * \complexity{\O()}
     */
    bool operator==(const string_map<T>::const_iterator &otro) const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores no apuntan al mismo elemento
     *
     * \complexity{\O(?)}
     */
    bool operator!=(const string_map<T>::const_iterator &otro) const;

private:
    friend class string_map<T>;
    friend class iterator;
    using Nodo = typename string_map<T>::Nodo;

    const_iterator() { pastTheEnd = true; }
    const_iterator(string k,Nodo* n) : key(k), nodo(n) {}
    const_iterator(string k,Nodo* n, bool end) : key(k), nodo(n), pastTheEnd(end){}

    Nodo* nodo{nullptr};
    string key;
    bool pastTheEnd{false};

    //Funciones auxiliares
    /*
    Nodo* sigoBajando(Nodo* proximo) {
        for (int i = 0; i < 256; i++) {
            if (proximo->siguientes[i] != nullptr) {
                proximo = proximo->siguientes[i];
                if (proximo->K != nullptr)
                    return iterator(proximo);
            }
        }
        return nullptr;
    }

    Nodo* parteDelMismoNivel(Nodo* n, Nodo* proximo) {
        Nodo* ret;
        for(int i=0; i<256; i++){
            if(proximo->padre->siguientes[i] != nullptr) {
                proximo = proximo->padre->siguientes[i];
                if(proximo->K != nullptr and *proximo->K > *n->K) {
                    return iterator(proximo);
                } else {
                    if(proximo->K == nullptr) {
                        ret = sigoBajando(proximo);
                        if (ret != nullptr and ret->K != n->K)return ret;
                    }
                }
            }
        }
        return nullptr;
    }
    */
};

template<typename T>
string_map<T>::const_iterator::const_iterator(const typename string_map<T>::iterator& otro) {
    key = otro.key;
    nodo = otro.nodo;
}

template<typename T>
string_map<T>::const_iterator::const_iterator(const typename string_map<T>::const_iterator& otro) {
    key = otro.key;
    nodo = otro.nodo;
}

template<typename T>
typename string_map<T>::const_iterator &string_map<T>::const_iterator::operator++() {
    std::cout << "const_iterator::Operator++"<< std::endl;
    //TODO Busco el siguiente
    return *this;
}

template<typename T>
const typename string_map<T>::const_iterator::value_type &string_map<T>::const_iterator::operator*() const {
    return make_pair(key,*nodo->definicion);
}

template<typename T>
const typename string_map<T>::const_iterator::value_type *string_map<T>::const_iterator::operator->() const {
    value_type ret = make_pair(key,*nodo->definicion);
    return &ret;
}

template<typename T>
bool string_map<T>::const_iterator::operator==(const string_map<T>::const_iterator &otro) const {
    bool ret;
    if(nodo->definicion != nullptr && otro.nodo->definicion != nullptr)
        ret = (*nodo->definicion == *otro.nodo->definicion);
    else
        ret = (nodo->definicion == otro.nodo->definicion);

    return (ret && key== otro.key);
}

template<typename T>
bool string_map<T>::const_iterator::operator!=(const string_map<T>::const_iterator &otro) const {
    return not (*this == otro);
}







/*******************************************************************************************
 * * * * * * * * * * * * * * * * * * *   ITERADOR  * * * * * * * * * * * * * * * * * * * * *
 *******************************************************************************************/

template<class T>
class string_map<T>::iterator {
public:
    using value_type = const string_map::value_type;
    using iterator_category = std::forward_iterator_tag;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;

    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(1)}
     */
    iterator(const typename string_map<T>::iterator& otro);

    /**
     * @brief Avanza el iterador una posición.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
     * siguiente.
     *
     * \complexity{\O(?)}
     */
    string_map::iterator &operator++();

    /**
     * @brief Avanza el iterador una posición.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
     * siguiente.
     *
     * \complexity{\O(?)}
     */
    string_map::iterator &operator--();

    /**
     * @brief Desreferencia el puntero
     *
     * El valor devuelto tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es una referencia al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    const value_type &operator*() const;

    /**
     * @brief Operador flechita
     *
     * El valor devuelvo tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es un puntero al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    const value_type* operator->() const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores apuntan al mismo elemento
     *
     * \complexity{\O()}
     */
    bool operator==(const string_map<T>::iterator &otro) const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores no apuntan al mismo elemento
     *
     * \complexity{\O(?)}
     */
    bool operator!=(const string_map<T>::iterator &otro) const;


private:
    friend class string_map<T>;
    using Nodo = typename string_map<T>::Nodo;

    iterator() { pastTheEnd = true; }
    iterator(string k,Nodo* n) : key(k), nodo(n) {}
    iterator(string k,Nodo* n, bool end) : key(k), nodo(n), pastTheEnd(end){}

    Nodo* nodo{nullptr};
    string key;
    bool pastTheEnd{false};

};

template<typename T>
string_map<T>::iterator::iterator(const typename string_map<T>::iterator& otro) {
    key = otro.key;
    nodo = otro.nodo;
}

template<typename T>
typename string_map<T>::iterator &string_map<T>::iterator::operator++() {
    //std::cout << "----- iterator::Operator++ -- Estoy en "<< key << "  -----" << std::endl;

    // Chequeo si tengo que seguir bajando
    Nodo* actual = nodo;
    string newKey = key;
    //std::cout << "Chequeo si tengo que seguir bajando" << std::endl;
    for(int i=0; i < 256; i++){
        if(actual->siguientes[i] != nullptr){
            actual = actual->siguientes[i];
            char letter = (char)i;
            newKey += letter;
            if(actual->definicion != nullptr)
                break;
        }
    }

    //std::cout << "newKey:" << newKey << std::endl;

    if(actual == nodo){
        //No baje, entonces subo.
        //std::cout << "No baje, entonces subo." << std::endl;
        bool encontrado = false;
        newKey = key;

        for(int l=key.size()-1; l != -1 && !encontrado; l--){
            actual = actual->padre;
            //std::cout << "Subo un nivel, L: "<< l <<", newKey: "<< newKey << std::endl;
            //std::cout <<"Letra: "<<key[l]<<std::endl;
            int sig = int(key[l]) + 1;
            //std::cout << "siguientes: " << sig << std::endl;
            newKey = newKey.substr(0,l);
            //std::cout <<"newKey: "<<newKey<<std::endl;

            string toAppend;
            while(sig < 256){
                if(actual->siguientes[sig] != nullptr){
                    actual = actual->siguientes[sig];
                    char letter = (char)sig;
                    toAppend += letter;
                    if(actual->definicion != nullptr) {
                        //std::cout <<"toAppend: "<<toAppend<<std::endl;
                        encontrado = true;
                        newKey.append(toAppend);
                        break;
                    }else{
                        sig = 0;
                        l++;
                    }
                }
                sig++;
            }
        }

        if(!encontrado){
            pastTheEnd = true;
        }else{
            nodo = actual;
            key = newKey;
        }

    }else{
        nodo = actual;
        key = newKey;
    }

    //std::cout << "newKey:"<< key << " es ultimo? " << pastTheEnd << std::endl;

    return *this;
}

template<typename T>
typename string_map<T>::iterator &string_map<T>::iterator::operator--() {
    if(!pastTheEnd) {
        // Chequeo si tengo que seguir bajando
        Nodo *actual = nodo;
        string newKey = key;

        //No baje, entonces subo.
        bool encontrado = false;
        newKey = key;

        for (int l = key.size() - 1; l != -1 && !encontrado; l--) {
            actual = actual->padre;
            int sig = int(key[l]) - 1;

            newKey = newKey.substr(0, l);

            string toAppend;
            while (-1 < sig) {
                if (actual->siguientes[sig] != nullptr) {
                    actual = actual->siguientes[sig];
                    char letter = (char) sig;
                    toAppend += letter;
                    if (actual->definicion != nullptr) {
                        encontrado = true;
                        newKey.append(toAppend);
                        break;
                    } else {
                        sig = 256;
                        l++;
                    }
                }
                sig--;
            }
        }

        if (!encontrado) {
            pastTheEnd = true;
        } else {
            nodo = actual;
            key = newKey;
        }
    }else
        pastTheEnd = false;

    return *this;
}



template<typename T>
const typename string_map<T>::iterator::value_type &string_map<T>::iterator::operator*() const {
    return make_pair(key,*nodo->definicion);
}

template<typename T>
const typename string_map<T>::iterator::value_type *string_map<T>::iterator::operator->() const {
    return &(*(*this));
}

template<typename T>
bool string_map<T>::iterator::operator==(const string_map<T>::iterator &otro) const {
    bool ret = (pastTheEnd == otro.pastTheEnd);

    if(pastTheEnd || otro.pastTheEnd) {

    }else{
        if (nodo != nullptr && otro.nodo != nullptr) {
            if (nodo->definicion != nullptr && otro.nodo->definicion != nullptr)
                ret = ((*nodo->definicion == *otro.nodo->definicion) && ret);
            else
                ret = ((nodo->definicion == otro.nodo->definicion) && ret);
        }
    }
    return (ret && key== otro.key);
}

template<typename T>
bool string_map<T>::iterator::operator!=(const string_map<T>::iterator &otro) const {
    return not (*this == otro);
}


#endif //TP2_STRING_MAP_ITERATORS_H
