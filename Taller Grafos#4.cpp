#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stdexcept>
#include <string>
#include <limits>
#include <algorithm>



using IdArticulo = int;

// Clase MapaCitaciones que maneja las relaciones de citaciones entre artículos
class MapaCitaciones {
private:
    // Mapa de artículo origen a vector de artículos citados (aristas dirigidas)
    std::map<IdArticulo, std::vector<IdArticulo>> citacionesRealizadasPor;

    // Mapa de artículo a los que lo citan (inversa para acceso rápido)
    std::map<IdArticulo, std::vector<IdArticulo>> citadoPor;

public:
    // Agrega un artículo (sin citaciones)
    void agregarArticulo(const IdArticulo& id) {
        if (citacionesRealizadasPor.find(id) == citacionesRealizadasPor.end()) {
            citacionesRealizadasPor[id] = {};
        }
        if (citadoPor.find(id) == citadoPor.end()) {
            citadoPor[id] = {};
        }
    }

    // Agrega una citación del artículo origen al destino
    void agregarCitacion(const IdArticulo& origen, const IdArticulo& destino) {
        agregarArticulo(origen);
        agregarArticulo(destino);

        // Evitar duplicados simples
        auto& citados = citacionesRealizadasPor[origen];
        if (std::find(citados.begin(), citados.end(), destino) == citados.end()) {
            citados.push_back(destino);
            citadoPor[destino].push_back(origen);
        }
    }

    // Verifica si un artículo existe
    bool existeArticulo(const IdArticulo& id) const {
        return citacionesRealizadasPor.find(id) != citacionesRealizadasPor.end();
    }

    // Elimina un artículo y todas sus conexiones
    void eliminarArticulo(const IdArticulo& id) {
        if (!existeArticulo(id)) return;

        // Eliminar de citacionesRealizadasPor
        citacionesRealizadasPor.erase(id);

        // Eliminar de citadoPor
        citadoPor.erase(id);

        // Remover id de las listas de citaciones en otros artículos
        for (auto& [origen, vec] : citacionesRealizadasPor) {
            vec.erase(std::remove(vec.begin(), vec.end(), id), vec.end());
        }
        for (auto& [destino, vec] : citadoPor) {
            vec.erase(std::remove(vec.begin(), vec.end(), id), vec.end());
        }
    }

    // Obtener todos los IDs de artículos
    std::vector<IdArticulo> obtenerTodosLosArticulosIds() const {
        std::vector<IdArticulo> res;
        for (const auto& p : citacionesRealizadasPor) {
            res.push_back(p.first);
        }
        return res;
    }

    // Contar cuántos artículos citan al artículo dado
    int contarCitacionesDirectasRecibidas(const IdArticulo& id) const {
        if (!existeArticulo(id)) return 0;
        return (int)citadoPor.at(id).size();
    }

    // Contar cuántos artículos cita el artículo dado
    int contarCitacionesDirectasRealizadasPor(const IdArticulo& id) const {
        if (!existeArticulo(id)) return 0;
        return (int)citacionesRealizadasPor.at(id).size();
    }

    // Obtener vector de artículos citados por un artículo dado
    std::vector<IdArticulo> obtenerCitacionesRealizadasPor(const IdArticulo& id) const {
        if (!existeArticulo(id)) return {};
        return citacionesRealizadasPor.at(id);
    }

    // Obtener vector de artículos que citan al artículo dado
    std::vector<IdArticulo> obtenerCitadoPor(const IdArticulo& id) const {
        if (!existeArticulo(id)) return {};
        return citadoPor.at(id);
    }
};


// Funciones auxiliares y principales (como antes, pero con uso de esta clase)

IdArticulo encontrarArticuloMasCitado(const MapaCitaciones& mc) {
    std::vector<IdArticulo> todosLosArticulos = mc.obtenerTodosLosArticulosIds();
    IdArticulo idArticuloMasCitado = -1;
    int maxNumeroCitaciones = -1;

    if (todosLosArticulos.empty()) {
        return idArticuloMasCitado;
    }

    for (const IdArticulo& idActual : todosLosArticulos) {
        int numeroCitacionesActual = mc.contarCitacionesDirectasRecibidas(idActual);
        if (numeroCitacionesActual > maxNumeroCitaciones) {
            maxNumeroCitaciones = numeroCitacionesActual;
            idArticuloMasCitado = idActual;
        }
    }
    return idArticuloMasCitado;
}

void _bfsParaComponentesDebiles(const MapaCitaciones& mapa,
                               const IdArticulo& articuloInicio,
                               std::set<IdArticulo>& visitados) {
    std::queue<IdArticulo> cola;

    if (!mapa.existeArticulo(articuloInicio) || visitados.count(articuloInicio)) {
        return;
    }

    cola.push(articuloInicio);
    visitados.insert(articuloInicio);

    while (!cola.empty()) {
        IdArticulo actual = cola.front();
        cola.pop();

        std::vector<IdArticulo> citados = mapa.obtenerCitacionesRealizadasPor(actual);
        for (const IdArticulo& vecino : citados) {
            if (mapa.existeArticulo(vecino) && visitados.count(vecino) == 0) {
                cola.push(vecino);
                visitados.insert(vecino);
            }
        }

        std::vector<IdArticulo> citadores = mapa.obtenerCitadoPor(actual);
        for (const IdArticulo& vecino : citadores) {
            if (mapa.existeArticulo(vecino) && visitados.count(vecino) == 0) {
                cola.push(vecino);
                visitados.insert(vecino);
            }
        }
    }
}

int contarGruposTrasRemocion(MapaCitaciones& mapa, const IdArticulo& idArticuloInteres) {
    if (mapa.existeArticulo(idArticuloInteres)) {
        mapa.eliminarArticulo(idArticuloInteres);
    }

    std::set<IdArticulo> visitados;
    int numeroDeGrupos = 0;

    std::vector<IdArticulo> articulosRestantes = mapa.obtenerTodosLosArticulosIds();

    for (const IdArticulo& articulo : articulosRestantes) {
        if (visitados.count(articulo) == 0) {
            numeroDeGrupos++;
            _bfsParaComponentesDebiles(mapa, articulo, visitados);
        }
    }

    return numeroDeGrupos;
}

double calcularIndiceDeReferenciacion(const MapaCitaciones& mapa, const IdArticulo& idArticuloDado) {
    if (!mapa.existeArticulo(idArticuloDado)) {
        throw std::invalid_argument("El artículo con ID proporcionado no existe en el mapa.");
    }

    int cantidadCitanAlArticulo = mapa.contarCitacionesDirectasRecibidas(idArticuloDado);
    int cantidadCitadosPorElArticulo = mapa.contarCitacionesDirectasRealizadasPor(idArticuloDado);

    if (cantidadCitadosPorElArticulo == 0) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double denominador = 0.5 * static_cast<double>(cantidadCitadosPorElArticulo);
    double indice = static_cast<double>(cantidadCitanAlArticulo) / denominador;

    return indice;
}

int contarCitacionesIndirectasRealizadas(const MapaCitaciones& mapa, const IdArticulo& idArticuloOrigen) {
    if (!mapa.existeArticulo(idArticuloOrigen)) {
        throw std::invalid_argument("El artículo de origen con el ID proporcionado no existe en el mapa.");
    }

    std::vector<IdArticulo> directamenteCitadosPorOrigenVec = mapa.obtenerCitacionesRealizadasPor(idArticuloOrigen);
    std::set<IdArticulo> directamenteCitadosPorOrigenSet(
        directamenteCitadosPorOrigenVec.begin(),
        directamenteCitadosPorOrigenVec.end()
    );

    std::set<IdArticulo> citacionesIndirectasEncontradas;

    for (const IdArticulo& idArticuloIntermedio : directamenteCitadosPorOrigenVec) {
        if (!mapa.existeArticulo(idArticuloIntermedio)) {
            continue;
        }

        std::vector<IdArticulo> citadosPorIntermedioVec = mapa.obtenerCitacionesRealizadasPor(idArticuloIntermedio);

        for (const IdArticulo& idArticuloPotencialIndirecto : citadosPorIntermedioVec) {
            if (!mapa.existeArticulo(idArticuloPotencialIndirecto)) {
                continue;
            }

            bool noEsElOrigen = (idArticuloPotencialIndirecto != idArticuloOrigen);
            bool noEsCitadoDirectamentePorOrigen =
                (directamenteCitadosPorOrigenSet.find(idArticuloPotencialIndirecto) == directamenteCitadosPorOrigenSet.end());

            if (noEsElOrigen && noEsCitadoDirectamentePorOrigen) {
                citacionesIndirectasEncontradas.insert(idArticuloPotencialIndirecto);
            }
        }
    }

    return static_cast<int>(citacionesIndirectasEncontradas.size());
}

// Ejemplo básico de uso
int main() {
    MapaCitaciones mapa;

    // Agregar artículos y citaciones ejemplo
    mapa.agregarCitacion(1, 2);
    mapa.agregarCitacion(1, 3);
    mapa.agregarCitacion(3, 4);
    mapa.agregarCitacion(2, 4);
    mapa.agregarCitacion(5, 1);

    std::cout << "Articulo mas citado: " << encontrarArticuloMasCitado(mapa) << "\n";

    MapaCitaciones copiaMapa = mapa;  // Para que la eliminación no afecte el original
    int grupos = contarGruposTrasRemocion(copiaMapa, 1);
    std::cout << "Grupos tras eliminar articulo 1: " << grupos << "\n";

    try {
        double indice = calcularIndiceDeReferenciacion(mapa, 1);
        std::cout << "Indice de referenciacion del articulo 1: " << indice << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    try {
        int indirectas = contarCitacionesIndirectasRealizadas(mapa, 1);
        std::cout << "Citaciones indirectas realizadas por articulo 1: " << indirectas << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
