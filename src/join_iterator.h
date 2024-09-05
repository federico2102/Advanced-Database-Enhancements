//
// Created by Federico Hofmann on 10/21/17.
//

#ifndef TP2_JOIN_ITERATOR_H
#define TP2_JOIN_ITERATOR_H

#include "BaseDeDatos.h"
#include <iostream>

using namespace std;

class BaseDeDatos::join_iterator {
public:
    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(1)}
     */
    //join_iterator(const typename BaseDeDatos::join_iterator& otro);

    /**
     * @brief Avanza el iterador una posición.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
     * siguiente.
     *
     * \complexity{\O(n * [L + log(m)])}
     */
    BaseDeDatos::join_iterator &operator++();

    /**
     * @brief Desreferencia el puntero
     *
     * El valor devuelto tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es una referencia al valor apuntado.
     *
     * \complexity{\O(copy(Registro)}
     */
    const Registro &operator*() const;

    /**
   * @brief Comparación entre iteradores
   *
   * \pre ambos iteradores refieren a la misma colección
   * \post true sii los iteradores apuntan al mismo elemento
   *
   * \complexity{\O(C+C')} con C y C' cantidad de campos en dos registros de
   * dos tablas distintas.
   */
    bool operator==(const BaseDeDatos::join_iterator &other) const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores no apuntan al mismo elemento
     *
     * \complexity{\O(C+C')}
     */
    bool operator!=(const BaseDeDatos::join_iterator &other) const;

private:
    friend class BaseDeDatos;

    conjunto_avl<DatoIndice>* _datosTabla2;
    Tabla::const_iterador_registros _registroTabla1;
    Tabla::const_iterador_registros _registrosTabla1_end;
    Tabla::const_iterador_registros _registroTabla2;
    string campo;

    join_iterator(Tabla::const_iterador_registros &registrosBegin, Tabla::const_iterador_registros &end,
                  conjunto_avl<DatoIndice>* conjDatos_2, const string& c) : _datosTabla2(conjDatos_2),
                    _registroTabla1(registrosBegin), _registroTabla2(registrosBegin), _registrosTabla1_end(end),
                    campo(c){

        actualizarPosiciones();
    }

    void actualizarPosiciones();
    const Registro &combinarRegistros() const;
};

#endif //TP2_JOIN_ITERATOR_H
