{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
	buildInputs = with pkgs; [
		cmake
		ninja

		pkgs.SDL2
		xorg.xorgserver
		xorg.libX11
		xorg.libxcb
		libcxx
		libglvnd.dev

		clang
		clang-tools # for clang-format, clang-tidy, etc.
	];


	shellHook = ''
		export CXX=clang++
		export CC=clang
	'';
}

