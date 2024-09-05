#include "BaseDeDatos.h"
#include <list>
#include <tuple>
#include <algorithm>

BaseDeDatos::BaseDeDatos() {};

BaseDeDatos::~BaseDeDatos() {
    for(auto tabla : _indices){
        for(auto indice : tabla.second){
            delete indice.second;
        }

    }

};

void BaseDeDatos::crearTabla(const string &nombre,
                             const linear_set<string> &claves,
                             const vector<string> &campos,
                             const vector<Dato> &tipos) {
    _nombres_tablas.fast_insert(nombre);
    _tablas.insert(make_pair(nombre, Tabla(claves, campos, tipos)));
}

void BaseDeDatos::agregarRegistro(const Registro &r, const string &nombre) {
    Tabla &t = _tablas.at(nombre);
    Tabla::const_iterador_registros itReg = t.agregarRegistro(r);

    if(_indices.count(nombre)){ //O(1) porque nombre es acotado
        string_map< conjunto_avl<DatoIndice>* > tablaIndices = _indices.at(nombre);  //O(1) porque nombre es acotado
        auto itCamposInx = tablaIndices.begin();
        while(itCamposInx != tablaIndices.end()){
            string campo = itCamposInx->first;
            AgDatoIndice(tablaIndices,campo,itReg);
            ++itCamposInx;
        }

        _indices[nombre] = tablaIndices;
    }
}

const linear_set<string> &BaseDeDatos::tablas() const { return _nombres_tablas; }

const Tabla &BaseDeDatos::dameTabla(const string &nombre) const {
    return _tablas.at(nombre);
}

int BaseDeDatos::uso_criterio(const BaseDeDatos::Criterio &criterio) const {
    if (_uso_criterios.count(criterio)) {
        return _uso_criterios.at(criterio);
    } else {
        return 0;
    }
}

bool BaseDeDatos::registroValido(const Registro &r,  const string &nombre) const {
    const Tabla &t = _tablas.at(nombre);        //O(1) los nombres de las tablas son acotados
    bool ret;

    if(t.campos().size() == r.campos().size()){ //O(1)
        ret = _mismos_tipos(r,t);               //O(C)
    }else{
        ret = false;
    }

    if(ret)
        ret = _no_repite(r,t);                  //O(c * n * L)

    return ret;
}

bool BaseDeDatos::_mismos_tipos(const Registro &r, const Tabla &t) const {
    for (auto c : t.campos()) {                             //O(C)
        if (r.dato(c).esNat() != t.tipoCampo(c).esNat()) {  //O(1)
            return false;
        }
    }
    return true;
}

bool BaseDeDatos::_no_repite(const Registro &r, const Tabla &t) const {
    for(auto reg : t.registros()){                //O(n * c * L)
        int repiteClave = 0;
        for (auto clave : t.claves()) {           //O(c * L)
            if(reg.dato(clave) == r.dato(clave)){ //O(L) (O(1) si el dato es Nat, O(L) si es string)
                repiteClave++;
            }
        }
        if(repiteClave == t.claves().size())      //O(1)
            return false;
    }
    return true;
}

list<Registro> &
BaseDeDatos::_filtrar_registros(const string &campo, const Dato &valor, list<Registro> &registros) const {
    return _filtrar_registros(campo, valor, registros, true);
}

list<Registro> &BaseDeDatos::_filtrar_registros(const string &campo,
                                                const Dato &valor,
                                                list<Registro> &registros,
                                                bool igualdad) const {
    auto iter = registros.begin();
    while (iter != registros.end()) {
        auto now = iter;
        iter++;
        if ((not igualdad) xor now->dato(campo) != valor) {
            registros.erase(now);
        }
    }

    return registros;
}

pair<vector<string>, vector<Dato> > BaseDeDatos::_tipos_tabla(const Tabla &t) {
    vector<string> res_campos;
    vector<Dato> res_tipos;
    for (auto c : t.campos()) {
        res_campos.push_back(c);
        res_tipos.push_back(t.tipoCampo(c));
    }
    return make_pair(res_campos, res_tipos);
}

bool BaseDeDatos::criterioValido(const Criterio &c,
                                 const string &nombre) const {
    const Tabla &t = _tablas.at(nombre);                                                //O(1) porque los nombres de las tablas son acotados
    for (auto restriccion : c) {                                                        //O(cr)
        if (not t.pertenece(restriccion.campo())) {                                     //O(1)
            return false;
        }
        if (t.tipoCampo(restriccion.campo()).esNat() != restriccion.dato().esNat()) {   //O(1) porque los nombres de laos campos on acotados
            return false;
        }
    }
    return true;
}

Tabla BaseDeDatos::busqueda(const BaseDeDatos::Criterio &c,
                            const string &nombre) {
    if (_uso_criterios.count(c)) {
        _uso_criterios.at(c)++;
    } else {
        _uso_criterios.fast_insert(make_pair(c, 1));
    }

    const Tabla &ref = dameTabla(nombre);
    auto campos_datos = _tipos_tabla(ref);
    Tabla res(ref.claves(), campos_datos.first, campos_datos.second);
    list<Registro> regs(ref.registros().begin(), ref.registros().end());
    for (auto restriccion : c) {
        _filtrar_registros(restriccion.campo(), restriccion.dato(),
                           regs, restriccion.igual());
    }
    for (auto r : regs) {
        res.agregarRegistro(r);
    }
    return res;
}

linear_set<BaseDeDatos::Criterio> BaseDeDatos::top_criterios() const {
    linear_set<Criterio> ret;
    int max = 0;
    for (auto crit_count : _uso_criterios) {
        if (crit_count.second >= max) {
            if (crit_count.second > max) {
                ret = linear_set<Criterio>();
                max = crit_count.second;
            }
            ret.fast_insert(crit_count.first);
        }
    }
    return ret;
}

void BaseDeDatos::crearIndice(const string &nombre, const string &campo) {
    const Tabla &tabla = _tablas.at(nombre);
    string_map<conjunto_avl<DatoIndice>* > tablaIndices = string_map<conjunto_avl<DatoIndice>* >::string_map();

    //Me fijo si la tabla 'nombre' tiene una tabla de indices, sino la creo
    if (_indices.count(nombre)) {
        tablaIndices = _indices.at(nombre);
    }

    conjunto_avl<DatoIndice>* datos = new conjunto_avl<DatoIndice>(); //Creo una forma de ordenar el indice 'campo' (conjunto basado en avl)
    //Meto el nuevo indice en la tabla de indices de la tabla 'nombre'
    tablaIndices.insert(make_pair(campo, datos));

    //Si la tabla tiene registros los indexo ahora.
    Tabla::const_iterador_registros itReg = tabla.registros_begin();
    while (itReg != tabla.registros_end()) {
        AgDatoIndice(tablaIndices, campo, itReg);
        ++itReg;
    }

    _indices[nombre] = tablaIndices;

}

void BaseDeDatos::AgDatoIndice(string_map<conjunto_avl<DatoIndice>* > &tablaIndices, const string &campo ,
                               Tabla::const_iterador_registros &itReg){

    conjunto_avl<DatoIndice>* conjDatos = tablaIndices.at(campo);               //O(1) porque campo es acotado

    DatoIndice _dato = DatoIndice(itReg->dato(campo), itReg);
    if(conjDatos->pertenece(_dato)){
        DatoIndice& nuevoDato = conjDatos->obtener(_dato);
        nuevoDato.AgRegistro(itReg);
    }else{
        conjDatos->insertar(_dato);
    }
}

BaseDeDatos::join_iterator BaseDeDatos::join(const string &tabla1, const string &tabla2,
                                             const string &campo) const{
    if(_indices.count(tabla1)) {
        if(_indices.at(tabla1).count(campo)) {
            Tabla::const_iterador_registros itReg = _tablas.at(tabla2).registros_begin();
            Tabla::const_iterador_registros end = _tablas.at(tabla2).registros_end();
            return join_iterator(itReg, end, _indices.at(tabla1).at(campo), campo);
        }
    }

    //Si la tabla1 no tenia indice en "campo" entonces la tabla2 si
    Tabla::const_iterador_registros itReg = _tablas.at(tabla1).registros_begin();
    Tabla::const_iterador_registros end = _tablas.at(tabla1).registros_end();
    return join_iterator(itReg, end, _indices.at(tabla2).at(campo), campo);
}

BaseDeDatos::join_iterator BaseDeDatos::join_end() const{
    conjunto_avl<DatoIndice>* conjDatos;
    Tabla::const_iterador_registros itReg = _tablas.at(*tablas().begin()).registros_end();
    Tabla::const_iterador_registros end = _tablas.at(*tablas().begin()).registros_end();
    return join_iterator(itReg, end, conjDatos, "");
}
