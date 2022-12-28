#pragma once
#include <atomic>
class Settings {
public:
	Settings() {};
	~Settings() {};
	void setRestoringState(bool enable) { this->keepRestoringManaAngHealth = enable; };
	void setTakingScreensState(bool enable) { this->keepTakingScreenShots = enable; };
	void setLoadingState(bool enable) { this->keepLoadingScreenShots = enable; };
	void setKeepAnalyzeMiniMap(bool enable) { this->keepAnalyzingMiniMap = enable; };
	void setKeepAnalyzeMainGameWindow(bool enable) { this->keepAnalyzingMainGameWindow = enable; };
	void setKeepHuntingAutomaticly(bool enable) { this->keepHuntingAutomaticly = enable; };
	void setKeepAnalyzeStates(bool enable) { this->keepAnalyzeStates = enable; };
	void setKeepHasted(bool enable) { this->keepHasted = enable; };
	void setKeepUpraded(bool enable) { this->keepUpgraded = enable; };
	void setClearVitalityImgs(bool enable) { this->clearVitalityImgs = enable; };

	bool getTakingScreensState() const { return keepTakingScreenShots; }
	bool getRestoringState() const { return keepRestoringManaAngHealth; }
	bool getLoadingState() const { return keepLoadingScreenShots; }
	bool getKeepHuntingAutomaticly() const { return keepHuntingAutomaticly; }
	bool getKeepAnalyzeMainGameWindow() const { return keepAnalyzingMainGameWindow; }
	bool getKeepAnalyzeMiniMap() const { return keepAnalyzingMiniMap; }
	bool getKeepAnalyzeStates() const { return keepAnalyzeStates; }
	bool getKeepHasted() const { return keepHasted; }
	bool getKeepUpraded() const { return keepUpgraded; }
	bool getClearVitalityImgs() const { return clearVitalityImgs; }
	bool getPlayingOnSmallMonitor() const { return playingOnSmallMonitor; }
private:
	std::atomic<bool> keepRestoringManaAngHealth = true;
	std::atomic<bool> keepTakingScreenShots = true;
	std::atomic<bool> keepLoadingScreenShots = true;
	std::atomic<bool> keepAnalyzingMiniMap = false;
	std::atomic<bool> keepAnalyzingMainGameWindow;
	std::atomic<bool> keepHuntingAutomaticly = false;
	std::atomic<bool> keepAnalyzeStates = true;
	std::atomic<bool> keepHasted = true;
	std::atomic<bool> keepUpgraded = false;
	std::atomic<bool> clearVitalityImgs = false;
	bool playingOnSmallMonitor = false;
};