{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    gcc13
    cmake
    gnumake
    gtest
    gdb
    valgrind
    clang-tools
    git
  ];

  # Устанавливаем переменные окружения для совместимости
  shellHook = ''
    export CMAKE_POLICY_VERSION_MINIMUM=3.5
    
    echo "=== Окружение для разработки ST-1 ==="
    echo "CMake version: $(cmake --version | head -n1)"
    echo "GCC version: $(gcc --version | head -n1)"
    echo ""
    echo "Установлена CMAKE_POLICY_VERSION_MINIMUM=3.5"
    echo ""
    echo "Команды для сборки:"
    echo "  mkdir -p build && cd build"
    echo "  cmake .."
    echo "  make -j$(nproc)"
    echo "  ./ST-1.test"
    echo "========================================"
  '';
}
