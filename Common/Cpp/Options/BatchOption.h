/*  Batch Option
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_Options_BatchOption_H
#define PokemonAutomation_Options_BatchOption_H

#include "Common/Cpp/Containers/Pimpl.h"
#include "Common/Cpp/Containers/FixedLimitVector.h"
#include "ConfigOption.h"

namespace PokemonAutomation{

// A ConfigOption that groups one or more options.
class BatchOption : public ConfigOption{
public:
    ~BatchOption();
    BatchOption(bool horizontal = false);

public:
    //  This is not thread-safe with the rest of this class. You must
    //  fully initialize the class (by adding all the options it will ever have)
    //  before you start using it.
    void add_option(ConfigOption& option, std::string serialization_string);
#define PA_ADD_STATIC(x)    add_option(x, "")
#define PA_ADD_OPTION(x)    add_option(x, #x)


public:
    virtual void load_json(const JsonValue& json) override;
    virtual JsonValue to_json() const override;

    std::string check_validity() const override;
    virtual void restore_defaults() override;
    virtual void reset_state() override;

    virtual ConfigWidget* make_ui(QWidget& parent) override;

    bool horizontal() const;
    FixedLimitVector<ConfigOption*> options() const;


private:
    struct Data;
    Pimpl<Data> m_data;
};




}
#endif
