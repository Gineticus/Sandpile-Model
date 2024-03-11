#include <chrono>
#include <iostream>

#include "lib/args/args.h"
#include "lib/bmp/bmp.h"
#include "lib/sandpile/sandpile.h"

int main(int argc, char** argv) {
    Args args;
    args.Get(argc, argv);
    args.Print();
    if (!args.IsValid()) {
        std::cerr << "Wrong arguments" << std::endl;
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    Sandpile sandpile;
    if (!sandpile.Make(args.input)) {
        return 1;
    }
    size_t i = 0;
    for (; i < args.max_iter; ++i) {
        if (args.freq != 0 && i % args.freq == 0) {
            BMP bmp(sandpile.table, i);
            bmp.Write(args.output);
            std::cout << "Iteration: " << i << std::endl;
        }
        if (!sandpile.Iteration(args.is_fast)) {
            break;
        }
    }
    if (args.freq == 0 || i % args.freq == 0) {
        BMP bmp(sandpile.table, i);
        bmp.Write(args.output);
        std::cout << "Iteration: " << i << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
