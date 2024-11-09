{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    nixpkgs-unstable.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs =
    { nixpkgs, nixpkgs-unstable, ... }@inputs:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      pkgs-unstable = import nixpkgs-unstable { inherit system; };
    in
    {
      devShell.${system} = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          cmake
          just

          llvmPackages_18.libcxxStdenv
          llvmPackages_18.clang-tools

          cppcheck

          vulkan-tools

          doxygen

          shaderc

          glsl_analyzer
          glslang
          glslls
        ];

        buildInputs = with pkgs; [
          llvmPackages_18.libcxxStdenv

          vulkan-headers
          vulkan-loader
          vulkan-validation-layers

          glfw-wayland
          glm
        ];

        LD_LIBRARY_PATH = "${pkgs.lib.makeLibraryPath (
          with pkgs;
          [
            glfw-wayland
            glm
          ]
        )}";

        shellHook = ''
          exec zsh
        '';
      };
    };
}
