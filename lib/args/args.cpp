#include "args.h"

void Args::Get(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--input") == 0 || strcmp(argv[i], "-i") == 0) {
            input = argv[++i];
            if (strcmp(input + strlen(input) - 4, ".tsv") != 0 &&
                strcmp(input + strlen(input) - 4, ".tab") != 0) {
                std::cerr << "Invalid input file: " << input << std::endl;
                input = nullptr;
            }
        } else if (strcmp(argv[i], "--output") == 0 ||
                   strcmp(argv[i], "-o") == 0) {
            output = argv[++i];
        } else if (strcmp(argv[i], "--max_iter") == 0 ||
                   strcmp(argv[i], "-m") == 0) {
            ++i;
            auto result =
                std::from_chars(argv[i], argv[i] + strlen(argv[i]), max_iter);
            if (result.ec != std::errc() ||
                result.ptr != argv[i] + strlen(argv[i])) {
                std::cerr << "Invalid max_iter: " << argv[i] << std::endl;
                max_iter = std::numeric_limits<size_t>::max();
            }
        } else if (strcmp(argv[i], "--freq") == 0 ||
                   strcmp(argv[i], "-f") == 0) {
            ++i;
            auto result =
                std::from_chars(argv[i], argv[i] + strlen(argv[i]), freq);
            if (result.ec != std::errc() ||
                result.ptr != argv[i] + strlen(argv[i])) {
                std::cerr << "Invalid freq: " << argv[i] << std::endl;
                freq = std::numeric_limits<size_t>::max();
            }
        } else if (strcmp(argv[i], "--fast") == 0) {
            is_fast = true;
        } else {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
        }
    }
}

void Args::Print() const {
    std::cout << "input: " << (input != nullptr ? input : "null") << std::endl;
    std::cout << "output: " << (output != nullptr ? output : "null")
              << std::endl;
    std::cout << "max_iter: " << max_iter << std::endl;
    std::cout << "freq: " << freq << std::endl;
}

bool Args::IsValid() const {
    return input != nullptr && output != nullptr;
}
