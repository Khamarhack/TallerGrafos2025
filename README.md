# TallerGrafos2025
🧠 Sistema de Gestión de Citaciones Académicas
Este proyecto, desarrollado en C++, implementa una solución para gestionar y analizar las relaciones de citación entre artículos científicos. Modela las citaciones como un grafo dirigido, donde cada nodo representa un artículo y cada arista representa una citación desde un artículo hacia otro.

📚 Propósito
En la investigación académica, los artículos se citan mutuamente para respaldar resultados y construir el estado del arte. Este sistema permite representar esas conexiones de forma estructurada, facilitando el análisis de la red de citaciones y su dinámica.

🔧 Funcionalidades principales
El sistema permite realizar diversas operaciones clave, entre ellas:

Identificar el artículo más citado: Encuentra el artículo con mayor número de citaciones directas recibidas.

Contar componentes tras eliminar un artículo: Evalúa cuántos grupos de artículos interconectados quedan en el grafo al remover un nodo.

Calcular el índice de referenciación: Métrica que relaciona el número de citaciones recibidas con la mitad de las citaciones hechas por ese artículo.

Contar citaciones indirectas: Determina cuántos artículos son citados indirectamente desde un artículo dado (a través de cadenas de citaciones).

🛠 Detalles de implementación
Se define un tipo IdArticulo para identificar artículos.

La clase MapaCitaciones gestiona el grafo de citaciones mediante estructuras de datos eficientes.

Se utilizan algoritmos basados en búsqueda en amplitud (BFS) para explorar componentes conexos y trayectorias de citación.

▶️ Instrucciones de uso
Compilación:
Asegúrate de utilizar un compilador compatible con C++11 o superior:

bash
Copiar
Editar
g++ -std=c++11 main.cpp -o mapa_citaciones
Ejecución:

bash
Copiar
Editar
./mapa_citaciones
Personalización:
Puedes editar el archivo main.cpp para agregar artículos y relaciones manualmente, o adaptar el código para cargar datos desde archivos externos.

👤 Autor
Santiago Camargo Trujillo
