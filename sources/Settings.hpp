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
	bool getTakingScreensState() const { return keepTakingScreenShots; }
	bool getRestoringState() const { return keepRestoringManaAngHealth; }
	bool getLoadingState() const { return keepLoadingScreenShots; }
	bool getKeepHuntingAutomaticly() const { return keepHuntingAutomaticly; }
	bool getKeepAnalyzeMainGameWindow() const { return keepAnalyzingMainGameWindow; }
	bool getKeepAnalyzeMiniMap() const { return keepAnalyzingMiniMap; }

	bool getKeepAnalyzeStates() const { return keepAnalyzeStates; }
	void setKeepAnalyzeStates(bool enable) { this->keepAnalyzeStates = enable; };
private:
	//std::atomic<bool> newFullImgAwaits;
	std::atomic<bool> keepRestoringManaAngHealth = true;
	std::atomic<bool> keepTakingScreenShots = true;
	std::atomic<bool> keepLoadingScreenShots = true;
	std::atomic<bool> keepAnalyzingMiniMap = true;
	std::atomic<bool> keepAnalyzingMainGameWindow;
	std::atomic<bool> keepHuntingAutomaticly = true;
	std::atomic<bool> keepAnalyzeStates = true;
};