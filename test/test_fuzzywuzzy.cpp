#include "utils.hpp"
#include "fuzzywuzzy.hpp"
#include "process.hpp"

#include "catch.hpp"

/**
 * Port suitable tests from the original Python repository:
 *     https://github.com/seatgeek/fuzzywuzzy/blob/master/test_fuzzywuzzy.py
 * And add some exclusive tests for this library.
 *
 * Tests that relate to unicode are not ported because this library does not
 * support unicode for the time beging. Also, std::string is not unicode-compatible
 * so a new container and new interface shouble be specified.
 *
 * ( ) StringProcessingTest  Not applicable
 * (v) UtilsTest
 * (v) RatioTest
 * ( ) ValidatorTest         C++ does not have decorator as in Python
 * (v) ProcessTest
 * ( ) TestCodeFormat        Not applicable
 */

/**
 * @name UtilsTest
 */
TEST_CASE("UtilsTest")
{
    SECTION("testSplit")
    {
        const std::string s1 = "This is a test";
        const std::string s2 = "fuzzy/buzzy/is/a/bear";

        REQUIRE( fuzz::utils::split_string(s1) ==
                 std::vector<string>({"This", "is", "a", "test"}) );
        REQUIRE( fuzz::utils::split_string(s2, '/') ==
                 std::vector<string>({"fuzzy", "buzzy", "is", "a", "bear"}) );
    }

    SECTION("testJoin")
    {
        const std::vector<std::string> s1 = {"This", "is", "a", "test"};

        REQUIRE( fuzz::utils::join(s1) == "This is a test" );
        REQUIRE( fuzz::utils::join(s1, "/") == "This/is/a/test" );
    }

    SECTION("testTrim")
    {
        // trim() mutates input string
        std::string a1 = " ";
        std::string a2 = "    foo";
        std::string a3 = "foo       ";
        std::string a4 = "   foo     ";

        REQUIRE( fuzz::utils::trim(a1) == "" );
        REQUIRE( fuzz::utils::trim(a2) == "foo" );
        REQUIRE( fuzz::utils::trim(a3) == "foo" );
        REQUIRE( fuzz::utils::trim(a4) == "foo" );
    }

    SECTION("testFullProcess")
    {
        const std::string s1 = "new york mets";
        const std::string s1a = "new york mets";
        const std::string s2 = "new YORK mets";
        const std::string s3 = "the wonderful new york mets";

        REQUIRE( s1 == fuzz::utils::full_process(s1a) );
        REQUIRE( s1 != fuzz::utils::full_process(s3) );

        // String is in all lowercase
        REQUIRE( s1 == fuzz::utils::full_process(s2) );

        // Trim the exclamation mark at end
        const std::string a1 = "this is a test";
        const std::string a2 = "this is a test!";

        REQUIRE( a1 == fuzz::utils::full_process(a1) );
        REQUIRE( a1 == fuzz::utils::full_process(a2) );

        // Inner space is not trimmed
        const std::string a3 = "new york mets - atlanta braves";
        const std::string a4 = "new york mets   atlanta braves";

        REQUIRE( a4 == fuzz::utils::full_process(a3) );
        REQUIRE( a4 == fuzz::utils::full_process(a4) );
    }
}

/**
 * @name RatioTest
 *
 * @todo Enable 'testPartialTokenSortRatio' once the function is implemented
 */
TEST_CASE("RatioTest")
{
    const std::string s1 = "new york mets";
    const std::string s1a = "new york mets";
    const std::string s2 = "new YORK mets";
    const std::string s3 = "the wonderful new york mets";
    const std::string s4 = "new york mets vs atlanta braves";
    const std::string s5 = "atlanta braves vs new york mets";
    const std::string s6 = "new york mets - atlanta braves";
    const std::string s7 = "new york city mets - atlanta braves";
    // test silly corner cases
    const std::string s8 = "{";
    const std::string s8a = "{";
    const std::string s9 = "{a";
    const std::string s9a = "{a";
    const std::string s10 = "a{";
    const std::string s10a = "{b";

    SECTION("testEqual")
    {
        REQUIRE( 100 == fuzz::ratio(s1, s1a) );
        REQUIRE( 100 == fuzz::ratio(s8, s8a) );
        REQUIRE( 100 == fuzz::ratio(s9, s9a) );
    }

    SECTION("testCaseInsensitive")
    {
        REQUIRE( 100 != fuzz::ratio(s1, s2) );
        REQUIRE( 100 == fuzz::ratio(fuzz::utils::full_process(s1), fuzz::utils::full_process(s2)) );
    }

    SECTION("testPartialRatio")
    {
        REQUIRE( 100 == fuzz::partial_ratio(s1, s1) );
        REQUIRE( 100 == fuzz::partial_ratio(s1, s3) );
    }

    SECTION("testTokenSortRatio")
    {
        REQUIRE( 100 == fuzz::token_sort_ratio(s1, s1a) );
    }

    /*
    SECTION("testPartialTokenSortRatio")
    {
        REQUIRE( 100 == fuzz::partial_token_sort_ratio(s1, s1a) );
        REQUIRE( 100 == fuzz::partial_token_sort_ratio(s4, s5) );
        REQUIRE( 100 == fuzz::partial_token_sort_ratio(s8, s8a, false) );
        REQUIRE( 100 == fuzz::partial_token_sort_ratio(s9, s9a, true) );
        REQUIRE( 100 == fuzz::partial_token_sort_ratio(s9, s9a, false) );
        REQUIRE( 50  == fuzz::partial_token_sort_ratio(s10, s10a, false) );
    }
    */

    SECTION("testTokenSetRatio")
    {
        REQUIRE( 100 == fuzz::token_set_ratio(s4, s5) );
        REQUIRE( 100 == fuzz::token_set_ratio(s8, s8a, false) );
        REQUIRE( 100 == fuzz::token_set_ratio(s9, s9a, true) );
        REQUIRE( 100 == fuzz::token_set_ratio(s9, s9a, false) );
        REQUIRE( 50  == fuzz::token_set_ratio(s10, s10a, false) );
    }

    SECTION("testPartialTokenSetRatio")
    {
        REQUIRE( 100 == fuzz::partial_token_set_ratio(s4, s7) );
    }

    SECTION("testQuickRatioEqual")
    {
        REQUIRE( 100 == fuzz::quick_ratio(s1, s1a) );
    }

    SECTION("testQuickRatioCaseInsensitive")
    {
        REQUIRE( 100 == fuzz::quick_ratio(s1, s2) );
    }

    SECTION("testQuickRatioNotEqual")
    {
        REQUIRE( 100 != fuzz::quick_ratio(s1, s3) );
    }

    SECTION("testWRatioEqual")
    {
        REQUIRE( 100 == fuzz::weighted_ratio(s1, s1a) );
    }

    SECTION("testWRatioCaseInsensitive")
    {
        REQUIRE( 100 == fuzz::weighted_ratio(s1, s2) );
    }

    SECTION("testWRatioPartialMatch")
    {
        // a partial match is scaled by .9
        REQUIRE( 90 == fuzz::weighted_ratio(s1, s3) );
    }

    SECTION("testWRatioMisorderedMatch")
    {
        // misordered full matches are scaled by .95
        REQUIRE( 95 == fuzz::weighted_ratio(s4, s5) );
    }

    SECTION("testEmptyStringsScore100")
    {
        REQUIRE( 100 == fuzz::ratio("", "") );
        REQUIRE( 100 == fuzz::partial_ratio("", "") );
    }

    SECTION("testIssueSeven")
    {
        const string s1 = "HSINCHUANG";
        const string s2 = "SINJHUAN";
        const string s3 = "LSINJHUANG DISTRIC";
        const string s4 = "SINJHUANG DISTRICT";

        REQUIRE( fuzz::partial_ratio(s1, s2) > 75 );
        REQUIRE( fuzz::partial_ratio(s1, s3) > 75 );
        REQUIRE( fuzz::partial_ratio(s1, s4) > 75 );
    }
}

/**
 * @name ProcessTest
 *
 *      In the Python repo's test 'testWithProcessor', it makes use of the
 * type-punning property and supplies a vector of string instead of one string
 * as arguments. This makes porting the original test without API change
 * impossible.
 *      Also, the same things happened in 'testWithScorer', it passes a dictionary
 * instead a list in Python. There is no such counterpart mapping in current API.
 * Maybe a template approach can be applied in the future.
 */
TEST_CASE("ProcessTest")
{
    const std::string s1 = "new york mets";
    const std::string s1a = "new york mets";
    const std::string s2 = "new YORK mets";
    const std::string s3 = "the wonderful new york mets";
    const std::string s4 = "new york mets vs atlanta braves";
    const std::string s5 = "atlanta braves vs new york mets";
    const std::string s6 = "new york mets - atlanta braves";

    const std::vector<std::string> cirque_strings = {
        "cirque du soleil - zarkana - las vegas",
        "cirque du soleil ",
        "cirque du soleil las vegas",
        "zarkana las vegas",
        "las vegas cirque du soleil at the bellagio",
        "zarakana - cirque du soleil - bellagio"
    };

    const std::vector<std::string> baseball_strings = {
        "new york mets vs chicago cubs",
        "chicago cubs vs chicago white sox",
        "philladelphia phillies vs atlanta braves",
        "braves vs mets",
    };

    SECTION("testGetBestChoice1")
    {
        const std::string query = "new york mets at atlanta braves";
        auto best = fuzz::extractOne(query, baseball_strings);
        REQUIRE( best[0].first == baseball_strings[3] );
    }

    SECTION("testGetBestChoice2")
    {
        const std::string query = "philadelphia phillies at atlanta braves";
        auto best = fuzz::extractOne(query, baseball_strings);
        REQUIRE( best[0].first == baseball_strings[2] );
    }

    SECTION("testGetBestChoice3")
    {
        const std::string query = "atlanta braves at philadelphia phillies";
        auto best = fuzz::extractOne(query, baseball_strings);
        REQUIRE( best[0].first == baseball_strings[2] );
    }

    SECTION("testGetBestChoice4")
    {
        const std::string query = "chicago cubs vs new york mets";
        auto best = fuzz::extractOne(query, baseball_strings);
        REQUIRE( best[0].first == baseball_strings[0] );
    }

    /*
    SECTION("testWithProcessor")
    {
        const std::vector< std::vector< std::string > > events =
            {
                {"chicago cubs vs new york mets", "CitiField", "2011-05-11", "8pm"},
                {"new york yankees vs boston red sox", "Fenway Park", "2011-05-11", "8pm"},
                {"atlanta braves vs pittsburgh pirates", "PNC Park", "2011-05-11", "8pm"},
            };
        const std::vector<std::string> query =
            {"new york mets vs chicago cubs", "CitiField", "2017-03-19", "8pm"};

        auto best = fuzz::extractOne(query, events,
                                     [](const std::vector<std::string>& vec) -> std::string
                                     { return vec[0]; });
        REQUIRE( best[0].first == events[0] );
    }
    */

    SECTION("testWithScorer")
    {
        const std::vector<std::string> choices = {
            "new york mets vs chicago cubs",
            "chicago cubs at new york mets",
            "atlanta braves vs pittsbugh pirates",
            "new york yankees vs boston red sox"
        };

        /*
        const std::unordered_map<int, std::string> choices_dict = {
            {1, "new york mets vs chicago cubs"},
            {2, "chicago cubs vs chicago white sox"},
            {3, "philladelphia phillies vs atlanta braves"},
            {4, "braves vs mets"}
        };
        */

        const std::string query = "new york mets at chicago cubs";

        auto best1 = fuzz::extractOne(query, choices);
        REQUIRE( best1[0].first == choices[1] );

        // now, use the custom scorer
        auto best2 = fuzz::extractOne(query, choices, fuzz::utils::full_process,
                                      fuzz::quick_ratio);
        REQUIRE( best2[0].first == choices[0] );

        /*
        auto best3 = fuzz::extractOne(query, choices_dict);
        REQUIRE( best3.first == choices_dict[1] );
        */
    }

    SECTION("testWithCutoff")
    {
        const std::vector<std::string> choices = {
            "new york mets vs chicago cubs",
            "chicago cubs at new york mets",
            "atlanta braves vs pittsbugh pirates",
            "new york yankees vs boston red sox"
        };

        const std::string query = "los angeles dodgers vs san francisco giants";

        // in this situation, this is an event that does not exist in the list
        // we don't want to randomly match to something, so we use a reasonable cutoff

        auto best = fuzz::extractOne(query, choices, fuzz::utils::full_process,
                                     fuzz::weighted_ratio, 50);
        REQUIRE( best.empty() );
    }

    SECTION("testWithCutoff2")
    {
        const std::vector<std::string> choices = {
            "new york mets vs chicago cubs",
            "chicago cubs at new york mets",
            "atlanta braves vs pittsbugh pirates",
            "new york yankees vs boston red sox"
        };

        const std::string query = "new york mets vs chicago cubs";

        // Only find 100-score cases
        auto best = fuzz::extractOne(query, choices, fuzz::utils::full_process,
                                     fuzz::weighted_ratio, 100);
        REQUIRE( !best.empty() );
        REQUIRE( best[0].first == choices[0]);
    }

    SECTION("testEmptyStrings")
    {
        const std::vector<string> choices = {
            "",
            "new york mets vs chicago cubs",
            "new york yankees vs boston red sox",
            "",
            ""
        };

        const std::string query = "new york mets at chicago cubs";

        auto best = fuzz::extractOne(query, choices);
        REQUIRE( best[0].first == choices[1] );
    }

    SECTION("testDedupe1")
    {
        const std::vector<std::string> contains_dupes =
            {"Frodo Baggins", "Tom Sawyer", "Bilbo Baggin", "Samuel L. Jackson",
             "F. Baggins", "Frody Baggins", "Bilbo Baggins"};

        auto result = fuzz::dedupe(contains_dupes);
        REQUIRE( result.size() < contains_dupes.size() );
    }

    SECTION("testDedupe2")
    {
        const std::vector<std::string> contains_dupes = {"Tom", "Dick", "Harry"};

        // we should end up with the same list since no duplicates are contained
        // in the list (e.g. original list is returned)
        const std::vector<std::string> deduped_list = {"Tom", "Dick", "Harry"};

        auto result = fuzz::dedupe(contains_dupes);
        REQUIRE( result == deduped_list );
    }

    SECTION("testSimpleMatch")
    {
        const std::string basic_string = "a, b";
        const std::vector<std::string> match_strings = {"a, b"};

        auto result = fuzz::extractOne(basic_string, match_strings,
                                       fuzz::utils::full_process, fuzz::ratio);
        auto part_result = fuzz::extractOne(basic_string, match_strings,
                                            fuzz::utils::full_process,
                                            fuzz::partial_ratio);

        REQUIRE( fuzz::ratio(basic_string, basic_string) == 100 );
        REQUIRE( fuzz::partial_ratio(basic_string, basic_string) == 100 );

        REQUIRE( result[0] == std::pair<std::string, int>{"a, b", 100} );
        REQUIRE( part_result[0] == std::pair<std::string, int>{"a, b", 100} );
    }
}
