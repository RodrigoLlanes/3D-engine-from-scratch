# Motor gráfico 3D desde zero

Código de la serie de directos (y proximamente videos) en los que desarrollamos un motor gráfico 3D desde zero.

## Dependencias

Únicamente empleamos la librería SDL2 para dibujar pixeles en pantalla, por lo que se puede sustituir esta librería por cualquier otra que disponga de la misma funcionalidad.

## Características implementadas

- Dibujo de triángulos.
  - Rasterización mediante Edge-Walking con el algoritmo de Brasenham.
  - Interpolación de color por vertice (por coordenadas baricentricas).
- Buffer de profundidad.
- OIT (Order-independent transparency), renderizado de transparencias empleando el algoritmo simple.

## Advertencias

Todo se está implementando para correr en CPU y sin paralelismo. No se pretende implementar un motor eficiente para usar en videojuegos comerciales, sino aprender como funcionan los motores gráficos por detras y algunos de los algoritmos más empleados.

Cualquier propuesta de mejora o idea de nueva implementación podeis dejarla en el canal de YouTube o en algun directo de Twitch.
