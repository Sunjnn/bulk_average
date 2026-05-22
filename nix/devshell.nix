{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  inputsFrom = [ (import ./toolchain.nix { inherit pkgs; }) ];

  packages = with pkgs; [
    clang-tools
    gdb
    zsh
    zsh-autosuggestions
    zsh-syntax-highlighting
    oh-my-zsh
  ];

  shellHook = ''
    export ZDOTDIR=$(mktemp -d)
    cat > "$ZDOTDIR/.zshrc" << 'EOF'
export ZSH=${pkgs.oh-my-zsh}/share/oh-my-zsh
export ZSH_THEME="robbyrussell"
plugins=()
source $ZSH/oh-my-zsh.sh
source ${pkgs.zsh-autosuggestions}/share/zsh-autosuggestions/zsh-autosuggestions.zsh
source ${pkgs.zsh-syntax-highlighting}/share/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh
EOF
    exec zsh
  '';
}
