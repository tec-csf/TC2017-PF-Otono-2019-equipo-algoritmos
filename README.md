# *[Poner aquí el Título del Proyecto]*
---
#### Materia: *Análisis y Diseño de Algoritmos (TC2017)*

#### Semestre: *Otoño 2019*

##### Campus: *Santa Fe*

##### Integrantes:
1. *[Poner aquí Nombre y Apellidos del integrante 1]* *[Matrícula]*
2. *[Poner aquí Nombre y Apellidos del integrante 2]* *[Matrícula]*

---
## 1. Aspectos generales

### 1.1 Requerimientos

A continuación se mencionan los requerimientos mínimos del proyecto, favor de tenerlos presente para que cumpla con todos.

* El equipo tiene la libertad de elegir el problema a resolver.
* El proyecto deberá utilizar [OpenMP](https://www.openmp.org/) para la implementación del código paralelo.
* Todo el código y la documentación del proyecto debe alojarse en este repositorio de GitHub. Favor de mantener la estructura de carpetas propuesta.

### 1.2 Estructura del repositorio
El proyecto debe seguir la siguiente estructura de carpetas:
```
- / 			        # Raíz de todo el proyecto
    - README.md			# este archivo
    - secuencial		# Carpeta con la solución secuencial
    - paralelo			# Carpeta con la solución paralela
    - docs              # Carpeta con los documentos, tablas, gráficas, imágenes
```

### 1.3 Documentación  del proyecto

Como parte de la entrega final del proyecto, se debe incluir la siguiente información:

* Descripción del problema a resolver.
* Descripción de la solución secuencial con referencia (enlace) al código secuencial alojado en la carpeta [secuencial](secuencial/).
* Análisis de los inhibidores de paralelismo presente y una explicación de cómo se solucionaron.
* Descripción de la solución paralela con referencia (enlace) al código paralelo alojado en la carpeta [paralelo](paralelo/).
* Tabla de resultados con los tiempos de ejecución medidos para cada variante de la solución secuencial vs. la solución paralela, teniendo en cuenta: 5 tamaños diferentes de las entradas, 5 opciones diferentes de números de CPU (threads), 4 ocpiones diferentes de balanceo (*auto, guided, static, dynamic*).
* Gráfica(s) comparativa(s) de los resultados obtenidos en las mediciones.
* Interpretación de los resultados obtenidos.
* Guía paso a paso para la ejecución del código y replicar los resultados obtenidos.
* El código debe estar documentado siguiendo los estándares definidos para el lenguaje de programación seleccionado.

## 2. Descripción del problema

*[Incluya aquí la descripción del problema a resolver.]*

## 3. Solución secuencial

1. Se le asignará a cada celda un índice que irá del 0 al 80, para completar, así, un Sudoku de 81 celdas (9 x 9). Las celdas, se almacenarán en un vector lineal de tamaño k.
2. Se crearán diferentes “grupos” de celdas que servirán para respetar las reglas 
   - **Grupos**: Para los renglones, columnas y casillas que deberán seguir las reglas
     - Existen 27 grupos distintos:
        - 0 - 8: grupos por renglón
        - 9 - 17: grupos por columnas
        - 18 - 26: grupo por casilla
   - **GroupsOf**: A qué grupos una cierta celda pertenece
   - **Neighbors**: Todas las celdas que están relacionadas con una celda


En la siguiente imagen, se muestra un Sudoku de 9x9 que tiene asignados los valores de las casillas, señalando en distintos colores las reglas que se deben de seguir, así como también la representación de los grupos.

Por ejemplo: Dada la casilla 10
- ¿A qué grupos pertence?: 1 (renglón), 10, (columna), 22 (casilla)
- ¿Vecinos de casilla #10?: 0, 1, 2, 9, 11, 12, 13, 14, 15, 16, 17, 28, 37, 46, 55, 64, 72

3. Las opraciones a realizar son:
   - **Assign(int *k*, int *val*):** Elimina a los demás valores de la celda (*k*) que no son el valor dado (*val*)
   - **DeleteVal(int *k*, int *val*):** Eliminar *v* de los posibles valores de la celda *k*. Existen tres posibilidades para eliminar:
        - Si hay 0 valores posibles, entonces existe un error y se regresa un False para descartar la rama
        - Si hay 1 valor posible, entonces ese es el valor (*v2*) que va en la celda (*k*). Para esto, se deberá eliminar el valor *v2* de todos sus vecinos para evitar repetidos
        - Si solo queda una celda (*k2*) en los grupos de la celda, con valor *v* (el de entrada) solo se debe de asignar en la celda *k2* el valor de *v*

4. Un árbol de posibilidades se irá creando conforme se realicen diferentes operaciones; en el árbol se propagarán las restricciones de lo que se le indicó en un inicio, y si en algún punto una rama no cumple con las restricciones, se regresará un False para indicar que por ese camino no es y se descarta la rama completa.

Al compilar un cierto Sudoku con el programa como se encuentra en este momento, podría llegar a funcionar si, sólo si, existe una sola posibilidad para cada celda. De lo contrario se deberá de añadir lo siguiente: 
1. Se creará una función que escogerá un valor aleatorio dentro de las distintas posibilidades que pudiera tener una celda de un Sudoku que aún está sin resolver. Creará un nuevo “árbol” de posibilidades, que irá borrando ramas en cuanto estás dejen de cumplir con las reglas. Se deberá de empezar con las celdas que tienen menores posibilidades ya que al descartar una de sus ramas, rápidamente se descartan los valores en las demás ramas.

A todo esto, significa que Sudoku puede ser resuelto utilizando un método de “Backtracking”, pues busca una solución y se regresa si ésta no es válida, ya que esto significa que nos hemos equivocado y busca en otra dirección utilizando otros valores.


## 4. Análisis de los inhibidores del paralelismo

*[Incluya aquí el análisis de los inhibidores presentes en su problema, así como la solución a los mismos.]*

## 5. Solución paralela

*[Incluya aquí la descripción de la solución paralela.]*

## 6. Tabla de resultados

*[Incluya aquí la tabla con los resultados de las mediciones.]*

## 7. Gráfica(s) comparativa(s)

*[Incluya aquí la(s) gráfica(s) comparativa(s).]*

## 8. Interpretación de los resultados

*[Incluya aquí la interpretación de los resultados.]*

## 9. Guía paso a paso

*[Incluya aquí la guía para la ejecución de los códigos.]*

## 10. Referencias

*[Incluya aquí las referencias a sitios de interés y cualquier otra información que haya utilizado para realizar el proyecto y que le puedan ser de utilidad a otras personas que quieran usarlo como referencia]*
