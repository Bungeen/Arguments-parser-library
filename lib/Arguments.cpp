#include "Arguments.h"


Argument::Argument(char short_name, const std::string& full_name,
                                    const std::string& description)
    : short_name_(short_name)
    , full_name_(full_name)
    , description_(description)
{}

Argument::Argument(const std::string& full_name,
                                    const std::string& description)
    : short_name_(0)
    , full_name_(full_name)
    , description_(description)
{}

bool Argument::IsUsed() const {
    return is_used_;
}

std::string Argument::GetFullName() const {
    return full_name_;
}

char Argument::GetShortName() {
    return short_name_;
}

std::string Argument::GetDescription() const {
    return description_;
}

bool Argument::IsPositional() const {
    return is_positional_;
}

bool Argument::IsMultiValue() const {
    return is_multi_value_;
}


IntArgument::IntArgument(char short_name, std::string full_name,
                                        std::string description)
    : Argument(short_name, full_name, description)
{};

IntArgument::IntArgument(std::string full_name, std::string description)
    : Argument(full_name, description)
{};

IntArgument& IntArgument::Default(int value) {
    default_value_ = value;
    have_default_ = true;
    local_value_ = default_value_;

    return *this;
}

IntArgument& IntArgument::StoreValue(int& value_link) {
    if (linked_value_) {
        return *this;
    }
    
    linked_value_ = &value_link;
    return *this;
}

IntArgument& IntArgument::StoreValues(std::vector<int>& multi_value_link) {
    if (linked_vector_) {
        return *this;
    }
    
    linked_vector_ = &multi_value_link;
    return *this;
}

IntArgument& IntArgument::MultiValue(size_t minimal_count) {
    is_multi_value_ = true;
    minimal_count_ = minimal_count;
    return *this;
}

IntArgument& IntArgument::Positional() {
    is_positional_ = true;
    return *this;
}

int IntArgument::GetValue(int index) const {
    if (!is_multi_value_) {
        return local_value_;
    }

    if (index >= local_vector_.size()) {
        return local_value_;
    }
    
    if (index < 0) {
        return local_value_;
    }
    
    return local_vector_[index];
}

void IntArgument::SetValue(int value) {
    is_used_ = true;
    local_value_ = value;

    if (linked_value_) {
        *linked_value_ = value;
    }
    
    if (!is_multi_value_) {
        return;
    }
    
    local_vector_.push_back(value);

    if (linked_vector_) {
        linked_vector_->push_back(value);
    }
    
}

bool IntArgument::IsValid() {
    if (!is_multi_value_) {
        if (have_default_ && !is_used_ && linked_value_) {
            *linked_value_ = default_value_;
        }
        
        return is_used_ || have_default_;
    }
    
    if (local_vector_.size() >= minimal_count_) {
        return true;
    }
    
    return false;
}

std::string IntArgument::GetParamteres() const{
    std::string result = "";
    bool param_started = false;

    if (have_default_) {
        if (param_started) {
            result += ", default = ";
            result += std::to_string(default_value_);
        } else {
            result += " [default = ";
            result += std::to_string(default_value_);
            param_started = true;
        }    
    }
    
    if (is_multi_value_) {
        if (param_started) {
            result += ", repeated, min args = ";
            result += std::to_string(minimal_count_);
        } else {
            result += " [repeated, min args = ";
            result += std::to_string(minimal_count_);
            param_started = true;
        }    
    }
    
    if (is_positional_) {
        if (param_started) {
            result += ", positional";
        } else {
            result += " [positional";
            param_started = true;
        }    
    }
    
    if (!param_started) {
        return result;
    }
    
    result += ']';
    return result;
}

void IntArgument::SetToDefault() {
    local_value_ = 0;
    if (have_default_) {
        local_value_ = default_value_;
    }
    
    if (is_multi_value_) {
        local_vector_.clear();
    }
    
    is_used_ = false;
}



StringArgument::StringArgument(char short_name, std::string full_name,
                                                std::string description)
    : Argument(short_name, full_name, description)
{};

StringArgument::StringArgument(std::string full_name, std::string description)
    : Argument(full_name, description)
{};

StringArgument& StringArgument::Default(std::string value) {
    default_value_ = value;
    have_default_ = true;
    local_value_ = default_value_;

    return *this;
}

StringArgument& StringArgument::StoreValue(std::string& value_link) {
    if (linked_value_) {
        return *this;
    }
    
    linked_value_ = &value_link;
    return *this;
}

StringArgument& StringArgument::StoreValues(
            std::vector<std::string>& multi_value_link) {
    if (linked_vector_) {
        return *this;
    }
    
    linked_vector_ = &multi_value_link;
    return *this;
}

StringArgument& StringArgument::MultiValue(size_t minimal_count) {
    is_multi_value_ = true;
    minimal_count_ = minimal_count;
    return *this;
}

StringArgument& StringArgument::Positional() {
    is_positional_ = true;
    return *this;
}

std::string StringArgument::GetValue(int index) const {
    if (!is_multi_value_) {
        return local_value_;
    }

    if (index >= local_vector_.size()) {
        return local_value_;
    }
    
    if (index < 0) {
        return local_value_;
    }
    
    return local_vector_[index];
}

void StringArgument::SetValue(std::string value) {
    is_used_ = true;
    local_value_ = value;

    if (linked_value_) {
        *linked_value_ = value;
    }

    if (!is_multi_value_) {
        return;
    }
    
    local_vector_.push_back(value);

    if (linked_vector_) {
        linked_vector_->push_back(value);
    }
    
}

bool StringArgument::IsValid() {
    if (!is_multi_value_) {
        if (have_default_ && !is_used_ && linked_value_) {
            *linked_value_ = default_value_;
        }

        return is_used_ || have_default_;
    }
    
    if (local_vector_.size() >= minimal_count_) {
        return true;
    }
    
    return false;
}

std::string StringArgument::GetParamteres() const {
    std::string result = "";
    bool param_started = false;

    if (have_default_) {
        if (param_started) {
            result += ", default = ";
            result += default_value_;
        } else {
            result += " [default = ";
            result += default_value_;
            param_started = true;
        }    
    }
    
    if (is_multi_value_) {
        if (param_started) {
            result += ", repeated, min args = ";
            result += std::to_string(minimal_count_);
        } else {
            result += " [repeated, min args = ";
            result += std::to_string(minimal_count_);
            param_started = true;
        }    
    }
    
    if (is_positional_) {
        if (param_started) {
            result += ", positional";
        } else {
            result += " [positional";
            param_started = true;
        }    
    }
    
    if (!param_started) {
        return result;
    }
    
    result += ']';
    return result;
}

void StringArgument::SetToDefault() {
    local_value_ = "";
    if (have_default_) {
        local_value_ = default_value_;
    }
    
    if (is_multi_value_) {
        local_vector_.clear();
    }
    
    is_used_ = false;
}



BoolArgument::BoolArgument(char short_name, std::string full_name,
                                            std::string description)
    : Argument(short_name, full_name, description)
{
    is_used_ = true;
};

BoolArgument::BoolArgument(std::string full_name, std::string description)
    : Argument(full_name, description)
{
    is_used_ = true;
};

BoolArgument& BoolArgument::Default(bool value) {
    default_value_ = value;
    have_default_ = true;
    local_value_ = default_value_;

    return *this;
}

BoolArgument& BoolArgument::StoreValue(bool& value_link) {
    if (linked_value_) {
        return *this;
    }

    linked_value_ = &value_link;
    return *this;
}

BoolArgument& BoolArgument::StoreValues(std::vector<bool>& multi_value_link) {
    if (linked_vector_) {
        return *this;
    }
    
    linked_vector_ = &multi_value_link;
    return *this;
}

BoolArgument& BoolArgument::MultiValue(size_t minimal_count) {
    is_multi_value_ = true;
    minimal_count_ = minimal_count;
    return *this;
}

bool BoolArgument::GetValue(int index) const {
    return local_value_;
}

void BoolArgument::SetValue() {    
    local_value_ = true;

    if (linked_value_) {
        *linked_value_ = true;
    }

    if (!is_multi_value_) {
        return;
    }
    
    local_vector_.push_back(true);

    if (linked_vector_) {
        linked_vector_->push_back(true);
    }
    
}

bool BoolArgument::IsValid() {
    if (!is_used_ && linked_value_) {
        *linked_value_ = default_value_;
    }
    
    return true;
}

std::string BoolArgument::GetParamteres() const {
    std::string result = "";
    bool param_started = false;

    if (have_default_) {
        if (param_started) {
            result += ", default = ";
            if (default_value_) {
                result += "true";
            } else {
                result += "false";
            }
        } else {
            result += " [default = ";
            if (default_value_) {
                result += "true";
            } else {
                result += "false";
            }
            param_started = true;
        }    
    }
    
    if (is_multi_value_) {
        if (param_started) {
            result += ", repeated, min args = ";
            result += std::to_string(minimal_count_);
        } else {
            result += " [repeated, min args = ";
            result += std::to_string(minimal_count_);
            param_started = true;
        }    
    }
    
    if (is_positional_) {
        if (param_started) {
            result += ", positional";
        } else {
            result += " [positional";
            param_started = true;
        }    
    }
    
    if (!param_started) {
        return result;
    }
    
    result += ']';
    return result;
}

void BoolArgument::SetToDefault() {
    local_value_ = false;
    if (have_default_) {
        local_value_ = default_value_;
    }
    
    if (is_multi_value_) {
        local_vector_.clear();
    }
}
