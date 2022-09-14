# G1-Compiladores

# Dependencies
Install Docker in your machine. If you are on Linux based OS, just run:

> sudo apt install docker.io

# Execution

To run the compiler you must follow these commands

> docker run -it alexandremitsurukaihara/compiladores:simulator

If this is your first time executing, then the Docker image will be downloaded from the Docker Hub repository. Then, finished the download, execute:

```
git clone https://github.com/titofrota/G1-Compiladores.git
cd G1-Compiladores
make all
cd src
python3 compiler.py ../tests/fat.tiny

```

To clean all the files created by Make, run:

```
    make clean
```

# Create the Simulator

> cd simulator

> docker build --tag=CONTAINER_NAME .
