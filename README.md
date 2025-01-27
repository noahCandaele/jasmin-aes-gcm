# Efficient and secure implementation of AES-GCM in Jasmin

Projet d'Études et de Recherche  
Université Côte d'Azur  
Autumn semester 2023-2024  

Students: Noah CANDAELE, Anthony IOZZIA (Master 2 computer science cybersecurity)  
Supervisor: Sid TOUATI  
Co-supervisors: Benjamin GRÉGOIRE, Jean-Christophe LÉCHENET  

[Official Jasmin GitHub repository](https://github.com/jasmin-lang/jasmin)  
[Official Libjade GitHub repository](https://github.com/formosa-crypto/libjade)

## Environment setup

### Classic installation

Install Jasmin (tested on Ubuntu 22.04):
- Update package repositories: `sudo apt update`
- Install packages: `sudo apt install -y gcc curl`
- Install the Nix package manager: `sh <(curl -L https://nixos.org/nix/install) --no-daemon`
- Set the necessary environment variables: `. ~/.nix-profile/etc/profile.d/nix.sh`
- Install Jasmin compiler: `nix-env -iA nixpkgs.jasmin-compiler`
- Reboot system
- Test installation: `jasminc -version`

Sources:
- [Nix package manager installation instructions](https://nixos.org/download.html)
- [Jasmin installation instructions](https://github.com/jasmin-lang/jasmin/wiki/Installation-instructions)

If you later need to update the Jasmin compiler to the latest version:
- Check current version: `jasminc -version`
- Update Nix package repositories: `nix-channel --update`
- Install Jasmin Compiler latest version: `nix-env -iA nixpkgs.jasmin-compiler`
- Check installed version: `jasminc -version`

### Docker
We provide a Dockerfile in the `docker` directory to build a Docker image with the Jasmin compiler preinstalled. A container can then be created using this image.

This Dockerfile has been successfully tested with a Ubuntu 22.04 base image.

Remark: on linux, running the docker commands requires root privileges (insert `sudo` in front of every command, or run `sudo su` once).

If not already done, install Docker.

Here are some important commands to work with Docker:
- Build an image from a Dockerfile: `docker build -t <image_name> <path_to_directory_containing_dockerfile>`
	- Explanation of the options used:
		- `-t`: specify the desired image name
	- Example: build an image with the provided Dockerfile: `docker build -t jasmin docker/`
- Create a container, based on an image, with a volume that links a host directory with a container directory: `docker run -it -v <host_workspace_path>:<container_workspace_path> --hostname <hostname> --name <container_name> <image_name>`
	- Explanation of the options used:
		- `-i`: interactive mode (keep a container running and interact with it)
		- `-t`: provides a pseudo-terminal for the container, allowing you to interact with it as if you were using a real terminal
		- `-v`: create a volume that links a host directory with a container directory
		- `--hostname`: specify the host name of the container
		- `--name`: specify the name of the container
	- Example: create and run a container based on the previously created image: `docker run -it -v C:\Workspace:/workspace --hostname jasmin-container --name jasmin-container jasmin`.
	- In the container, you now have access to the files present in the directory <host_workspace_path> on the host machine.
	- If you have multiple containers based on the same image, give them different host names so that you can differentiate between them while in the terminal.
- Exit the container without stopping it (from inside the container): `Ctrl + P`, `Ctrl + Q`
- Join a running container: `docker attach <container_name>`
- Exit the container and stop it (from inside the container): `exit`
- Stop a container: `docker stop <container_name>`
- Start a container: `docker start <container_name>`
- List all images: `docker image ls`
- List all containers: `docker container ls -a`
	- Remark: running this command without the `-a` option displays only the running containers.

### Open project from a Docker container in Visual Studio Code
You might want to open VS Code in the container. For example, when opening C source files in Windows, if GCC is not installed, C libraries are unknown. With this method, VS Code will have the environment of the container, and will detect the C libraries if GCC is installed in the container. It is generally better to give VS Code the development environment, to fully take advantage of its features, notably code completion and syntax highlighting.
- Start the Docker container: `docker start <container_name>`
- Open VS Code
- Install the following extensions:
	- Docker: `ms-azuretools.vscode-docker`
	- Dev Containers: `ms-vscode-remote.remote-containers`
- Click on the `Docker` extension
- Right click on the container, click `Attach Visual Studio Code`
- You can now open a directory of your choice in the container. `File` -> `Open Folder...` -> select the directory -> `OK`. For example, open `<container_workspace_path>` to work on the files shared with the host.

Source: [Developing inside a Container](https://code.visualstudio.com/docs/devcontainers/containers)

### Configure syntax highlighting for Jasmin code in Visual Studio Code
- Open any `.jazz` file
- Click on `Plain Text` in the bottom right corner
- Click on `Configure File Association for '.jazz'...`
- Search for `rust`
- Click on `Rust`
- Repeat the same steps with a `.jinc` file

## Run Jasmin code

TL;DR: to run AES-GCM:
- `cd src/aes_gcm`
- `make`
- `make run`

You can try your own values in `src/aes_gcm/test_aes_gcm.c`.

Here are the commands to run a simple Jasmin program:
- Compile Jasmin to assembly: `jasminc hello_world.jazz -o hello_world.s`
- Compile the C program using the Jasmin code: `gcc main.c hello_world.s -o main.out`
- Run: `/main.out`

There are several modules that can be run in this repository. These modules are in the `src` and `misc` directories. All modules have a Makefile for simplicity. To run a module, follow these instructions:
- `cd <directory>`
- Compile: `make`
- Run: `make run` or `./<name>.out`

The main module is `src/aes_gcm`. It uses all other modules in `src`.

If you want to try programming in Jasmin, an hello world example is available in the `misc` directory.
