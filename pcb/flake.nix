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
  description = "Building pcb export files for JLCPCB";

  inputs = {

    nixpkgs.url = "github:nixos/nixpkgs/nixos-25.11";  # TODO: pin this once a stable version has been reached

    flake-utils.url = "github:numtide/flake-utils";

    nixpkgs-esp-dev.url = "github:mirrexagon/nixpkgs-esp-dev";
    nixpkgs-esp-dev.inputs.nixpkgs.follows = "nixpkgs";

  };

    outputs = { self, nixpkgs, flake-utils, nixpkgs-esp-dev, ... }:

        let
          system = "x86_64-linux";
          pkgs = import nixpkgs { inherit system; };
        in {
          packages.${system}.fab = pkgs.stdenvNoCC.mkDerivation {
            pname = "pcb-fab";
            version = "1.0";

            src = ./.;

            nativeBuildInputs = [
              pkgs.kicad-small
            ];

            buildPhase = ''
              export HOME=$TMPDIR

              export KICAD_CONFIG_HOME=$TMPDIR/kicad-config
              export KICAD_CACHE_HOME=$TMPDIR/kicad-cache

              mkdir -p "$KICAD_CONFIG_HOME" "$KICAD_CACHE_HOME"

              mkdir -p fab/{gerbers,drill,pos}

              kicad-cli pcb export gerbers --board-plot-params -o fab/gerbers pcb/driver-radio.kicad_pcb  

              kicad-cli pcb export drill -o fab/drill pcb/driver-radio.kicad_pcb

              kicad-cli pcb export pos -o fab/pos/driver-radio-pos.txt pcb/driver-radio.kicad_pcb
            '';

            installPhase = ''
              mkdir -p $out
              cp -r fab $out/
              rm -rf fab
            '';
          };
        };

    


    
}