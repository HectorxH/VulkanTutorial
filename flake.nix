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
        packages = with pkgs; [
          cmake
          just

          llvmPackages_18.libcxxStdenv
          llvmPackages_18.clang-tools

          vulkan-tools
          vulkan-headers
          vulkan-validation-layers

          doxygen

          shaderc

          glsl_analyzer
          glslang
          glslls

          glfw-wayland
          glm
        ];

        LD_LIBRARY_PATH = "${pkgs.lib.makeLibraryPath (
          with pkgs;
          [
            wayland

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
