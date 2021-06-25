#include "bakelite.h"
#include <fstream>
#include <sstream>
#include <string>
#include "GfeSDKWrapper.h"
#include "Maps.h"
#include "bakkesmod/wrappers/includes.h"

#include "bakkesmod/wrappers/GameObject/Stats/StatEventWrapper.h"
// clang-format off
#include <gfesdk/bindings/cpp/highlights/highlights_types_cpp.h>
// clang-format on

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

using namespace std;

char const* GROUP1_ID = "GROUP1";

int PGUP_KEY = 17506;

BAKKESMOD_PLUGIN(Bakelite,
                 "Triggers Nvidia Highlights",
                 "1.1",
                 PLUGINTYPE_FREEPLAY)

struct HighlightsDataHolder {
  bool relevant;
  int startDelta;
  int endDelta;
  std::chrono::system_clock::time_point lastCapture;
};

struct FNameStruct {
  int Index;
  int Number;
};
struct KeyPressParams {
  int ControllerID;
  FNameStruct Key;
  unsigned char EventType;
  float AmountDepressed;
  unsigned int bGamepad;
  unsigned int ReturnValue;
};

struct KeyInfo {
  std::string keyName;
  bool isPressed;
};

struct HighlightsData {
  std::string gameName;
  std::string defaultLocale;
  std::map<std::string, HighlightsDataHolder> highlightsData{
      {"Goal", {true, -5000, 3000, std::chrono::system_clock::now()}},
      {"EpicSave", {true, -5000, 3000, std::chrono::system_clock::now()}},
      {"Save", {true, -5000, 3000, std::chrono::system_clock::now()}},
      {"HighFive", {true, -5000, 3000, std::chrono::system_clock::now()}},
      {"Assist", {true, -5000, 3000, std::chrono::system_clock::now()}},
      {"Demolish", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"Demolition", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"Win", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"MVP", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"AerialGoal", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"BackwardsGoal", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"BicycleGoal", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"LongGoal", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"TurtleGoal", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"PoolShot", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"OvertimeGoal", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"HatTrick", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"Playmaker", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"Savior", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"Shot", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"Center", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"Clear", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"FirstTouch", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"BreakoutDamage",
       {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"BreakoutDamageLarge",
       {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"LowFive", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"HoopsSwishGoal",
       {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"BicycleHit", {false, -5000, 3000, std::chrono::system_clock::now()}},
      {"OwnGoal", {true, -5000, 3000, std::chrono::system_clock::now()}}};

  std::vector<NVGSDK_Highlight> highlights;
};

GfeSdkWrapper g_highlights;
HighlightsData g_highlightsConfig;

void Bakelite::LoadHighlightConfig() {
  cvarManager->log("Initializing Nvidia Geforce Experience Wrapper.");
  InitGfeSdkWrapper(&g_highlights);
  g_highlightsConfig.gameName = "Rocket League";
  // TODO: Support user locale
  g_highlightsConfig.defaultLocale = "en-US";

  g_highlightsConfig.highlights.resize(
      g_highlightsConfig.highlightsData.size());

  int i = 0;
  for (auto const& [key, val] : g_highlightsConfig.highlightsData) {
    g_highlightsConfig.highlights[i].id = key.c_str();
    g_highlightsConfig.highlights[i].userInterest = val.relevant;
    // TODO: Figure out relevance of these 2
    g_highlightsConfig.highlights[i].significance =
        static_cast<NVGSDK_HighlightSignificance>(3);
    g_highlightsConfig.highlights[i].highlightTags =
        static_cast<NVGSDK_HighlightType>(0);
    g_highlightsConfig.highlights[i].nameTable = nullptr;
    g_highlightsConfig.highlights[i].nameTableSize = 0;
    ostringstream os;
    os << "Event enabled: " << key.c_str() << " [" << val.startDelta << "ms/"
       << val.endDelta << "ms]";
    cvarManager->log(os.str());
    i++;
  }
}

void Bakelite::onLoad() {
  Bakelite::LoadHighlightConfig();
  bEnabled = std::make_shared<bool>(false);
  bShowSummaryOnExit = std::make_shared<bool>(true);
  bClearHighlightsOnNewMatch = std::make_shared<bool>(true);

  fDelay = std::make_shared<float>(3.f);
  cvarManager
      ->registerCvar("BL_Enable", "1", "Trigger Nvidia Highlights", true, true,
                     0, true, 1)
      .bindTo(bEnabled);
  cvarManager
      ->registerCvar("BL_ShowSummaryOnExit", "0", "Show Nvidia Highlights summary on match exit", true, true,
                     0, true, 1)
      .bindTo(bShowSummaryOnExit);
  cvarManager
      ->registerCvar("BL_ClearHighlightsOnNewMatch", "0",
                     "Delete existing unsaved highlights when new match starts", true, true,
                     0, true, 1)
      .bindTo(bShowSummaryOnExit);
  cvarManager
      ->registerCvar("BL_Delay", "3.0", "Delay between recordings of same type", true, true,
                     0.0, true, 10.0)
      .bindTo(fDelay);


  // Called when icon event happens for player
  gameWrapper->HookEventWithCallerPost<ServerWrapper>(
      "Function TAGame.GFxHUD_TA.HandleStatEvent",
      std::bind(&Bakelite::OnStatEvent, this, std::placeholders::_1,
                std::placeholders::_2));
  // Called when exiting stats screen
  gameWrapper->HookEvent("Function TAGame.GameEvent_TA.Destroyed",
                         std::bind(&Bakelite::OnMatchExit, this));
  // Called when game starts
  gameWrapper->HookEvent("Function GameEvent_Soccar_TA.WaitingForPlayers.BeginState",
                         std::bind(&Bakelite::OnMatchEnter, this));
  // Called when rejoining game
  gameWrapper->HookEvent(
      "Function Engine.PlayerInput.InitInputSystem",
      std::bind(&Bakelite::OnMatchEnter, this));
  gameWrapper->HookEventWithCaller<ActorWrapper>(
      "Function TAGame.GameViewportClient_TA.HandleKeyPress",
      std::bind(&Bakelite::OnKeyPressed, this,
                                 std::placeholders::_1, std::placeholders::_2,
                                 std::placeholders::_3));
  cvarManager->log("Nvidia Shadowplay Init()");
  g_highlights.Init(g_highlightsConfig.gameName.c_str(),
                    g_highlightsConfig.defaultLocale.c_str(),
                    &g_highlightsConfig.highlights[0],
                    g_highlightsConfig.highlights.size(), gameWrapper->GetBakkesModPath().string().c_str(),
                    GetCurrentProcessId());
  cvarManager->log("Nvidia Shadowplay Init() complete.");
  cvarManager->log("Bakelite ready!");
  // Open group now
  g_highlights.OnOpenGroup(GROUP1_ID);
}

void Bakelite::onUnload() {
  cvarManager->log("Nvidia Shadowplay DeInit()");
  g_highlights.DeInit();
  cvarManager->log("Nvidia Shadowplay DeInit complete.");
}
void Bakelite::OnKeyPressed(ActorWrapper aw,
    void* params,
    std::string eventName) {
  KeyPressParams* keyPressData = (KeyPressParams*)params;
  if (keyPressData->Key.Index == PGUP_KEY) {
    cvarManager->log("Player requested opening Nvidia summary.");
    g_highlights.OnOpenSummary(&std::vector<char const*>({GROUP1_ID})[0], 1,
                               NVGSDK_HIGHLIGHT_SIGNIFICANCE_NONE,
                               NVGSDK_HIGHLIGHT_TYPE_NONE);
  }
}


void Bakelite::OnMatchEnter() {
  // Don't close group if user doesn't open summary page or they will lose their recordings.
    if (*bClearHighlightsOnNewMatch)
      g_highlights.OnCloseGroup(GROUP1_ID, true);
  cvarManager->log("Player entered match, creating Highlights group.");
  g_highlights.OnOpenGroup(GROUP1_ID);
}

void Bakelite::OnMatchExit() {
  if (!(*bShowSummaryOnExit))
    return;
  cvarManager->log("Player exited, opening Nvidia summary.");
  g_highlights.OnOpenSummary(&std::vector<char const*>({GROUP1_ID})[0], 1,
                             NVGSDK_HIGHLIGHT_SIGNIFICANCE_NONE,
                             NVGSDK_HIGHLIGHT_TYPE_NONE);
}

void Bakelite::OnRecordingTrigger(std::string name,
                                  int startDelta,
                                  int endDelta) {
  if (!(*bEnabled))
    return;

  // If receiving same events multiple times in a short interval, check if enough time passed.
  std::chrono::duration<double> elapsed_seconds =
      std::chrono::system_clock::now() -
      g_highlightsConfig.highlightsData[name].lastCapture;
  if (elapsed_seconds.count() < *fDelay) {
    return;
  }
  ostringstream os;
  os << "Received event: " << name << " - Will record [" << startDelta << "ms/+"
     << endDelta << "ms]";
  cvarManager->log(os.str());

  g_highlights.OnSaveVideo(name.c_str(), GROUP1_ID, startDelta, endDelta);
  g_highlightsConfig.highlightsData[name].lastCapture =
      std::chrono::system_clock::now();
}

void Bakelite::OnStatEvent(ServerWrapper caller, void* args) {
  auto tArgs = (StatEventStruct*)args;

  auto statEvent = StatEventWrapper(tArgs->StatEvent);
  std::string eventString = statEvent.GetEventName();
  if (g_highlightsConfig.highlightsData.find(eventString) !=
      g_highlightsConfig.highlightsData.end()) {
    cvarManager->log("Found config for event of type: " + eventString);
    OnRecordingTrigger(
        eventString, g_highlightsConfig.highlightsData[eventString].startDelta,
        g_highlightsConfig.highlightsData[eventString].endDelta);
  } else {
    cvarManager->log("Could not find config for event of type: " + eventString);
  }
}
