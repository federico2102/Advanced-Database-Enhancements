#ifndef STRING_MAP_STRING_MAP_H
#define STRING_MAP_STRING_MAP_H

#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::pair;

/**
 * Implementacion de map<string,T> sobre Trie
 * Asume de T:
 * - tiene constructor por copia (con complejidad copy(T))
 * - tiene operador == (con complejidad cmp(T))
 * - solo permite utilizar el operator[] si T tiene constructor por defecto
 */
template < typename T >
class string_map {
public:
    typedef string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef size_t size_type;


    class iterator;
    class const_iterator;


    /** @brief Construye mapa vacio
     *
     * \complexity{\O(1)}
     */
    string_map();

    /** @brief Destruye mapa
     *
     * \complexity{\O(sn * S)}
     */
    ~string_map();

    /** @brief Constructor por copia
     *
     * \complexity{\O(sn * S * copy(T))}
     */
    string_map(const string_map &);

    /** @brief Operador de asignacion
     *
     * \complexity{\O(sn * S * copy(T))}
     */
    string_map& operator=(const string_map &);

    /** @brief Operadores de comparacion
     *
     * \complexity{\O(sn * S * cmp(T))}
     */
    bool operator==(const string_map& otro) const;
    bool operator!=(const string_map& otro) const {return !(*this == otro);}

    /** @brief Cantidad de apariciones de la clave (0 o 1)
     *  @param key clave a buscar
     *
     *  \complexity{\O(S)}
     */
    size_type count(const key_type &key) const;

    /** @brief Devuelve cantidad de claves definidas */
    size_t size() const;

    /** @brief devuelve true si size() == 0 */
    bool empty() const;


    /** @brief Acceso / definición de pares clave/valor
     *  @param key clave a acceder, si no existe, se crea
     *  @returns una referencia a la definicion.
     *
     *  \complexity{\O(S)}
     */
    mapped_type &operator[](const key_type &key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia a la definicion.
     *
     *  \complexity{\O(S)}
     */
    mapped_type& at(const key_type& key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia const a la definicion.
     *
     *  \complexity{\O(S)}
     */
    const mapped_type& at(const key_type& key) const;

    /** @brief Vacia el mapa */
    void clear();

    // Accesos con iteradores

    /** @brief iterador al primer par <clave,significado> en orden lexicografico
     *  @returns iterador al elemento o end() si el mapa era vacio
     *
     *  \complexity{\O(S)}
     */
   iterator begin();

    /** @brief iterador al fin de la coleccion
     *
     *  \complexity{\O(S)}
     */
   iterator end();

    /// Versiones const de begin/end
   const_iterator begin() const;
   const_iterator end() const;
   const_iterator cbegin() const;
   const_iterator cend() const;

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador al par <clave, significado>
     *
     *  \complexity{\O(S)}
     */
   iterator find(const key_type &key);

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador const al par <clave, significado>
     *
     *  \complexity{\O(S)}
     */
   const_iterator find(const key_type &key) const;

    /** @brief insercion
     *
     * @param value par <clave,significado> a insertar
     * @returns un par con un iterador al par clave-significado agregado o
     * modificado y un bool que indica si la clave se insertó como una clave
     * nueva.
     *
     * \complexity{\O(S + copy(T))}
     */
    pair<iterator,bool> insert(const value_type &value);

    /** @brief eliminar una clave
     *  @param key clave a eliminar
     *  @returns cantidad de elementos eliminados
     *
     *  \complexity{\O(S)}
     */
    size_type erase(const key_type& key);

    /** @brief eliminar una clave mediante iterador
     *  @param pos iterador apuntando a clave a eliminar
     *  @returns iterador apuntando el proximo de la clave eliminada (o end() si era la ultima)
     *
     *  \complexity{\O(S)}
     */
    iterator erase(iterator pos);

private:

    friend class iterator;
    friend class const_iterator;

    struct Nodo {
        Nodo* padre;
        Nodo** siguientes;
        T* definicion;

        Nodo(Nodo* parent) {
            padre = parent;
            siguientes = new Nodo*[256];
            for (int i = 0; i < 256; i++) {
                siguientes[i] = nullptr;
            }
            definicion = nullptr;
        }

        bool operator==(const Nodo &otro) const {
            bool ret;

            if(definicion != nullptr && otro.definicion != nullptr)
                ret = (*definicion == *otro.definicion);
            else
                ret = (definicion == otro.definicion);

            for (int i = 0; i < 256 && ret; i++) {
                if(siguientes[i] != nullptr && otro.siguientes[i] != nullptr)
                    ret = (*siguientes[i] == *otro.siguientes[i]);
                else
                    ret = (siguientes[i] == otro.siguientes[i]);
            }

            return ret;
        }

        ~Nodo() {
            delete definicion;
            delete [] siguientes;
        }
    };

    Nodo* raiz;
    unsigned cant_claves;
    pair<string, Nodo*> menorClave;
    pair<string, Nodo*> mayorClave;

    /******** ******** FUNCIONES AUXILIARES ******** ********/

    void completarTrie(Nodo* actual, const Nodo* otroActual);
    void destruirTodo(Nodo* actual);

    /**
     * @brief Checkea si la clave indicada la mayor o menor al Dic.
     *
     * pre: key y acutal pertenecen al trie
     *
     * @param key
     * @param actual
     */
    void CheckEsMenorOMayorClave(const key_type &key, Nodo *actual);

    /**
     * @brief Obtiene el nodo al que apunta la clave key
     *
     * pre: true (key puede no estar en el trie, entonces lo que devuelve es nullptr)
     *
     * @param key
     * @returns Nodo
     */
    Nodo* obtener(const key_type &key) const;

    /**
     *
     * @param key
     * @return Nodo*
     */
    Nodo* obtenerOCrear(const key_type &key);

};

#include "string_map_iterators.h"
#include "string_map.hpp"


#endif //STRING_MAP_STRING_MAP_H
