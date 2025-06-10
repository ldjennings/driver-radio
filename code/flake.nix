# A standalone shell definition that downloads and uses packages from `nixpkgs-esp-dev` automatically.


# let
#   nixpkgs-esp-dev = builtins.fetchGit {
#     url = "https://github.com/mirrexagon/nixpkgs-esp-dev.git";

#     # Optionally pin to a specific commit of `nixpkgs-esp-dev`.
#     # rev = "<commit hash>";
#   };

#   pkgs = import <nixpkgs> { overlays = [ (import "${nixpkgs-esp-dev}/overlay.nix") ]; };
# in
# pkgs.mkShell {
#   name = "esp-project";

#   buildInputs = with pkgs; [
#     esp-idf-full
#   ];
# }


{
  description = "setting up development environment for the driver radio.";

  inputs = {

    nixpkgs.url = "nixpkgs/nixos-unstable";  # TODO: pin this once a stable version has been reached

    flake-utils.url = "github:numtide/flake-utils";

    nixpkgs-esp-dev.url = "github:mirrexagon/nixpkgs-esp-dev";
    nixpkgs-esp-dev.inputs.nixpkgs.follows = "nixpkgs";

  };

    outputs = { self, nixpkgs, flake-utils, nixpkgs-esp-dev, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      {
        devShells.default = nixpkgs-esp-dev.devShells.${system}.esp32-idf;
      });
}