#include "fuzzywuzzy.hpp"
#include "catch.hpp"

/**
 * Test the examples described in Readme
 */
TEST_CASE("ReadmeExample")
{
    // Simple Ratio
    REQUIRE( 97 == fuzz::ratio("this is a test", "this is a test!") );

    // Partial Ratio
    REQUIRE( 100 == fuzz::partial_ratio("this is a test", "this is a test!") );

    // Token Sort Ratio
    REQUIRE( 91 == fuzz::ratio("fuzzy wuzzy was a bear", "wuzzy fuzzy was a bear")) ;
    REQUIRE( 100 == fuzz::token_sort_ratio("fuzzy wuzzy was a bear", "wuzzy fuzzy was a bear") );

    // Token Set Ratio
    REQUIRE( 84 == fuzz::token_sort_ratio("fuzzy was a bear", "fuzzy fuzzy was a bear") );
    REQUIRE( 100 == fuzz::token_set_ratio("fuzzy was a bear", "fuzzy fuzzy was a bear") );
}
