#include <sstream>
#include <fstream>

#include <gtest/gtest.h>

#include "lib/ArgParser.h"

using namespace ArgumentParser;


std::vector<std::string> SplitString(const std::string& str) {
    std::istringstream iss(str);

    return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
}


TEST(ArgParserTestSuite, EmptyTest) {
    ArgParser parser("My Empty Parser");

    ASSERT_TRUE(parser.Parse(SplitString("app")));
}


TEST(ArgParserTestSuite, StringTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument("param1");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1")));
    ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}


TEST(ArgParserTestSuite, ShortNameTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument('p', "param1");

    ASSERT_TRUE(parser.Parse(SplitString("app -p=value1")));
    ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}


TEST(ArgParserTestSuite, DefaultTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument("param1").Default("value1");

    ASSERT_TRUE(parser.Parse(SplitString("app")));
    ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}


TEST(ArgParserTestSuite, NoDefaultTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument("param1");

    ASSERT_FALSE(parser.Parse(SplitString("app")));
}


TEST(ArgParserTestSuite, StoreValueTest) {
    ArgParser parser("My Parser");
    std::string value;
    parser.AddStringArgument("param1").StoreValue(value);

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1")));
    ASSERT_EQ(value, "value1");
}


TEST(ArgParserTestSuite, MultiStringTest) {
    ArgParser parser("My Parser");
    std::string value;
    parser.AddStringArgument("param1").StoreValue(value);
    parser.AddStringArgument('a', "param2");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1 --param2=value2")));
    ASSERT_EQ(parser.GetStringValue("param2"), "value2");
}


TEST(ArgParserTestSuite, IntTest) {
    ArgParser parser("My Parser");
    parser.AddIntArgument("param1");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=100500")));
    ASSERT_EQ(parser.GetIntValue("param1"), 100500);
}


TEST(ArgParserTestSuite, MultiValueTest) {
    ArgParser parser("My Parser");
    std::vector<int> int_values;
    parser.AddIntArgument('p', "param1").MultiValue().StoreValues(int_values);

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=1 --param1=2 --param1=3")));
    ASSERT_EQ(parser.GetIntValue("param1", 0), 1);
    ASSERT_EQ(int_values[1], 2);
    ASSERT_EQ(int_values[2], 3);
}


TEST(ArgParserTestSuite, MinCountMultiValueTest) {
    ArgParser parser("My Parser");
    std::vector<int> int_values;
    size_t MinArgsCount = 10;
    parser.AddIntArgument('p', "param1").MultiValue(MinArgsCount).StoreValues(int_values);

    ASSERT_FALSE(parser.Parse(SplitString("app --param1=1 --param1=2 --param1=3")));
}


TEST(ArgParserTestSuite, FlagTest) {
    ArgParser parser("My Parser");
    parser.AddFlag('f', "flag1");

    ASSERT_TRUE(parser.Parse(SplitString("app --flag1")));
    ASSERT_TRUE(parser.GetFlag("flag1"));
}


TEST(ArgParserTestSuite, FlagsTest) {
    ArgParser parser("My Parser");
    bool flag3 ;
    parser.AddFlag('a', "flag1");
    parser.AddFlag('b', "flag2").Default(true);
    parser.AddFlag('c', "flag3").StoreValue(flag3);

    ASSERT_TRUE(parser.Parse(SplitString("app -ac")));
    ASSERT_TRUE(parser.GetFlag("flag1"));
    ASSERT_TRUE(parser.GetFlag("flag2"));
    ASSERT_TRUE(flag3);
}


TEST(ArgParserTestSuite, PositionalArgTest) {
    ArgParser parser("My Parser");
    std::vector<int> values;
    parser.AddIntArgument("Param1").MultiValue(1).Positional().StoreValues(values);

    ASSERT_TRUE(parser.Parse(SplitString("app 1 2 3 4 5")));
    ASSERT_EQ(values[0], 1);
    ASSERT_EQ(values[2], 3);
    ASSERT_EQ(values.size(), 5);
}


TEST(ArgParserTestSuite, PositionalAndNormalArgTest) {
    ArgParser parser("My Parser");
    std::vector<int> values;
    parser.AddFlag('f', "flag", "Flag");
    parser.AddIntArgument('n', "number", "Some Number");
    parser.AddIntArgument("Param1").MultiValue(1).Positional().StoreValues(values);

    ASSERT_TRUE(parser.Parse(SplitString("app -n 0 1 2 3 4 5 -f")));
    ASSERT_TRUE(parser.GetFlag("flag"));
    ASSERT_EQ(parser.GetIntValue("number"), 0);
    ASSERT_EQ(values[0], 1);
    ASSERT_EQ(values[2], 3);
    ASSERT_EQ(values.size(), 5);
}


TEST(ArgParserTestSuite, RepeatedParsingTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddStringArgument('i', "input", "File path for input file");
    parser.AddStringArgument('o', "output", "File path for output directory");
    parser.AddFlag('s', "flag1", "Read first number");
    parser.AddFlag('p', "flag2", "Read second number");
    parser.AddIntArgument("number", "Some Number");

    ASSERT_TRUE(parser.Parse(SplitString("app --number 2 -s -i test -o=test")));

    if (parser.GetFlag("flag1")) {
      parser.AddIntArgument("first", "First Number");
    } else if (parser.GetFlag("flag2")) {
      parser.AddIntArgument("second", "Second Number");
    }

    ASSERT_TRUE(parser.Parse(SplitString("app --number 2 -s -i test -o=test --first=52")));
    ASSERT_EQ(parser.GetIntValue("first"), 52);
}


TEST(ArgParserTestSuite, HelpTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");

    ASSERT_TRUE(parser.Parse(SplitString("app --help")));
    ASSERT_TRUE(parser.Help());
}


TEST(ArgParserTestSuite, HelpStringTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddStringArgument('i', "input", "File path for input file").MultiValue(1);
    parser.AddFlag('s', "flag1", "Use some logic").Default(true);
    parser.AddFlag('p', "flag2", "Use some logic");
    parser.AddIntArgument("number", "Some Number");


    ASSERT_TRUE(parser.Parse(SplitString("app --help")));

    ASSERT_EQ(
        parser.HelpDescription(),
        "My Parser\n"
        "Some Description about program\n"
        "\n"
        "-i,  --input=<string>,  File path for input file [repeated, min args = 1]\n"
        "-s,  --flag1,  Use some logic [default = true]\n"
        "-p,  --flag2,  Use some logic\n"
        "     --number=<int>,  Some Number\n"
        "\n"
        "-h, --help Display this help and exit\n"
    );
}

TEST(ArgParserTestSuite, RepeatedParsingDiferenceTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddStringArgument('i', "input", "File path for input file");
    parser.AddStringArgument('o', "output", "File path for output directory");
    parser.AddFlag('s', "flag1", "Read first number");
    parser.AddFlag('p', "flag2", "Read second number");
    parser.AddIntArgument("number", "Some Number");

    ASSERT_TRUE(parser.Parse(SplitString("app --number 2 -s -i test -o=test")));
    ASSERT_EQ(parser.GetIntValue("number"), 2);
    ASSERT_EQ(parser.GetStringValue("output"), "test");

    if (parser.GetFlag("flag1")) {
      parser.AddIntArgument("first", "First Number");
    } else if (parser.GetFlag("flag2")) {
      parser.AddIntArgument("second", "Second Number");
    }

    ASSERT_TRUE(parser.Parse(SplitString("app --number 24 -s -i test -o=test2 --first=52")));
    ASSERT_EQ(parser.GetIntValue("first"), 52);
    ASSERT_EQ(parser.GetIntValue("number"), 24);
    ASSERT_EQ(parser.GetStringValue("output"), "test2");

    ASSERT_TRUE(parser.Parse(SplitString("app --number 44 -s -i test --output=test3 --first=52")));
    ASSERT_EQ(parser.GetIntValue("first"), 52);
    ASSERT_EQ(parser.GetIntValue("number"), 44);
    ASSERT_EQ(parser.GetStringValue("output"), "test3");
}

TEST(ArgParserTestSuite, RepeatedParsingWithDefaultTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    std::string output;
    parser.AddStringArgument('i', "input", "File path for input file");
    parser.AddStringArgument('o', "output", "File path for output directory").StoreValue(output).Default("output.txt");

    ASSERT_TRUE(parser.Parse(SplitString("app --input=test -o=test2")));
    ASSERT_EQ(output, "test2");
    ASSERT_EQ(parser.GetStringValue("output"), "test2");

    ASSERT_TRUE(parser.Parse(SplitString("app --input=test")));
    ASSERT_EQ(output, "output.txt");
    ASSERT_EQ(parser.GetStringValue("output"), "output.txt");
}

TEST(ArgParserTestSuite, BlocksOfShortsTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddStringArgument('i', "input", "File path for input file");
    parser.AddStringArgument('o', "output", "File path for output directory");
    parser.AddStringArgument('d', "date", "Date of rewriting").Default("Today");

    ASSERT_TRUE(parser.Parse(SplitString("app -oid test_test")));
    ASSERT_EQ(parser.GetStringValue("output"), "test_test");
    ASSERT_EQ(parser.GetStringValue("date"), "test_test");
    ASSERT_EQ(parser.GetStringValue("input"), "test_test");

    ASSERT_TRUE(parser.Parse(SplitString("app -oi test_test")));
    ASSERT_EQ(parser.GetStringValue("output"), "test_test");
    ASSERT_EQ(parser.GetStringValue("date"), "Today");
    ASSERT_EQ(parser.GetStringValue("input"), "test_test");
}

TEST(ArgParserTestSuite, MultiPositionalStringTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddStringArgument('i', "input", "File path for input file").Positional();
    parser.AddIntArgument('n', "numbers").Positional().MultiValue();
    parser.AddStringArgument('o', "output", "File path for output directory").Positional();
    parser.AddStringArgument('d', "date", "Date of rewriting").Default("Today").Positional();
    
    ASSERT_TRUE(parser.Parse(SplitString("app input output date 1 10 -5 -1320 343 0")));

    ASSERT_EQ(parser.GetStringValue("output"), "output");
    ASSERT_EQ(parser.GetStringValue("date"), "date");
    ASSERT_EQ(parser.GetStringValue("input"), "input");

    ASSERT_EQ(parser.GetIntValue("numbers", 2), -5);
    ASSERT_EQ(parser.GetIntValue("numbers", 5), 0);
}

TEST(ArgParserTestSuite, MultiPositionalIntTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");

    parser.AddIntArgument('i', "input_int", "File path for input file code").Positional();
    parser.AddIntArgument('o', "output_int", "File path for output directory code").Positional();
    parser.AddIntArgument('d', "date_int", "Date of rewriting").Default(100).Positional();

    parser.AddStringArgument('s', "strings").Positional().MultiValue();
    
    ASSERT_TRUE(parser.Parse(SplitString("app 1 input 10 -5 output 1320 date 343 0")));

    ASSERT_EQ(parser.GetIntValue("input_int"), 1);
    ASSERT_EQ(parser.GetIntValue("output_int"), 10);
    ASSERT_EQ(parser.GetIntValue("date_int"), -5);

    ASSERT_EQ(parser.GetStringValue("strings", 0), "input");
    ASSERT_EQ(parser.GetStringValue("strings", 3), "date");
    ASSERT_EQ(parser.GetStringValue("strings", 2), "1320");
}

TEST(ArgParserTestSuite, InvalidGetTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");

    ASSERT_TRUE(parser.Parse(SplitString("app")));
    ASSERT_EQ(parser.GetStringValue("input"), "");
    ASSERT_EQ(parser.GetIntValue("input"), 0);
    ASSERT_FALSE(parser.GetFlag("input"));
}

TEST(ArgParserTestSuite, InvalidHelpTest) {
    ArgParser parser("My Parser");

    ASSERT_TRUE(parser.Parse(SplitString("app")));
    ASSERT_EQ(parser.HelpDescription(), "Help is not defined");
}

TEST(ArgParserTestSuite, ErrorDefaultWithStoreValueTest) {
    ArgParser parser("My Parser");

    bool flag = false;
    int int_value = 5;
    std::string string_value = "local";

    parser.AddFlag('f', "flag").Default(true).StoreValue(flag);

    parser.AddIntArgument('a', "A_value").StoreValue(int_value).Default(-101);

    parser.AddStringArgument('s', "S_value").Default("TESTVALUE").StoreValue(string_value);

    ASSERT_FALSE(parser.Parse(SplitString("app --help")));

    ASSERT_TRUE(parser.GetFlag("flag"));
    ASSERT_FALSE(flag);
    ASSERT_EQ(parser.GetIntValue("A_value"), -101);
    ASSERT_EQ(int_value, 5);
    ASSERT_EQ(parser.GetStringValue("S_value"), "TESTVALUE");
    ASSERT_EQ(string_value, "local");
}

TEST(ArgParserTestSuite, InvalidBlockOfShortsTest) {
    ArgParser parser("My Parser");

    parser.AddStringArgument('i', "input", "File path for input file").Default("test");
    parser.AddStringArgument('o', "output", "File path for output directory").Default("test");
    parser.AddStringArgument('d', "date", "Date of rewriting").Default("test");
    parser.AddFlag('f', "flag");
    parser.AddIntArgument('a', "a_arg").Default(10);


    ASSERT_FALSE(parser.Parse(SplitString("app -if=10")));

    ASSERT_FALSE(parser.Parse(SplitString("app -a=10 -ifo")));

    ASSERT_FALSE(parser.Parse(SplitString("app -iod")));

    ASSERT_FALSE(parser.Parse(SplitString("app -iod=")));

    ASSERT_FALSE(parser.Parse(SplitString("app -a")));

    ASSERT_FALSE(parser.Parse(SplitString("app -a=")));

    ASSERT_TRUE(parser.Parse(SplitString("app -f -a 10 -iod=tmp")));
}


TEST(ArgParserTestSuite, InvalidLongArgumentsTest) {
    ArgParser parser("My Parser");

    parser.AddStringArgument('i', "input", "File path for input file").Default("test");
    parser.AddStringArgument('o', "output", "File path for output directory").Default("test");
    parser.AddStringArgument('d', "date", "Date of rewriting").Default("test");
    parser.AddFlag('f', "flag");
    parser.AddIntArgument('a', "a_arg").Default(10);


    ASSERT_FALSE(parser.Parse(SplitString("app --input 1 --output= -f -a=10 -d=oo")));

    ASSERT_FALSE(parser.Parse(SplitString("app --input")));

    ASSERT_FALSE(parser.Parse(SplitString("app --a_arg=test")));

    ASSERT_FALSE(parser.Parse(SplitString("app --flag=10")));
}

TEST(ArgParserTestSuite, InvalidPositionalTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument('a', "a_string").Positional();
    parser.AddStringArgument('b', "b_string").Positional();

    ASSERT_FALSE(parser.Parse(SplitString("app value --a_string=value_second")));
    ASSERT_EQ(parser.GetStringValue("a_string"), "value_second");
    ASSERT_EQ(parser.GetStringValue("b_string"), "");
}


TEST(ArgParserTestSuite, TwoPositionalTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument('a', "a_string").Positional();
    parser.AddStringArgument('b', "b_string").Positional();

    ASSERT_TRUE(parser.Parse(SplitString("app value value_second")));
    ASSERT_EQ(parser.GetStringValue("a_string"), "value");
    ASSERT_EQ(parser.GetStringValue("b_string"), "value_second");
}


TEST(ArgParserTestSuite, MultiZeroTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument('a', "a_string").MultiValue();
    parser.AddStringArgument('b', "b_string");

    ASSERT_TRUE(parser.Parse(SplitString("app -b value_second")));
    ASSERT_EQ(parser.GetStringValue("a_string"), "");
    ASSERT_EQ(parser.GetStringValue("b_string"), "value_second");
}


TEST(ArgParserTestSuite, InitizalizationTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddStringArgument('i', "input", "File path for input file");
    parser.AddStringArgument('o', "output", "File path for output directory");
    parser.AddStringArgument('d', "date", "Date of rewriting").Default("Today");

    ASSERT_TRUE(parser.Parse(SplitString("app -oid test_test")));
    ASSERT_EQ(parser.GetStringValue("output"), "test_test");
    ASSERT_EQ(parser.GetStringValue("date"), "test_test");
    ASSERT_EQ(parser.GetStringValue("input"), "test_test");

    ArgParser parser_second = parser;

    ASSERT_EQ(parser.HelpDescription(), parser_second.HelpDescription());

    ASSERT_TRUE(parser_second.Parse(SplitString("app -oi test_second")));
    ASSERT_EQ(parser_second.GetStringValue("output"), "test_second");
    ASSERT_EQ(parser_second.GetStringValue("date"), "Today");
    ASSERT_EQ(parser_second.GetStringValue("input"), "test_second");

    ASSERT_EQ(parser.GetStringValue("output"), "test_test");
    ASSERT_EQ(parser.GetStringValue("date"), "test_test");
    ASSERT_EQ(parser.GetStringValue("input"), "test_test");

    ASSERT_EQ(parser.HelpDescription(), parser_second.HelpDescription());
}

TEST(ArgParserTestSuite, CopyTest) {
    ArgParser parser("My Parser");
    ArgParser parser_second("My Second Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddStringArgument('i', "input", "File path for input file");
    parser.AddStringArgument('o', "output", "File path for output directory");
    parser.AddStringArgument('d', "date", "Date of rewriting").Default("Today");

    parser_second.AddFlag('f', "flag");

    ASSERT_TRUE(parser.Parse(SplitString("app -oid test_test")));
    ASSERT_EQ(parser.GetStringValue("output"), "test_test");
    ASSERT_EQ(parser.GetStringValue("date"), "test_test");
    ASSERT_EQ(parser.GetStringValue("input"), "test_test");

    ASSERT_TRUE(parser_second.Parse(SplitString("app -f")));

    parser_second = parser;

    ASSERT_EQ(parser.GetStringValue("output"), "test_test");
    ASSERT_EQ(parser.GetStringValue("date"), "test_test");
    ASSERT_EQ(parser.GetStringValue("input"), "test_test");

    ASSERT_EQ(parser_second.GetStringValue("output"), "test_test");
    ASSERT_EQ(parser_second.GetStringValue("date"), "test_test");
    ASSERT_EQ(parser_second.GetStringValue("input"), "test_test");

    ASSERT_EQ(parser.HelpDescription(), parser_second.HelpDescription());

    ASSERT_FALSE(parser_second.Parse(SplitString("app -f")));

    ASSERT_EQ(parser_second.GetStringValue("output"), "");
    ASSERT_EQ(parser_second.GetStringValue("date"), "Today");
    ASSERT_EQ(parser_second.GetStringValue("input"), "");

    ASSERT_EQ(parser.GetStringValue("output"), "test_test");
    ASSERT_EQ(parser.GetStringValue("date"), "test_test");
    ASSERT_EQ(parser.GetStringValue("input"), "test_test");

    ASSERT_EQ(parser.HelpDescription(), parser_second.HelpDescription());
}


TEST(ArgParserTestSuite, MultiFlagsTest) {
    ArgParser parser("My Parser");
    std::vector<bool> data;
    parser.AddFlag('f', "flag").MultiValue().StoreValues(data);

    ASSERT_TRUE(parser.Parse(SplitString("app -ffff")));
    ASSERT_EQ(parser.GetFlag("flag", 1), true);
    ASSERT_EQ(data[1], true);
    ASSERT_EQ(data.size(), 4);
}