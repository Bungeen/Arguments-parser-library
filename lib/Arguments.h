#pragma once

#include <string>
#include <vector>
#include <cstdint>


enum class ArgumentType {
    kInt,
    kString,
    kBool,
};


class Argument {
    public:
        Argument(char short_name, const std::string& full_name,
                                    const std::string& description="");

        Argument(const std::string& full_name,
                                    const std::string& description="");

        bool IsUsed() const;

        std::string GetFullName() const;

        char GetShortName();

        std::string GetDescription() const;

        bool IsPositional() const;

        bool IsMultiValue() const;
    
    private:
        char short_name_;
        std::string full_name_;
        std::string description_;
    
    protected:
        bool is_used_ = false;
        bool is_positional_ = false;
        bool is_multi_value_ = false;
        bool have_default_ = false;
        size_t minimal_count_ = 0;
};

class IntArgument : public Argument {
    public:
        IntArgument(char short_name, std::string full_name,
                                    std::string description="");


        IntArgument(std::string full_name, std::string description="");

        IntArgument& StoreValue(int& value_link);

        IntArgument& StoreValues(std::vector<int>& vector_value_link);

        IntArgument& Default(int value);

        IntArgument& MultiValue(size_t minimal_count=0);

        IntArgument& Positional();

        int GetValue(int index=0) const;
        void SetValue(int value);

        bool IsValid();

        std::string GetParamteres() const;

        void SetToDefault();
    
    private:
        int default_value_ = 0;
        int local_value_ = 0;
        int* linked_value_ = nullptr;

        std::vector<int> local_vector_;
        std::vector<int>* linked_vector_ = nullptr;
  
};


class StringArgument : public Argument {
    public:
        StringArgument(char short_name, std::string full_name,
                                    std::string description="");

        StringArgument(std::string full_name, std::string description="");

        StringArgument& StoreValue(std::string& value_link);

        StringArgument& StoreValues(std::vector<std::string>& vector_value_link);

        StringArgument& Default(std::string value);

        StringArgument& MultiValue(size_t minimal_count=0);

        StringArgument& Positional();

        std::string GetValue(int index=0) const;
        void SetValue(std::string value);

        bool IsValid();

        std::string GetParamteres() const;

        void SetToDefault();
    
    private:
        std::string default_value_ = "";
        std::string local_value_ = "";
        std::string* linked_value_ = nullptr;

        std::vector<std::string> local_vector_;
        std::vector<std::string>* linked_vector_ = nullptr;

};


class BoolArgument : public Argument {
    public:
        BoolArgument(char short_name, std::string full_name,
                                    std::string description="");

        BoolArgument(std::string full_name, std::string description="");
        BoolArgument& StoreValue(bool& value_link);

        BoolArgument& StoreValues(std::vector<bool>& vector_value_link);

        BoolArgument& Default(bool value);

        BoolArgument& MultiValue(size_t minimal_count=0);

        bool GetValue(int index=0) const;
        void SetValue();

        bool IsValid();

        std::string GetParamteres() const;

        void SetToDefault();

    private:
        bool default_value_ = false;
        bool local_value_ = false;
        bool* linked_value_ = nullptr;

        std::vector<bool> local_vector_;
        std::vector<bool>* linked_vector_ = nullptr;
        
};

struct UnitArgument{
    ArgumentType type;
    Argument* argument;
};
