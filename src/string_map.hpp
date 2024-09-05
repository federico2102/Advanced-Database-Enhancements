#include "string_map.h"
#include <algorithm>

using namespace std;

template <typename T>
string_map<T>::string_map() {
    raiz = nullptr;
    cant_claves = 0;
    menorClave = make_pair("", nullptr);
    mayorClave = make_pair("", nullptr);
}

template <typename T>
string_map<T>::~string_map() {
    clear();
}

template <typename T>
string_map<T>::string_map(const string_map& s) {
    menorClave = make_pair("", nullptr);
    mayorClave = make_pair("", nullptr);

    if( s.raiz != nullptr ) {
        //Copio Trie
        raiz = new Nodo(nullptr);
        completarTrie(raiz, s.raiz);
        cant_claves = s.cant_claves;

        //Copio menorclave
        Nodo* _menor = get<1>(s.menorClave);
        if(_menor != nullptr){
            menorClave = make_pair(get<0>(s.menorClave), obtener(get<0>(s.menorClave)));
            // Usar obtener no suma complejidad ya que es menor o igual que copiar el trie
        }

        //Copio mayorClave
        Nodo* _mayor = get<1>(s.mayorClave);
        if(_mayor != nullptr){
            mayorClave = make_pair(get<0>(s.mayorClave), obtener(get<0>(s.mayorClave)));
            // Usar obtener no suma complejidad ya que es menor o igual que copiar el trie
        }

    }else{
        raiz = nullptr;
        cant_claves = 0;
    }
}

template <typename T>
typename string_map<T>::string_map &string_map<T>::operator=(const string_map& s) {
    clear();

    //TODO fijarese si se puede hacer un swap
    if( s.raiz != nullptr ) {
        //Copio Trie
        raiz = new Nodo(nullptr);
        completarTrie(raiz, s.raiz);
        cant_claves = s.cant_claves;

        //Copio menorclave
        Nodo* _menor = get<1>(s.menorClave);
        if(_menor != nullptr){
            menorClave = make_pair(get<0>(s.menorClave), obtener(get<0>(s.menorClave)));
            // Usar obtener no suma complejidad ya que es menor o igual que copiar el trie
        }

        //Copio mayorClave
        Nodo* _mayor = get<1>(s.mayorClave);
        if(_mayor != nullptr){
            mayorClave = make_pair(get<0>(s.mayorClave), obtener(get<0>(s.mayorClave)));
            // Usar obtener no suma complejidad ya que es menor o igual que copiar el trie
        }

    }

    return *this;
}

template <typename T>
bool string_map<T>::operator==(const string_map &otro) const {
    bool ret = false;

    if( cant_claves == otro.cant_claves
     && get<0>(menorClave) == get<0>(otro.menorClave)
     && get<0>(mayorClave) == get<0>(otro.mayorClave)){
        ret = (*raiz == *otro.raiz);
    }

    return ret;
}

template <typename T>
typename string_map<T>::size_type string_map<T>::count(const string_map<T>::key_type &key) const {
    Nodo* actual = obtener(key);

    if(actual != nullptr && actual->definicion != nullptr)
        return 1;
    else
        return 0;
}

template <typename T>
size_t string_map<T>::size() const {
    return cant_claves;
}

template <typename T>
bool string_map<T>::empty() const {
    return cant_claves == 0;
}

template <typename T>
typename string_map<T>::mapped_type &string_map<T>::operator[](const string_map<T>::key_type &key) {
    Nodo* actual = obtenerOCrear(key);

    if(actual->definicion == nullptr) {
        cant_claves++;

        T* def = new T();
        actual->definicion = def;

        CheckEsMenorOMayorClave(key,actual);
    }

    return *(actual->definicion);
}

template <typename T>
typename string_map<T>::mapped_type &string_map<T>::at(const string_map<T>::key_type &key) {
    Nodo* actual = obtener(key);
    return *(actual->definicion);
}

template <typename T>
const typename string_map<T>::mapped_type &string_map<T>::at(const string_map<T>::key_type &key) const {
    Nodo* actual = obtener(key);
    return *(actual->definicion);
}

template <typename T>
void string_map<T>::clear() {
    if( raiz != nullptr )
        destruirTodo(raiz);

    raiz = nullptr;
    cant_claves = 0;

    menorClave = make_pair("", nullptr);
    mayorClave = make_pair("", nullptr);
}

// Accesos con iteradores

template <typename T>
typename string_map<T>::iterator string_map<T>::begin() {
    if(get<1>(menorClave) != nullptr)
        return iterator(get<0>(menorClave), get<1>(menorClave));
    return end();
}

template <typename T>
typename string_map<T>::iterator string_map<T>::end() {
    if(get<1>(mayorClave) != nullptr)
        return iterator(get<0>(mayorClave), get<1>(mayorClave), true);
    return iterator();
}

template <typename T>
typename string_map<T>::const_iterator string_map<T>::begin() const {
    return cbegin();
}

template <typename T>
typename string_map<T>::const_iterator string_map<T>::end() const {
    return cend();
}

template <typename T>
typename string_map<T>::const_iterator string_map<T>::cbegin() const {
    if(get<1>(menorClave) != nullptr)
        return const_iterator(get<0>(menorClave),get<1>(menorClave));
    return cend();
}

template <typename T>
typename string_map<T>::const_iterator string_map<T>::cend() const {
    if(get<1>(mayorClave) != nullptr)
        return const_iterator(get<0>(mayorClave),get<1>(mayorClave),true);
    return const_iterator();
}

template <typename T>
typename string_map<T>::iterator string_map<T>::find(const string_map::key_type &key) {
    return iterator(key, obtener(key));
}

template <typename T>
typename string_map<T>::const_iterator string_map<T>::find(const string_map::key_type &key) const {
    return const_iterator(key, obtener(key));
}

template <typename T>
pair<typename string_map<T>::iterator, bool> string_map<T>::insert(const string_map::value_type &value) {
    key_type key = get<0>(value);
    Nodo* actual = obtenerOCrear(key);
    bool existia = false;

    if(actual->definicion != nullptr) {
        delete actual->definicion;
        existia = true;
    }

    T valor = get<1>(value);
    T* def = new T(valor);
    actual->definicion = def;

    if(!existia){
        cant_claves++;
        CheckEsMenorOMayorClave(key,actual);
    }

    return make_pair(iterator(key,actual), existia);
}

template <typename T>
typename string_map<T>::size_type string_map<T>::erase(const string_map<T>::key_type &key) {
    Nodo* actual = obtener(key);
    delete actual->definicion;
    actual->definicion = nullptr;
    cant_claves--;
    size_type ret = 1;


    bool borroActual = true;
    for(int i=key.size()-1; borroActual; i--){
        borroActual = (actual->definicion == nullptr);

        for(int l=0; l < 256 && borroActual; l++){
            if(actual->siguientes[l] != nullptr)
                borroActual = false;
        }

        if(borroActual) {
            ret++;
            Nodo *aBorrar = actual;

            if (actual == raiz) { // o actual->padre == nullptr
                raiz = nullptr;
                borroActual = false;
            } else {
                actual = actual->padre;
                actual->siguientes[int(key[i])] = nullptr;
            }


            Nodo* nodoMenor = get<1>(menorClave);
            if(aBorrar == nodoMenor) {
                iterator firstIt = iterator(get<0>(menorClave), get<1>(menorClave));
                ++firstIt;
                menorClave = make_pair(firstIt.key, firstIt.nodo);
            }

            Nodo* nodoMayor = get<1>(mayorClave);
            if(aBorrar == nodoMayor) {
                iterator endIt = iterator(get<0>(mayorClave), get<1>(mayorClave));
                --endIt;
                mayorClave = make_pair(endIt.key, endIt.nodo);
            }

            delete aBorrar;
        }
    }

    return ret;
}

template <typename T>
typename string_map<T>::iterator string_map<T>::erase(typename string_map<T>::iterator pos) {
    iterator it = iterator(pos.key,pos.nodo);
    ++it;
    erase(pos->first);
    if(it.pastTheEnd)
        return end();
    return it;
}

/** FUNCIONES AUXILIARES **/

template <typename T>
void string_map<T>::completarTrie(Nodo* actual, const Nodo* otroActual) {

    if( otroActual->definicion != nullptr ) {
        T* sig = new T(*(otroActual->definicion));
        actual->definicion = sig;
    }

    for(int i=0; i < 256 ; i++) {
        if( otroActual->siguientes[i] != nullptr ){
            actual->siguientes[i] = new Nodo(actual);
            completarTrie(actual->siguientes[i], otroActual->siguientes[i]);
        }
    }
}

template <typename T>
void string_map<T>::destruirTodo(Nodo* actual) {
    for(int i=0; i < 256; i++) {
        if( actual->siguientes[i] != nullptr )
            destruirTodo(actual->siguientes[i]);
    }

    delete actual;
}

template <typename T>
void string_map<T>::CheckEsMenorOMayorClave(const string_map<T>::key_type &key, string_map<T>::Nodo *actual){
    string _menor = get<0>(menorClave);
    string _mayor = get<0>(mayorClave);
    Nodo* nodo_menor = get<1>(menorClave);
    if(nodo_menor != nullptr) {
        if (key < _menor) {
            menorClave = make_pair(key, actual);//Asigno la menor clave
        }else if (key > _mayor) {
            mayorClave = make_pair(key, actual);//Asigno la mayor clave
        }
    } else {
        menorClave = make_pair(key, actual);
        mayorClave = make_pair(key, actual);
    }
}

template <typename T>
typename string_map<T>::Nodo* string_map<T>::obtener(const string_map<T>::key_type &key) const {
    Nodo* actual = raiz;
    for(int i = 0; i < key.size() && actual != nullptr ; i++)
        actual = actual->siguientes[int(key[i])];
    return actual;
}

template <typename T>
typename string_map<T>::Nodo* string_map<T>::obtenerOCrear(const string_map<T>::key_type &key) {
    if( raiz == nullptr )
        raiz = new Nodo(nullptr);

    Nodo* actual = raiz;
    for(int i = 0; i < key.size(); i++) {
        if(actual->siguientes[int(key[i])] == nullptr)
            actual->siguientes[int(key[i])] = new Nodo(actual);

        actual = actual->siguientes[int(key[i])];
    }

    return actual;
}
