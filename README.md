# Motor gráfico 3D desde zero

Código de la serie de directos (y próximamente videos) en los que desarrollamos un motor gráfico 3D desde zero.


## Características implementadas

- Dibujo de triángulos.
  - Rasterización mediante Edge-Walking con el algoritmo de Brasenham.
  - Interpolación de color por vértice (por coordenadas baricéntricas).
- Buffer de profundidad.
- OIT (Order-independent transparency), renderizado de transparencias empleando el algoritmo simple.


## Dependencias

- [SDL2](https://www.libsdl.org) (Únicamente para dibujar pixeles en pantalla).
- [Cabecera `stb_image.h`](https://github.com/nothings/stb/tree/master) (Para la carga de imágenes).
- [Eigen3](https://eigen.tuxfamily.org/index.php?title=Main_Page) (Para el cálculo vectorial).


## Otros recursos de terceros

- [Dungeon Tileset 32x32 px](https://stealthix.itch.io/dungeon-tileset-32x32-px) Tileset empleado para ejemplos y tests.


## Advertencias

- Todo se está implementando para correr en CPU y sin paralelismo. No se pretende implementar un motor eficiente para usar 
en videojuegos comerciales, sino aprender como funcionan los motores gráficos por detrás y algunos de los algoritmos más 
empleados.

- Cualquier propuesta de mejora o idea de nueva implementación podéis dejarla en el canal de YouTube o en algún directo de 
Twitch.

- La idea del proyecto es implementar todo lo posible desde cero, empleando la menor cantidad de librerías posible, por lo
que en futuras versiones se intentará reducir el número de dependencias implementando sus funcionalidades.
