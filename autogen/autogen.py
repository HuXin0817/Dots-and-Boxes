MaxBoardSize = 36

for i in range(1, MaxBoardSize + 1):
    open(f"MainWindow{i}.cpp", "w").write(
        f"""#include <Dab/Frontend/Frontend.hpp>

template class dab::MainWindow<{i}>;"""
    )
