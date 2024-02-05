# AES-GCM in Jasmin

Noah CANDAELE and Anthony IOZZIA

## Environment installation

### Install Jasmin (tested on Ubuntu)
- `sudo apt update && sudo apt install -y gcc curl`
- `sh <(curl -L https://nixos.org/nix/install) --no-daemon`
- `nix-env -iA nixpkgs.jasmin-compiler`
- Reboot system
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
