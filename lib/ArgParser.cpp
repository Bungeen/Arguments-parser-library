#include "ArgParser.h"

namespace AP = ArgumentParser;

AP::ArgParser::ArgParser(std::string name)
    : name_(name)
{}

AP::ArgParser::~ArgParser() {
    for (int i = 0; i < data_.size(); ++i) {
        delete data_[i].argument;
    }

    if (help_) {
        delete help_;
    }
}

AP::ArgParser& AP::ArgParser::operator=(const ArgParser& other) {
    for (int i = 0; i < data_.size(); ++i) {
        delete data_[i].argument;
    }
    data_.clear();

    delete help_;

    for (int i = 0; i < other.data_.size(); ++i) {
        if (other.data_[i].type == ArgumentType::kInt) {
            IntArgument* other_arg =
                static_cast<IntArgument*>(other.data_[i].argument);
            IntArgument* tmp_arg = new IntArgument(*other_arg);
            UnitArgument new_data;
            new_data.type = other.data_[i].type;
            new_data.argument = tmp_arg;
            data_.push_back(new_data);
            continue;
        }
        if (other.data_[i].type == ArgumentType::kString) {
            StringArgument* other_arg =
                static_cast<StringArgument*>(other.data_[i].argument);
            StringArgument* tmp_arg = new StringArgument(*other_arg);
            UnitArgument new_data;
            new_data.type = other.data_[i].type;
            new_data.argument = tmp_arg;
            data_.push_back(new_data);
            continue;
        }
        if (other.data_[i].type == ArgumentType::kBool) {
            BoolArgument* other_arg =
                static_cast<BoolArgument*>(other.data_[i].argument);
            BoolArgument* tmp_arg = new BoolArgument(*other_arg);
            UnitArgument new_data;
            new_data.type = other.data_[i].type;
            new_data.argument = tmp_arg;
            data_.push_back(new_data);
            continue;
        }
    }

    help_ = new Argument(*other.help_);

    is_help_ = other.is_help_;

    name_ = other.name_;

    return *this;
}

AP::ArgParser::ArgParser(const ArgParser& other)
    : name_(other.name_)
    , is_help_(other.is_help_)
{
    for (int i = 0; i < other.data_.size(); ++i) {
        if (other.data_[i].type == ArgumentType::kInt) {
            IntArgument* other_arg =
                static_cast<IntArgument*>(other.data_[i].argument);
            IntArgument* tmp_arg = new IntArgument(*other_arg);
            UnitArgument new_data;
            new_data.type = other.data_[i].type;
            new_data.argument = tmp_arg;
            data_.push_back(new_data);
            continue;
        }
        if (other.data_[i].type == ArgumentType::kString) {
            StringArgument* other_arg =
                static_cast<StringArgument*>(other.data_[i].argument);
            StringArgument* tmp_arg = new StringArgument(*other_arg);
            UnitArgument new_data;
            new_data.type = other.data_[i].type;
            new_data.argument = tmp_arg;
            data_.push_back(new_data);
            continue;
        }
        if (other.data_[i].type == ArgumentType::kBool) {
            BoolArgument* other_arg =
                static_cast<BoolArgument*>(other.data_[i].argument);
            BoolArgument* tmp_arg = new BoolArgument(*other_arg);
            UnitArgument new_data;
            new_data.type = other.data_[i].type;
            new_data.argument = tmp_arg;
            data_.push_back(new_data);
            continue;
        }
    }

    help_ = new Argument(*other.help_);
}

IntArgument& AP::ArgParser::AddIntArgument(char short_name,
                    const std::string& full_name, std::string description) {
    UnitArgument tmp_data;
    tmp_data.type = ArgumentType::kInt;

    IntArgument* tmp_arg = new IntArgument(short_name, full_name, description);

    tmp_data.argument = tmp_arg;
    data_.push_back(tmp_data);
    return *tmp_arg;
}

IntArgument& AP::ArgParser::AddIntArgument(const std::string& full_name,
                                                std::string description) {
    UnitArgument tmp_data;
    tmp_data.type = ArgumentType::kInt;

    IntArgument* tmp_arg = new IntArgument(full_name, description);

    tmp_data.argument = tmp_arg;
    data_.push_back(tmp_data);
    return *tmp_arg;
}



StringArgument& AP::ArgParser::AddStringArgument(char short_name,
                    const std::string& full_name, std::string description) {
    UnitArgument tmp_data;
    tmp_data.type = ArgumentType::kString;

    StringArgument* tmp_arg = new StringArgument(short_name,
                                                full_name, description);

    tmp_data.argument = tmp_arg;
    data_.push_back(tmp_data);
    return *tmp_arg;
}

StringArgument& AP::ArgParser::AddStringArgument(const std::string& full_name,
                                                    std::string description) {
    UnitArgument tmp_data;
    tmp_data.type = ArgumentType::kString;

    StringArgument* tmp_arg = new StringArgument(full_name, description);

    tmp_data.argument = tmp_arg;
    data_.push_back(tmp_data);
    return *tmp_arg;
}



BoolArgument& AP::ArgParser::AddFlag(char short_name,
                    const std::string& full_name, std::string description) {
    UnitArgument tmp_data;
    tmp_data.type = ArgumentType::kBool;

    BoolArgument* tmp_arg = new BoolArgument(short_name,
                                            full_name, description);

    tmp_data.argument = tmp_arg;
    data_.push_back(tmp_data);
    return *tmp_arg;
}

BoolArgument& AP::ArgParser::AddFlag(const std::string& full_name,
                                            std::string description) {
    UnitArgument tmp_data;
    tmp_data.type = ArgumentType::kBool;

    BoolArgument* tmp_arg = new BoolArgument(full_name, description);

    tmp_data.argument = tmp_arg;
    data_.push_back(tmp_data);
    return *tmp_arg;
}

void AP::ArgParser::AddHelp(char short_name, const std::string& full_name,
                                                std::string description) {
    help_ = new Argument(short_name, full_name, description);
}

bool AP::ArgParser::Help() const {
    return is_help_;
}

std::string AP::ArgParser::HelpDescription() const {
    if (!help_) {
        return "Help is not defined";
    }
    
    std::string result = name_;
    result += '\n';
    result += help_->GetDescription();
    result += "\n\n";

    for (int i = 0; i < data_.size(); ++i) {
        if (data_[i].argument->GetShortName()) {
            result += '-';
            result += data_[i].argument->GetShortName();
            result += ",  --";
        } else {
            result += "     --";
        }

        result += data_[i].argument->GetFullName();

        if (data_[i].type == ArgumentType::kInt) {
            result += "=<int>";
        }

        if (data_[i].type == ArgumentType::kString) {
            result += "=<string>";
        }

        result += ",  ";
        result += data_[i].argument->GetDescription();

        if (data_[i].type == ArgumentType::kInt) {
            result += static_cast<IntArgument*>(data_[i].argument)
                        ->GetParamteres();
        }

        if (data_[i].type == ArgumentType::kString) {
            result += static_cast<StringArgument*>(data_[i].argument)
                        ->GetParamteres();
        }

        if (data_[i].type == ArgumentType::kBool) {
            result += static_cast<BoolArgument*>(data_[i].argument)
                        ->GetParamteres();
        }

        result += '\n';
    }

    result += "\n-";
    result += help_->GetShortName();
    result += ", --";
    result += help_->GetFullName();
    result += " Display this help and exit\n";

    return result;
}

bool AP::ArgParser::CheckHelp(const std::vector<std::string>& base) {
    if (!help_) {
        return false;
    }

    std::string full_help_name = "--" + help_->GetFullName();
    std::string short_help_name = "-" + help_->GetShortName();

    for (int i = 0; i < base.size(); ++i) {
        if (full_help_name == base[i] || short_help_name == base[i]) {
            is_help_ = true;
            return true;
        }
    }
    
    return false;
}

bool AP::ArgParser::SetPositionalArgument(const std::string& value,
                                                        bool is_int) {
    for (int i = 0; i < data_.size(); ++i) {
        if (is_int && data_[i].type != ArgumentType::kInt) {
            continue;
        }
        
        if (!data_[i].argument->IsPositional()) {
            continue;
        }
        if (data_[i].argument->IsMultiValue()
            && SetArgumentValue(&data_[i], value)) {
            return true;
        }
        if (data_[i].argument->IsUsed()) {
            continue;
        }
        if (SetArgumentValue(&data_[i], value)) {
            return true;
        }
    }
    return false;
}

bool AP::ArgParser::SetArgumentValue(UnitArgument* current_argument,
                                    const std::string& value, bool is_bool) {
    if (!current_argument) {
        return false;
    }

    if (current_argument->type == ArgumentType::kBool) {
        static_cast<BoolArgument*>(current_argument->argument)->SetValue();
        return true;
    }

    if (is_bool) {
        return false;
    }
    
    if (current_argument->type == ArgumentType::kString) {
        static_cast<StringArgument*>(current_argument->argument)
                                                ->SetValue(value);
        return true;
    }

    const char* first = value.data();
    const char* last = first + value.size();
    int tmp_int_value;

    const std::from_chars_result result = std::from_chars(first,
                                                        last, tmp_int_value);

    if (result.ec == std::errc{}) {
        static_cast<IntArgument*>(current_argument->argument)
                                        ->SetValue(tmp_int_value);
        return true;
    }
    
    return false;
}

bool AP::ArgParser::LongArgumentParse(std::vector<std::string>& base, int& i) {
    size_t equival_position = 0;
    UnitArgument* current_argument = nullptr;
    equival_position = base[i].find('=');

    if (equival_position == std::string::npos) {
        std::string argument_name = base[i].substr(2, base[i].length() - 2);

        if (argument_name.length() == 0) {
            return false;
        }

        current_argument = Find(argument_name);

        ++i;
        if (i >= base.size() && SetArgumentValue(current_argument, "", true)) {
            return true;
        } else if (i >= base.size()) {
            return false;
        }

        if (current_argument->type == ArgumentType::kBool) {
            i--;
        }
        

        return SetArgumentValue(current_argument, base[i]);
    }
    std::string argument_name = base[i].substr(2, equival_position - 2);

    if (argument_name.length() == 0) {
        return false;
    }

    current_argument = Find(argument_name);

    if (SetArgumentValue(current_argument, "", true)) {
        return false;
    }

    size_t size_of_substring = base[i].length() - equival_position;
    
    std::string tmp_string_value = base[i].substr(equival_position + 1,
                                                    size_of_substring);
    if (tmp_string_value.length() == 0) {
        return false;
    }
    
    return SetArgumentValue(current_argument, tmp_string_value);
}

bool AP::ArgParser::ShortsArgumentsParse(std::vector<UnitArgument*>& local_data,
                        ArgumentType& local_type, const std::string& argument,
                                                                size_t length) {

    UnitArgument* current_argument = nullptr;

    for (int j = 1; j < length; ++j) {
        current_argument = Find(argument[j]);

        if (!current_argument) {
            return false;
        }

        local_data.push_back(current_argument);

        if (j == 1) {
            local_type = current_argument->type;
            continue;
        }

        if (local_type != current_argument->type) {
            return false;
        }

    }

    return true;
}

bool AP::ArgParser::WriteShortsArgumentsValues(
                    std::vector<UnitArgument*>& local_data,
                                    const std::string& value) {

    for (int j = 0; j < local_data.size(); ++j) {
        if (!SetArgumentValue(local_data[j], value)) {
            return false;
        }
    }

    return true;
}

bool AP::ArgParser::BlockOfShortsArgumentsParse(
                                    std::vector<std::string>& base, int& i) {
    size_t equival_position = base[i].find('=');
    if (equival_position == std::string::npos) {
        const char* first = base[i].data();
        const char* last = first + base[i].size();
        int tmp_int_value;

        const std::from_chars_result result = std::from_chars(first,
                                                    last, tmp_int_value);
        if (result.ec == std::errc{}) {
            return SetPositionalArgument(base[i], true);
        }
        
        std::vector<UnitArgument*> local_data;
        ArgumentType local_type;

        if (!ShortsArgumentsParse(local_data, local_type,
                                    base[i], base[i].length())) {
            return false;
        }
        
        if (local_type == ArgumentType::kBool) {
            for (int j = 0; j < local_data.size(); ++j) {
                if (!SetArgumentValue(local_data[j], "", true)) {
                    return false;
                }
            }
            return true;
        }
        
        ++i;

        if (i >= base.size()) {
            return false;
        }
        

        if (!WriteShortsArgumentsValues(local_data, base[i])) {
            return false;
        }
        
        return true;
    }

    std::vector<UnitArgument*> local_data;
    ArgumentType local_type;

    if (!ShortsArgumentsParse(local_data, local_type, base[i], 
                                                equival_position)) {
        return false;
    }

    if (local_type == ArgumentType::kBool) {
        return false;
    }

    size_t size_of_substring = base[i].length() - equival_position;
    std::string tmp_string_value = base[i].substr(equival_position + 1,
                                                    size_of_substring);

    if (tmp_string_value.length() == 0) {
        return false;
    }
    

    if (!WriteShortsArgumentsValues(local_data, tmp_string_value)) {
        return false;
    }

    return true;
}

bool AP::ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> base(argv, argv + argc);
    return Parse(base);
}

bool AP::ArgParser::Parse(std::vector<std::string> base) {
    ClearLocalData();

    if (CheckHelp(base)) {
        return true;
    }
    
    size_t equival_position = 0;
    UnitArgument* current_argument = nullptr;
    for (int i = 0; i < base.size(); ++i) {
        if (i == 0) {
            continue;
        }

        if (base[i][0] == '-' && base[i][1] == '-' && base[i].length() > 2) {
            if (!LongArgumentParse(base, i)) {
                return false;
            }
            continue;
        }
        
        if (base[i][0] == '-' && base[i].length() > 1) {
            if (!BlockOfShortsArgumentsParse(base, i)) {
                return false;
            }
            continue;
        }

        if (SetPositionalArgument(base[i])) {
            continue;
        }
        
        return false;
    }
    return CheckCorrect();
}

bool AP::ArgParser::CheckCorrect() const {
    for (int i = 0; i < data_.size(); ++i) {
        if (data_[i].type == ArgumentType::kInt) {
            IntArgument* tmp_argument =
                    static_cast<IntArgument*>(data_[i].argument);

            if (!tmp_argument->IsValid()) {
                return false;
            }
            continue;
        }

        if (data_[i].type == ArgumentType::kString) {
            StringArgument* tmp_argument =
                    static_cast<StringArgument*>(data_[i].argument);

            if (!tmp_argument->IsValid()) {
                return false;
            }
            continue;
        }

        BoolArgument* tmp_argument =
                    static_cast<BoolArgument*>(data_[i].argument);
        tmp_argument->IsValid();
    }

    return true;
}

UnitArgument* AP::ArgParser::Find(const std::string& full_name) {
    for (int i = 0; i < data_.size(); ++i) {
        if (data_[i].argument->GetFullName() == full_name) {
            return &data_[i];
        }
    }
    return nullptr;
}

UnitArgument* AP::ArgParser::Find(char full_name) {
    for (int i = 0; i < data_.size(); ++i) {
        if (data_[i].argument->GetShortName() == full_name) {
            return &data_[i];
        }
        
    }
    return nullptr;
}


std::string AP::ArgParser::GetStringValue(const std::string& full_name,
                                                                int index) {
    UnitArgument* current_argument = Find(full_name);
    if (!current_argument) {
        return "";
    }

    if (current_argument->type != ArgumentType::kString) {
        return "";
    }
    
    return static_cast<StringArgument*>(current_argument->argument)
                                                        ->GetValue(index);
}

int AP::ArgParser::GetIntValue(const std::string& full_name, int index) {
    UnitArgument* current_argument = Find(full_name);
    if (!current_argument) {
        return 0;
    }

    if (current_argument->type != ArgumentType::kInt) {
        return 0;
    }

    return static_cast<IntArgument*>(current_argument->argument)
                                                        ->GetValue(index);
}

bool AP::ArgParser::GetFlag(const std::string& full_name, int index) {
    UnitArgument* current_argument = Find(full_name);
    if (!current_argument) {
        return false;
    }
    
    if (current_argument->type != ArgumentType::kBool) {
        return false;
    }

    return static_cast<BoolArgument*>(current_argument->argument)->GetValue();
}

void AP::ArgParser::ClearLocalData() {
    is_help_ = false;
    for (int i = 0; i < data_.size(); ++i) {
        if (data_[i].type == ArgumentType::kInt) {
            static_cast<IntArgument*>(data_[i].argument)->SetToDefault();
            continue;
        }
        if (data_[i].type == ArgumentType::kString) {
            static_cast<StringArgument*>(data_[i].argument)->SetToDefault();
            continue;
        }
        if (data_[i].type == ArgumentType::kBool) {
            static_cast<BoolArgument*>(data_[i].argument)->SetToDefault();
            continue;
        }
    }
}