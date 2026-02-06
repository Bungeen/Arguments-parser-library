#pragma once

#include <string>
#include <vector>
#include <charconv>
#include <system_error>

#include "Arguments.h"

namespace ArgumentParser {

class ArgParser {
    public:
        ArgParser(std::string name);

        ArgParser(const ArgParser& other);

        ~ArgParser();

        ArgParser& operator=(const ArgParser& other);

        bool Parse(std::vector<std::string> base);

        bool Parse(int argc, char** argv);

        IntArgument& AddIntArgument(char short_name,
                const std::string& full_name, std::string description="");
        IntArgument& AddIntArgument(const std::string& full_name,
                                        std::string description="");

        StringArgument& AddStringArgument(char short_name,
                const std::string& full_name, std::string description="");
        StringArgument& AddStringArgument(const std::string& full_name,
                                        std::string description="");

        BoolArgument& AddFlag(char short_name,
                const std::string& full_name, std::string description="");
        BoolArgument& AddFlag(const std::string& full_name,
                                        std::string description="");

        void AddHelp(char short_name,
                const std::string& full_name, std::string description="");

        bool CheckCorrect() const;

        std::string GetStringValue(const std::string& full_name, int index=0);
        int GetIntValue(const std::string& full_name, int index=0);
        bool GetFlag(const std::string& full_name, int index=0);

        bool Help() const;

        std::string HelpDescription() const;
    
    private:
        std::string name_;

        Argument* help_ = nullptr;
        std::vector<UnitArgument> data_;

        bool is_help_ = false;

        UnitArgument* Find(char short_name);
        UnitArgument* Find(const std::string& full_name);

        bool SetArgumentValue(UnitArgument* current_argument,
                    const std::string& value, bool is_bool=false);
        bool LongArgumentParse(std::vector<std::string>& base, int& i);
        bool ShortsArgumentsParse(std::vector<UnitArgument*>& local_data,
                    ArgumentType& local_type, const std::string& argument,
                                                            size_t length);
        bool WriteShortsArgumentsValues(std::vector<UnitArgument*>& local_data,
                                                    const std::string& value);
        bool SetPositionalArgument(const std::string& value,
                                                    bool is_int=false);
        bool BlockOfShortsArgumentsParse(std::vector<std::string>& base,
                                                                    int& i);

        bool CheckHelp(const std::vector<std::string>& base);

        void ClearLocalData();
};

}