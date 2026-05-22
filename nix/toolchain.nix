{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    cmake
    ninja
    clang
    gtest
  ];

  env = {
    CC = "clang";
    CXX = "clang++";
  };
}
