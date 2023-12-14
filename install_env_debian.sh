sudo apt update
sudo apt install gcc curl
sh <(curl -L https://nixos.org/nix/install) --no-daemon
nix-env -iA nixpkgs.jasmin-compiler
echo "Installation done, please reboot."
