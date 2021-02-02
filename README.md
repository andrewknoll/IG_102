# Path Tracer
El objetivo de este proyecto es implementar un path tracer utilizando las clases y funciones desarrolladas en prácticas anteriores. Para ello, se van a utilizar los siguientes módulos:
  - Color: Define e implementa los diferentes ToneMappers y otras clases relacionadas con la tonalidad  de la imagen.
  - Imaging: utilizado para representar una imagen como una matriz de tuplas RGB.
  - Materials: utilizado para representar un material con sus características concretas.
  - Shapes: utilizado para representar figuras geométricas utilizadas en las escenas.
  - Vfield: Contiene clases y funciones que permiten representar vectores y puntos en el espacio.
  - Camera: representa una cámara pinhole en una escena.
  - LightPoint y AreaLigth: utilizado para representar luces puntuales y luces de área.
  - Ray: utilizado para representar un rayo.
  - Scene: utilizado para representar una escena que contiene geometrías, luces y una cámara.

### Compilación
Para la compilación de este proyecto se facilita un fichero Makefile que automatiza este proceso. Utilizando el siguiente comando se generará el fichero ejecutable del programa:

```sh
$ make
```
Si se desea recompilar el programa será necesario borrar antes los ficheros generados en la anterior compilación:

```sh
$ make clean
```

### Ejecución
El programa principal main consta de 8 parámetros de entrada opcionales que deben ser introducidos del siguiente modo: 
```sh
$ .\main -<nombre del parámetro> [<valor/es del parámetro>]
```
| Parámetro | Descripción | Valor por defecto | Rango |
| :-------: | ----------- | :---------------: | :---: |
| output-name | Nombre del fichero de salida donde se almacena la imagen resultante. | output.ppm, output.bmp | Cadena de caracteres |
| width | Anchura en píxeles de la imagen. | 400 | Z^+^ |
| heigth | Altura en píxeles de la imagen. | 400 | Z^+^ |
| scene | Número de la escena a procesar. | 2 | {1, 2, 3} |
| rpp | Número de rayos por pixel | 10 | Z^+^ |
| tone-mapping | Tipo de tone-mapping que se aplica a la imagen resultante | 5 (Reinhard 2002) | {0, 1, 2, 3, 4, 5, 6} |
| bmp | Se utiliza para especificar el formato del fichero de salida como bmp | false |  |
| color-resolution | Número de tonalidades posibles de un píxel | 255 | Z^+^ |

El parámetro tone-mapping especifica que tipo de filtros aplicar sobre la imagen resultante del Path Tracer:
  1) Clamp: los valores por encima del valor máximo (1.0) se establecen a 1.
  2) Equalize: Normaliza los valores (de 0 a 1).
  3) Equalize and Clamp: necesita un parámetro adicional en la entrada que indique el valor límite a partir del cual clampear.
  4) Gamma: necesita un parámetro adicional en la entrada con el cual se realiza la corrección gamma.
  5) Gamma and Clamp: necesita dos parámetros adicionales, uno para realizar el clampeado (valor límite) y otro para la corrección gamma.
  6) Reinhard 2002: $$\frac{valor}{1 + valor}$$
  7) Reinhard 2005: $$\frac{valor * (1 + \frac{valor}{max^2})} {1 + f}$$

El parámetro bmp solo es necesario dar como entrada el nombre del parámetro, sin dar ningún valor.
