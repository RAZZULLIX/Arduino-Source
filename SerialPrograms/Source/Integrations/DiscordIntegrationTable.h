/*  Discord Integration Table
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_DiscordIntegrationTable_H
#define PokemonAutomation_DiscordIntegrationTable_H

#include "Common/Cpp/Options/BooleanCheckBoxOption.h"
#include "Common/Cpp/Options/StringOption.h"
#include "Common/Cpp/Options/EditableTableOption.h"

namespace PokemonAutomation{
namespace Integration{



class DiscordIntegrationChannel : public EditableTableRow{
public:
    DiscordIntegrationChannel();
    virtual std::unique_ptr<EditableTableRow> clone() const override;
    virtual void load_json(const JsonValue& json) override;

public:
    BooleanCheckBoxCell enabled;
    StringCell label;
    BooleanCheckBoxCell ping;
    StringCell tags_text;
    BooleanCheckBoxCell allow_commands;
    StringCell channel_id;
};

class DiscordIntegrationTable : public EditableTableOption_t<DiscordIntegrationChannel>{
public:
    DiscordIntegrationTable();
    virtual std::vector<std::string> make_header() const override;
    std::vector<std::string> command_channels() const;
};






}
}
#endif
