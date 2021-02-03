# Photon Mapping
El objetivo de este proyecto es implementar un algoritmo de Photon Mapping partiendo como base del código proporcionado. Es un algoritmo en dos pasos: el primer paso de preprocesado donde se generan los fotones desde las fuentes de luz, y un segundo paso en el que se estima la luz indirecta utilizando Ray Tracing.

### Compilación
Para la compilación del proyecto, se proporciona un fichero script *buildpm.sh* en el directorio Ux:

```sh
$ cd Ux
$ ./buildpm.sh
```
Como resultado se generará un fichero ejecutable *smallpm*.
### Ejecución
El programa principal main consta de 10 parámetros de entrada opcionales que deben ser introducidos del siguiente modo: 
```sh
$ .\smallpm -<nombre del parámetro> <valor del parámetro>
```
| Parámetro | Descripción | Valor por defecto | Rango |
| :-------: | ----------- | :---------------: | :---: |
| film-name | Nombre del fichero donde se almacena el resultado | name_file | Cadena de caracteres |
| film-size-x | Anchura de la escena | 512 | Z^+^ |
| film-size-y | Altura de la escena | 512 | Z^+^ |
| scene | Número de la escena a procesar. | 0 | Z |
| pm-total-photons | Número total de fotones( | 10000 | Z^+^ |
| pm-photons-global | Número de fotones globales(difusos). | 10000 | Z^+^ |
| pm-phtotons-caustic | Número de fotones caústicos. | 10000 | Z^+^ |
| pm-max-photons-shot | Número fotones máximo por fuente de luz. | 100000 | {1, 2, 3} |
| pm-nb-nearest-neighbor | Número de vecinos más cercanos a buscar  | 10 | Z^+^ |
| pm-no-direct-illumination | Indica que no se va a utilizar iluminación directa |  |  |

El parámetro scene tiene 5 posibles escenas, las cuatro primeras escenas se corresponden con su número y por defecto(si se escoge cualquier otro valor) se utiliza la quinta escena.

El parámetro pm-no-direct-illumination solo es necesario dar como entrada el nombre del parámetro, sin dar ningún valor.
