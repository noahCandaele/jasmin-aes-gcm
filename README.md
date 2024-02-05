# AES-GCM in Jasmin

Noah CANDAELE and Anthony IOZZIA

## Environment installation

Install Jasmin (tested on Ubuntu):
- Update package repositories: `sudo apt update`
- Install packages: `sudo apt install -y gcc curl`
- Install the Nix package manager: `sh <(curl -L https://nixos.org/nix/install) --no-daemon`
- Set the necessary environment variables: `. ~/.nix-profile/etc/profile.d/nix.sh`
- Install Jasmin compiler: `nix-env -iA nixpkgs.jasmin-compiler`
- Reboot system
- Test installation: `jasminc -version`

Configure syntax highlighting for Jasmin code in Visual Studio Code:
- Open any `.jazz` file
- Click on `Plain Text` in the bottom right corner
- Click on `Configure File Association for '.jazz'...`
- Search for `rust`
- Click on `Rust`

Sources:
- [Nix package manager installation instructions](https://nixos.org/download.html)
- [Jasmin installation instructions](https://github.com/jasmin-lang/jasmin/wiki/Installation-instructions)

## Docker
We provide a Dockerfile in the `Docker` directory to build a Docker image with the Jasmin compiler preinstalled. A container can then be created using this image. Here are useful commands to work with Docker:
- Build an image: `docker build -t <image_name> <path_to_directory_containing_dockerfile>`
- Create a container, based on an image, with a volume that links a host directory with a container directory: `docker run -it -v <host_workspace_path>:<container_workspace> --name <container_name> <image_name>`
- Exit the container without stopping it (from inside the container): `Ctrl + P`, `Ctrl + Q`
- Exit the container and stop it (from inside the container): `exit`
- Join a running container: `docker attach <container_name>`
- Stop a container: `docker stop <container_name>`
- Start a container: `docker start <container_name>`

Example:
- Build the image with the Jasmin compiler installed: `docker build -t jasmin Docker/`
- Create and run a container based on this image: `docker run -it -v C:\Workspace:/workspace --name my-container jasmin`

## Run

- `cd <directory>`
- `make`
- `./main.out`

## Environment update
Update Jasmin Compiler to the latest version:
- Check current version: `jasminc -version`
- Update Nix package repositories: `nix-channel --update`
- Install Jasmin Compiler latest version: `nix-env -iA nixpkgs.jasmin-compiler`
- Check installed version: `jasminc -version`
