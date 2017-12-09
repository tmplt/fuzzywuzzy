#include <iostream>
#include <cstdlib>
#include <string>

#include "fuzzywuzzy.hpp"
#include "cxxopts.hpp"

int main(int argc, char *argv[])
{
    cxxopts::Options options(argv[0], "CLI of the fuzzywuzzy library");
    options.positional_help("STRING1 STRING2");

    /*
     * These options are hidden from the help menu,
     * but must be declared to look for them. However, they can still be passed.
     */
    options.add_options()
        ("string1", "First input string", cxxopts::value<std::string>())
        ("string2", "Second input string", cxxopts::value<std::string>());

    options.add_options("Misc")
        ("h,help",                        "Print this help menu and exit")
        ("f,full-process",                "Apply the full process");
        /* ("p,partial"); // here instead? Some "depend" on it. */

    options.add_options("Basic scoring")
        ("r,ratio",                       "Calculates a Levenshtein ratio between the strings")
        ("p,partial",                     "Calculates the ratio of the most similar substrings");

    options.add_options("Advanced scoring")
        ("token-sort-ratio",              "Calculates the ratio between the strings "
                                          "after sorting their tokens")
        ("token-sort-partial-ratio",      "Calculates the partial ratio between the strings "
                                          "after sorting their tokens");

    options.parse_positional({"string1", "string2"});
    auto r = options.parse(argc, argv);

    if (r.count("help")) {
        std::cout << options.help({"Misc","Basic scoring","Advanced scoring"});
        return EXIT_SUCCESS;
    }

    if (r.count("string1") == 0 || r.count("string2") == 0) {
        std::cerr << options.help();
        return EXIT_FAILURE;
    }

    const auto string1 = r["string1"].as<std::string>(),
               string2 = r["string2"].as<std::string>();

    if (r.count("partial")) {
        std::cout << fuzz::partial_ratio(string1, string2) << "\n";
    } else {
        /* Simple ratio as default. */
        std::cout << fuzz::ratio(string1, string2) << "\n";
    }

    return EXIT_SUCCESS;
}
