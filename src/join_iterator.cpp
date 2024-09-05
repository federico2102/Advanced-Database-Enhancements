//
// Created by Federico Hofmann on 10/22/17.
//

#include "join_iterator.h"

/*BaseDeDatos::join_iterator::join_iterator(const typename BaseDeDatos::join_iterator &otro) {
    _datosTabla2 = otro._datosTabla2;
    _posicionTabla2 = otro._posicionTabla2;
    _registroTabla1 = otro._registroTabla1;
    _registrosTabla1_end = otro._registrosTabla1_end;
    _registroTabla2 = otro._registroTabla2;
}*/

BaseDeDatos::join_iterator &BaseDeDatos::join_iterator::operator++() {
    ++_registroTabla2;
    //Me fijo si termine de recorrer los registros del dato de la tabla 2
    if(_registroTabla2 == _registrosTabla1_end and
       _registroTabla1 != _registrosTabla1_end){
        //Si termine, entonces paso al siguiente registro de la tabla 1
        ++_registroTabla1;
        actualizarPosiciones();
    }
    return *this;
}

const Registro &BaseDeDatos::join_iterator::operator*() const {
    return combinarRegistros();
}

void BaseDeDatos::join_iterator::actualizarPosiciones() {
    while(_registroTabla1 != _registrosTabla1_end){
        DatoIndice _dato = DatoIndice(_registroTabla1->dato(campo), _registroTabla1);
        if(_datosTabla2->pertenece(_dato)){
            _registroTabla2 = *_datosTabla2->obtener(_dato)._registros.begin();
            break;
        }
        ++_registroTabla1;
    }

    //Si encontre un dato que coincide en la tabla2 entonces seteo el registro2
    // como el primero de los registros de ese dato
    if(_registroTabla1 == _registrosTabla1_end)
        _registroTabla2 = _registrosTabla1_end;
}

const Registro &BaseDeDatos::join_iterator::combinarRegistros() const {
    vector<string> campos;
    vector<Dato> datos;
    auto camposReg1 = (*_registroTabla1).campos();
    auto datosReg1 = *_registroTabla1;
    auto camposReg2 = (*_registroTabla2).campos();
    auto datosReg2 = *_registroTabla1;

    for(auto it=camposReg1.begin(); it != camposReg1.end(); ++it){
        campos.push_back(*it);
        datos.push_back(datosReg1.dato(*it));
    }

    for(auto it=camposReg2.begin(); it != camposReg2.end(); ++it){
        campos.push_back(*it);
        datos.push_back(datosReg2.dato(*it));
    }

    return Registro(campos,datos);
}

bool BaseDeDatos::join_iterator::operator==(const BaseDeDatos::join_iterator &otro) const {

   // return *_registroTabla1 == *otro._registroTabla1 and
   //        *_registroTabla2 == *otro._registroTabla2;
    return true;
}

bool BaseDeDatos::join_iterator::operator!=(const BaseDeDatos::join_iterator &otro) const {
    return not (*this == otro);
}
