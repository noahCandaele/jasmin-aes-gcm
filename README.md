# AES-GCM in Jasmin

Noah CANDAELE and Anthony IOZZIA

## Environment installation

### Install Jasmin (tested on Ubuntu)
- Update package repositories: `sudo apt update`
- Install packages: `sudo apt install -y gcc curl`
- Install the Nix package manager: `sh <(curl -L https://nixos.org/nix/install) --no-daemon`
- Set the necessary environment variables: `. ~/.nix-profile/etc/profile.d/nix.sh`
- Install Jasmin compiler: `nix-env -iA nixpkgs.jasmin-compiler`
- Test installation: `jasminc -version`

### Configure syntax highlighting for Jasmin code in Visual Studio Code
- Open any `.jazz` file
- Click on `Plain Text` in the bottom right corner
- Click on `Configure File Association for '.jazz'...`
- Search for `rust`
- Click on `Rust`

### Sources
- [Nix package manager installation instructions](https://nixos.org/download.html)
- [Jasmin installation instructions](https://github.com/jasmin-lang/jasmin/wiki/Installation-instructions)

## Run

- `cd <directory>`
- `make`
- `./main.out`
