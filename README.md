# *Sudoku 9x9 Solver*
---
#### Materia: *Análisis y Diseño de Algoritmos (TC2017)*

#### Semestre: *Otoño 2019*

##### Campus: *Santa Fe*

##### Integrantes:
1. *Daniela Vignau León* *A01021698*
2. *Allan Sánchez Iparrazar* *A01379951*

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

“Sudoku (en japonés: 数独, sūdoku) es un juego matemático que se inventó a finales de la década de 1970, adquirió popularidad en Japón en la década de 1980 y se dio a conocer en el ámbito internacional en 2005 cuando numerosos periódicos empezaron a publicarlo en su sección de pasatiempos.” (Wikipedia, 2019)

Sudoku es un juego de un solo jugador donde se tienen n x n celdas y algunas de ellas están llenadas con un número del 1 a n. Para ganar, se tienen que llenar las celdas que están vacías siguiendo ciertas reglas:

1. Cada número debe aparecer una sola vez por renglón
2. Cada número debe aparecer una sola vez por columna
3. Cada número debe aparecer una sola vez por *subgrid*
   - Por subgrid se puede pensar como un sudoku individual (debe de seguir las mismas reglas) pero de menor tamaño. Por ejemplo, en un Sudoko de 9x9, existen subgirds de 3x3

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

![SudokuExplained](/docs/1.png)

Por ejemplo: Dada la casilla 10
- ¿A qué grupos pertence?: 1 (renglón), 10, (columna), 22 (casilla)
- ¿Vecinos de casilla #10?: 0, 1, 2, 9, 11, 12, 13, 14, 15, 16, 17, 28, 37, 46, 55, 64, 72

3. Las opraciones a realizar son:
   - **Assign(int *k*, int *val*):** Elimina a los demás valores de la celda (*k*) que no son el valor dado (*val*)
   - **DeleteVal(int *k*, int *val*):** Eliminar *v* de los posibles valores de la celda *k*. Existen tres posibilidades para eliminar:
        - Si hay 0 valores posibles, entonces existe un error y se regresa un False para descartar la rama
        - Si hay 1 valor posible, entonces ese es el valor (*v2*) que va en la celda (*k*). Para esto, se deberá eliminar el valor *v2* de todos sus vecinos para evitar repetidos
        - Si al haber eliminado una posibilidad al asignar un valor en cierta casilla y solo hay otro lugar donde se puede asignar el valor restante, se asigna en la nueva casilla

4. Un árbol de posibilidades se irá creando conforme se realicen diferentes operaciones; en el árbol se propagarán las restricciones de lo que se le indicó en un inicio, y si en algún punto una rama no cumple con las restricciones, se regresará un False para indicar que por ese camino no es y se descarta la rama completa.

Al compilar un cierto Sudoku con el programa como se encuentra en este momento, podría llegar a funcionar si, sólo si, existe una sola posibilidad para cada celda. De lo contrario se deberá de añadir lo siguiente: 
1. Se creará una función que escogerá un valor aleatorio dentro de las distintas posibilidades que pudiera tener una celda de un Sudoku que aún está sin resolver. Creará un nuevo “árbol” de posibilidades, que irá borrando ramas en cuanto estás dejen de cumplir con las reglas. Se deberá de empezar con las celdas que tienen menores posibilidades ya que al descartar una de sus ramas, rápidamente se descartan los valores en las demás ramas.

A todo esto, significa que un Sudoku puede ser resuelto utilizando un método de “Backtracking”, pues busca una solución y se regresa si ésta no es válida, ya que esto significa que nos hemos equivocado y busca en otra dirección utilizando otros valores.

La complejidad del algoritmo secuencial es de: O(n2) al eliminar los valores pues es ahí donde se aplican las reglas

[Codigo Secuencial](https://github.com/tec-csf/TC2017-PF-Otono-2019-equipo-algoritmos/tree/master/secuencial)

## 4. Análisis de los inhibidores del paralelismo

Al momento de querer paralelizar este problema, nos dimos cuenta de que había muchos ciclos for en los cuales se pudiera implementar la librería Open MP para sacarle provecho a los hilos de procesamiento de nuestras PC y resolver el problema en una cantidad aún menor de tiempo. 
 
Nuestro primer tope fue el darnos cuenta de que no podíamos paralelizar todos los ciclos for de la solución, sino que debíamos limitarnos a escoger los ciclos en los cuales el procesamiento se iba a ser exhausto. 
 
Notamos que una gran parte del procesamiento se concentraba en cargar el archivo, para lo cual hicimos uso de #pragma omp parallel para distribuir las columnas a través de los hilos de procesamiento y, de esta forma, lograra resolver más rápido el sudoku puzzle.


## 5. Solución paralela

Nuestra solución se centra en paralelizar la forma en la que el programa procesa la entrada para crear la matriz. Lo hacemos dividiendo el número de columnas entre el número de hilos de procesamiento disponibles y así acelerar su procesamiento. 

[Codigo Paralelo](https://github.com/tec-csf/TC2017-PF-Otono-2019-equipo-algoritmos/tree/master/paralelo)

## 6. Tabla de resultados

Para poder comparar correctamente ambos algoritmos, se utilzaron 5 diferentes sudokus de entarada, los cuales iban incrementando su dificultad. Se resolvieron 2 Sudokus, los cuales fueron propuestos por un matemático filipino, que tienen la fama de ser muy difíciles. 

![VisualSudoku](/docs/inputSudokuVisual.png)

**Secuencial**

| Dificultad de Sudoku | Tiempo de ejecución |
| :------------------: | :-----------------: |
|      Easy - 1        |    0.02 segundos    |
|      Easy - 2        |     0.0 segundos    |
|     Hard Inkala 1    |    0.002 segundos   |
|     Hard Inkala 2    |   0.005 segundos    |
|      Impossible   |    0.027 segundos    |

**Paralelo**

| Dificultad de Sudoku | Tiempo de ejecución |
| :------------------: | :-----------------: |
|      Easy - 1        |     0.00000004 segundos |
|      Easy - 2        |    0.00000005 segundos |
|     Hard Inkala 1    |    0.00000006  segundos |
|     Hard Inkala 2    |    0.00000005 segundos|
|      Impossible      |    0.00000006 Segundo's |


## 7. Gráfica(s) comparativa(s)

![Tiempos de Ejecución S vs P](/docs/SecuencialVSParalelo.png)

![VisualSudoku](/docs/NumberOfThreads.png)

*Cambiando opciones de balanceo*

![VisualSudoku](/docs/BalancingOptions.png)

*Auto*

![VisualSudoku](/docs/Auto.png)

*Guided*

![VisualSudoku](/docs/Guided.png)

*Static*

![VisualSudoku](/docs/Static.png)

*Dynamic*

![VisualSudoku](/docs/Dynamic.png)

*Average*

![VisualSudoku](/docs/Average.png)

## 8. Interpretación de los resultados

Al correr ambos programas con el mismo set de datos nos percatamos que el procesamiento en paralelo sacó ventaja de la solución secuencial. La diferencia no es tan perceptible, pues ante nuestros ojos ambas soluciones resolvían el puzzle al instante, pero sigue siendo más rápido.
 
¿Cómo se notaría más esta ventaja? La ventaja vendría cuando se tuviera que procesar una matriz nxn. Considerando n un número mucho mayor a 9, la cantidad de elementos de una matriz a procesar serían basta y es ahí donde nuestro programa sacaría la ventaja. 

## 9. Guía paso a paso

Clonamos el repositorio a la carpeta de preferencia:
 
> git clone https://github.com/tec-csf/TC2017-PF-Otono-2019-equipo-algoritmos
 
Nos posicionamos en la carpeta de nuestro repositorio:
 
> cd TC2017-PF-Otono-2019-404
 
**Para el código secuencial**
 
Nos posicionamos en la carpeta secuencial:
 
> cd secuencial
 
Compilamos nuestro programa:
 
> g++ sudoku.cpp -o sudoku
 
Para que la solución secuencial resuelva un puzzle se le tiene que indicar el archivo de texto que debe de leer por medio del siguiente comando:
 
> ./sudoku < sudokuResolver.txt
 
O bien: 
 
> ./sudoku < impossible-1.txt
 
Para que resuelva otro. 
 
Si se quiere almacenar el sudoku resuelto en un archivo de texto, se deberá de ingresar el siguiente comando: 
 
> ./sudoku < sudokuResolver.txt > sudokuResuelto.txt. 
 
 
Se han añadido distintos Sudokus por resolver que están listos para ser usados por el usuario. 


*Importante* El programa actualmente acepta dos distintos formatos de entrada de Sudoku:


1. 
![ExampleInput](/docs/exampleInputSudoku.png)

El programa podrá diferenciar y hará uso de los valores que realmente le interesan, sustituyendo el punto (.) por 0 para tomarlos como valores nulos.

2. **003020600900305001001806400008102900700000008006708200002609500800203009005010300**

El programa tomará los 0 como valores vacíos y creará las tripletas necesarias para consturir el sudoku de 9x9
    
## 10. Referencias

1. Fernandez, P. [Pau Fernandez] (2013, enero 9). *C++ Solucionador de Sudokus 1-5/5*. [Archivo de video]. Recuperado el 22 de noviembre de 2019 de: https://www.youtube.com/watch?v=p7mjDQgbKYM
2. Norvig, P. (s.f.). *Solving Every Sudoku Puzzle* Recuperado el 22 de noviembre de 2019 de: http://norvig.com/sudoku.html
3. Overmiller, D. [Dustin Overmiller] (2013, marzo 16). *Sudoku Parallel Approach* [Archivo de video]. Recuperado el 22 de noviembre de 2019 de: https://www.youtube.com/watch?v=S9ZBsgsj31w
4. Sankar, S. (2014). *Parallelized Sudoku Solving Algorithm Using OpenMP*. Recuperado el 22 de noviembre de 2019 de: https://cse.buffalo.edu/faculty/miller/Courses/CSE633/Sankar-Spring-2014-CSE633.pdf
