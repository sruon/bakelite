#pragma once
#pragma comment(lib, "pluginsdk.lib")

#include "bakkesmod/plugin/bakkesmodplugin.h"
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

// structure of a stat event
struct StatEventStruct {
  uintptr_t PRI;
  uintptr_t StatEvent;
  // Count always is int_max. No idea why
  uintptr_t Count;
};

class Bakelite : public BakkesMod::Plugin::BakkesModPlugin {
 private:
  std::shared_ptr<bool> bEnabled;
  std::shared_ptr<bool> bShowSummaryOnExit;
  std::shared_ptr<bool> bClearHighlightsOnNewMatch;
  std::shared_ptr<float> fDelay;
  // delay between 2 recordings of same event

 public:
  void onLoad() override;
  void onUnload() override;
  void OnMatchEnter();
  void OnMatchExit();
  void LoadHighlightConfig();
  void LoadGfeSDK();
  void OnKeyPressed(ActorWrapper aw, void* params, std::string eventName);
  void OnStatEvent(ServerWrapper caller, void* args);
  void OnRecordingTrigger(std::string name, int startDelta, int endDelta);
};