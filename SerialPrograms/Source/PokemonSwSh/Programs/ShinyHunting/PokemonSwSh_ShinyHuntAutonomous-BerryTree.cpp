/*  Shiny Hunt Autonomous - Berry Tree
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/Clientside/PrettyPrint.h"
#include "Common/SwitchFramework/FrameworkSettings.h"
#include "Common/SwitchFramework/Switch_PushButtons.h"
#include "Common/PokemonSwSh/PokemonSettings.h"
#include "Common/PokemonSwSh/PokemonSwShGameEntry.h"
#include "Common/PokemonSwSh/PokemonSwShDateSpam.h"
#include "CommonFramework/PersistentSettings.h"
#include "PokemonSwSh/Inference/PokemonSwSh_StartBattleDetector.h"
#include "PokemonSwSh/Inference/ShinyDetection/PokemonSwSh_ShinyEncounterDetector.h"
#include "PokemonSwSh_EncounterTracker.h"
#include "PokemonSwSh_ShinyHuntAutonomous-BerryTree.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSwSh{


ShinyHuntAutonomousBerryTree::ShinyHuntAutonomousBerryTree()
    : SingleSwitchProgram(
        FeedbackType::REQUIRED, PABotBaseLevel::PABOTBASE_12KB,
        "Shiny Hunt Autonomous - Berry Tree",
        "SerialPrograms/ShinyHuntAutonomous-BerryTree.md",
        "Automatically hunt for shiny berry tree " + STRING_POKEMON + " using video feedback."
    )
    , GO_HOME_WHEN_DONE(
        "<b>Go Home when Done:</b><br>After finding a shiny, go to the Switch Home menu to idle. (turn this off for unattended streaming)",
        false
    )
    , REQUIRE_SQUARE(
        "<b>Require Square:</b><br>Stop only for a square shiny. Run from star shinies.",
        false
    )
    , m_advanced_options(
        "<font size=4><b>Advanced Options:</b> You should not need to touch anything below here.</font>"
    )
    , EXIT_BATTLE_MASH_TIME(
        "<b>Exit Battle Time:</b><br>After running, wait this long to return to overworld.",
        "6 * TICKS_PER_SECOND"
    )
    , VIDEO_ON_SHINY(
        "<b>Video Capture:</b><br>Take a video of the encounter if it is shiny.",
        true
    )
    , RUN_FROM_EVERYTHING(
        "<b>Run from Everything:</b><br>Run from everything - even if it is shiny. (For testing only.)",
        false
    )
{
    m_options.emplace_back(&GO_HOME_WHEN_DONE, "GO_HOME_WHEN_DONE");
    m_options.emplace_back(&REQUIRE_SQUARE, "REQUIRE_SQUARE");
    m_options.emplace_back(&m_advanced_options, "");
    m_options.emplace_back(&EXIT_BATTLE_MASH_TIME, "EXIT_BATTLE_MASH_TIME");
    if (settings.developer_mode){
        m_options.emplace_back(&VIDEO_ON_SHINY, "VIDEO_ON_SHINY");
        m_options.emplace_back(&RUN_FROM_EVERYTHING, "RUN_FROM_EVERYTHING");
    }
}



struct ShinyHuntAutonomousBerryTree::Stats : public ShinyHuntTracker{
    Stats()
        : ShinyHuntTracker(true)
        , m_timeouts(m_stats["Timeouts"])
    {
        m_display_order.insert(m_display_order.begin() + 1, Stat("Timeouts"));
    }
    uint64_t& m_timeouts;
};
std::unique_ptr<StatsTracker> ShinyHuntAutonomousBerryTree::make_stats() const{
    return std::unique_ptr<StatsTracker>(new Stats());
}




void ShinyHuntAutonomousBerryTree::program(SingleSwitchProgramEnvironment& env) const{
    grip_menu_connect_go_home(env.console);

    Stats& stats = env.stats<Stats>();
    StandardEncounterTracker tracker(
        stats, env.console,
        REQUIRE_SQUARE,
        EXIT_BATTLE_MASH_TIME,
        VIDEO_ON_SHINY,
        RUN_FROM_EVERYTHING
    );

    uint8_t year = MAX_YEAR;
    while (true){
        env.update_stats();

        home_roll_date_enter_game_autorollback(env.console, &year);
        pbf_mash_button(env.console, BUTTON_B, 90);
        env.console.botbase().wait_for_all_requests();

        {
            StartBattleDetector detector(env.console, std::chrono::seconds(60));

            //  Detect start of battle.
            bool timed_out = false;
            do{
                if (detector.has_timed_out()){
                    env.log("ScreenChangeDetector: Timed out.", Qt::red);
                    stats.m_timeouts++;
                    timed_out = true;
                    break;
                }
                pbf_mash_button(env.console, BUTTON_A, 10);
                env.console.botbase().wait_for_all_requests();
            }while (!detector.detect(env.console.video().snapshot()));

            pbf_mash_button(env.console, BUTTON_B, 5 * TICKS_PER_SECOND);
            if (timed_out){
                pbf_press_button(env.console, BUTTON_HOME, 10, GAME_TO_HOME_DELAY_FAST);
                continue;
            }
        }

        //  Detect shiny.
        ShinyDetection detection = detect_shiny_battle(
            env, env.console,
            SHINY_BATTLE_REGULAR,
            std::chrono::seconds(30)
        );

        if (tracker.process_result(detection)){
            break;
        }
        if (detection == ShinyDetection::NO_BATTLE_MENU){
            stats.m_timeouts++;
            pbf_mash_button(env.console, BUTTON_B, TICKS_PER_SECOND);
            tracker.run_away();
        }

        pbf_press_button(env.console, BUTTON_HOME, 10, GAME_TO_HOME_DELAY_FAST);
    }

    env.update_stats();

    pbf_press_button(env.console, BUTTON_HOME, 10, GAME_TO_HOME_DELAY_SAFE);

    home_to_date_time(env.console, false, false);
    pbf_press_button(env.console, BUTTON_A, 5, 5);
    pbf_press_button(env.console, BUTTON_A, 5, 10);
    pbf_press_button(env.console, BUTTON_HOME, 10, SETTINGS_TO_HOME_DELAY);

    if (!GO_HOME_WHEN_DONE){
        pbf_press_button(env.console, BUTTON_HOME, 10, HOME_TO_GAME_DELAY);
    }

    end_program_callback(env.console);
    end_program_loop(env.console);
}




}
}
}