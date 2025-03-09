let
    pkgs = import <nixpkgs> {};
in
pkgs.mkShell {
    packages = with pkgs; [
        clang
        cmake

        raylib

        libGL
        xorg.libX11
        xorg.libXcursor
        xorg.libXext
        xorg.libXfixes
        xorg.libXi
        xorg.libXinerama
        xorg.libXrandr
        xorg.libXrender
    ];

    RAYLIB = "${pkgs.raylib}";
}
