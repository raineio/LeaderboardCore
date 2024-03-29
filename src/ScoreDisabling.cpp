#include "ScoreDisabling.hpp"
#include "logger.h"

namespace LeaderboardCore::Scoring {
    std::unordered_map<std::string, ScoreDisabling::DisableMap> ScoreDisabling::disablingMods;
    void ScoreDisabling::Disable(const ModInfo& modInfo, const std::string_view& reason) {
        for (auto& [leaderboardId, disablingModInfos] : disablingMods) {
            auto modItr = disablingModInfos.find(modInfo.id);
            if (modItr == disablingModInfos.end()) {
                disablingModInfos.emplace(modInfo.id, reason);
            } else {
                INFO("Mod with ID {} tried to disable scoring twice on leaderboard {}", modInfo.id, leaderboardId);
            }
        }
    }

    void ScoreDisabling::Disable(const std::string& leaderboardId, const ModInfo& modInfo, const std::string_view& reason) {
        auto lbItr = disablingMods.find(leaderboardId);
        if (lbItr == disablingMods.end()) {
            lbItr = disablingMods.emplace(leaderboardId, DisableMap()).first;
            lbItr->second.emplace(modInfo.id, reason);
        } else {
            auto modItr = lbItr->second.find(modInfo.id);
            if (modItr == lbItr->second.end()) {
                lbItr->second.emplace(modInfo.id, reason);
            } else {
                INFO("Mod with ID {} tried to disable scoring twice on leaderboard {}", modInfo.id, leaderboardId);
            }
        }
    }

    void ScoreDisabling::Enable(const ModInfo& modInfo) {
        for (auto& [leaderboardId, disablingModInfos] : disablingMods) {
            auto modItr = disablingModInfos.find(modInfo.id);
            if (modItr != disablingModInfos.end()) {
                disablingModInfos.erase(modItr);
            } else {
                INFO("Mod with ID {} tried to enable scoring on leaderboard {}, but wasn't disabling!", modInfo.id, leaderboardId);
            }
        }
    }

    void ScoreDisabling::Enable(const std::string& leaderboardId, const ModInfo& modInfo) {
        auto lbItr = disablingMods.find(leaderboardId);
        if (lbItr != disablingMods.end()) {
            auto modItr = lbItr->second.find(modInfo.id);
            if (modItr != lbItr->second.end()) {
                lbItr->second.erase(modItr);
            } else {
                INFO("Mod with ID {} tried to enable scoring on leaderboard {}, but wasn't disabling!", modInfo.id, leaderboardId);
            }
        } else {
            INFO("Mod with ID {} tried to enable scoring on leaderboard {}, but that leaderboard did not exist!", modInfo.id, leaderboardId);
        }
    }

    void ScoreDisabling::RegisterLeaderboard(const std::string& leaderboardId) {
        auto lbItr = disablingMods.find(leaderboardId);
        if (lbItr == disablingMods.end()) disablingMods.emplace(leaderboardId, DisableMap());
    }

    bool ScoreDisabling::GetScoreSubmissionAllowed(const std::string& leaderboardId) {
        if (leaderboardId == "") {
            for (const auto& [id, disablingModInfos] : disablingMods) {
                if (!disablingModInfos.empty()) return false;
            }
            return true;
        } else {
            auto lbItr = disablingMods.find(leaderboardId);
            if (lbItr == disablingMods.end()) return true;
            else return disablingMods.empty();
        }
    }
    const ScoreDisabling::DisableMap& ScoreDisabling::GetDisablingModIds(const std::string& leaderboardId) {
        auto lbItr = disablingMods.find(leaderboardId);
        if (lbItr != disablingMods.end()) return lbItr->second;

        static DisableMap empty;
        return empty;
    }

    std::vector<std::string> ScoreDisabling::GetLeaderboardIds() {
        std::vector<std::string> ids;
        ids.reserve(disablingMods.size());
        for (const auto& [id, disablingModInfos] : disablingMods) {
            ids.emplace_back(id);
        }
        return ids;
    }
}