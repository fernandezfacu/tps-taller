# TP 1: Sockets - Taller de Programación (Cátedra Veiga)

## Setup

Instalar pre-commit hooks:
```shell
pip install pre-commit
pre-commit install
```

Para crear un commit sin correr los hooks se debe ejecutar:
```shell
git commit --no-verify
```

Para correr los hooks sin necesidad de un commit se debe ejecutar:
```shell
pre-commit run --hook-stage manual --files xx.cpp
```

(debe reemplazarse `xxx.cpp` por el o los archivos .h y .cpp).

## Compilación

Compilación con código:
```shell
make -f MakefileSockets
```

Compilación con código con wrap randomizado de sockets:
```shell
make -f MakefileSockets wrapsocks=1
```

## Ejecución

Correr tests sin valgrind:
```shell
./run_tests.sh ./ casos/ single-client no-valgrind 60 10
```

Correr tests con valgrind:
```shell
./run_tests.sh ./ casos/ single-client valgrind 60 10
```
(es posible que sea necesario incrementar esos los números, el primero es el primer timeout de un test y si el programa está colgado y no responde, el segundo timeout entra en acción y hace un kill al programa).

Comparar outputs:
```shell
./compare_outputs.sh casos/ salidas/
```

(dentro de casos/ están los archivos iniciales que se requieren para ejecutar el test y las salidas obtenidas de las ejecuciones, y en salidas/ están las salidas esperadas).

## Código fuente utilizado

Se hace uso de código provisto por la cátedra para sockets.
Código fuente usado bajo licencia GPL v2: https://github.com/eldipa/hands-on-sockets-in-cpp