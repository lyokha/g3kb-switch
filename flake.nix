{
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem
      (system:
        with nixpkgs.legacyPackages.${system};
        {
          formatter = nixpkgs-fmt;
          packages.default = stdenv.mkDerivation rec {
            pname = "g3kb-switch";
            version = "1.3";
            src = self;
            nativeBuildInputs = [
              cmake
              pkg-config
            ];
            buildInputs = [
              glib
            ];
            cmakeFlags = [
              "-DG3KBSWITCH_VIM_XKBSWITCH_LIB_PATH=lib"
              "-DBASH_COMPLETION_COMPLETIONSDIR=${placeholder "out"}/share/bash-completions/completions"
              "-DZSH_COMPLETION_COMPLETIONSDIR=${placeholder "out"}/share/zsh/site-functions"
            ];
          };
        }
      );
}
