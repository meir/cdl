{
  inputs = {
    flake-compat = {
      url = "github:edolstra/flake-compat";
      flake = false;
    };
    nixpkgs.url = "github:nixos/nixpkgs/stable";
  };

  outputs = { self, nixpkgs, flake-compat }: {
    packages = nixpkgs.lib.genAttrs targetSystem (system:
      let
        pkgs = pkgsFor system;
        gcc = pkgs.gcc;
        version = "0.1.1";
      in
      rec {
        cdl = mkDerivation {
          version = "v${version}";
          pname = "cdl";

          src = ./.;

          buildInputs = [ gcc ];

          phases = "installPhase";

          installPhase = ''
            mkdir -p $out/bin
            rm -rf $out/bin/*

            ${lib.getExe gcc} -o $out/bin/cdls ${src}/src/cache/cache.c ${src}/src/cdls.c
            ${lib.getExe gcc} -o $out/bin/cdp ${src}/src/cache/cache.c ${src}/src/cdp.c
            ${lib.getExe gcc} -o $out/bin/cdr ${src}/src/cache/cache.c ${src}/src/cdr.c
            ${lib.getExe gcc} -o $out/bin/cds ${src}/src/cache/cache.c ${src}/src/cds.c

            echo 'cdl() { cd "$(cdp $1)" }' > $out/bin/cdl-alias

            chmod +x $out/bin/*
          '';

          outputs = [ "out" ];

          meta = {
            homepage = "https://github.com/meir/cdl";
            description = "Change Directory List (cdl) is a small directory waypoint tool for the terminal";
            platforms = lib.platforms.all;
            maintainers = [ ];
            mainProgram = [
              "cdls"
              "cdp"
              "cdr"
              "cds"
            ];
          };
        };
      }
    );

    devShells = nixpkgs.lib.genAttrs targetSystems (system:
      let
        pkgs = pkgsFor system;
        gcc = pkgs.gcc;
      in
      {
        default = pkgs.mkShell {
          inputsFrom = [ self.packages.${system}.cdl ];
          packages = with pkgs; [ gcc ];
        };
      }
    );
  };
}

