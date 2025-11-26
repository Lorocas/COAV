# Advanced Compilation - Project

C++ plugin for static verification of MPI collective function call sequences.

## Report

See the file [**`misc/report.pdf`**](misc/report.pdf).


## Slides

See the file [**`misc/slides.pdf`**](misc/slides.pdf).


## Code source

See the file [**`code/README.md`**](code/README.md).


## Docker

To use the project, you will need to download the GCC sources.
```bash
wget https://ftp.gnu.org/gnu/gcc/gcc-12.2.0/gcc-12.2.0.tar.gz
```

To start the container:
```bash
docker-compose up -d
docker exec -it coav-container bash
```

To stop the container:
```bash
docker-compose down
```


## Authors

- Lorenzo LUCAS -- ROBLOT
- Alicia PLATH
