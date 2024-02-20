# Use an Ubuntu base image
FROM ubuntu:latest

# Install necessary dependencies
RUN apt-get update && \
	apt-get install -y \
	sudo \
	curl \
	xz-utils \
	gpg \
	gcc \
	make \
	vim

# Create a non-root user for Nix
RUN useradd -m -s /bin/bash user

# Allow the non-root user to execute commands with sudo
RUN echo "user ALL=(ALL:ALL) NOPASSWD:ALL" > /etc/sudoers.d/user

# Switch to the non-root user
USER user

# Download and install Nix
RUN curl -L https://nixos.org/nix/install | sh

# Add the Nix binary directory to the PATH
ENV PATH="/home/user/.nix-profile/bin:$PATH"

# Set up the Nix store
RUN nix-collect-garbage

# Install Jasmin compiler
RUN nix-env -iA nixpkgs.jasmin-compiler

# Color the container bash prompt
ENV force_color_prompt=yes

# Set the work directory
WORKDIR /workspace
