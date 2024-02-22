#pragma once
#include "NewProfileConfiguartor.h"
#include "Profession.hpp"
#include "Profile.h"
namespace Ui {
class SelectProfileWindow;
};

class SelectProfileWindow : public QDialog {
  Q_OBJECT

 public:
  SelectProfileWindow(QSharedPointer<Profile> profileToBeChoosen, QWidget* parent = Q_NULLPTR);
  ~SelectProfileWindow();
 public slots:
  void addNewProfileButtonAction();
  void editProfileButtonAction();
  void deleteProfileButtonAction();
  void selectListAction();
  void profSelected();

 private:
  Ui::SelectProfileWindow* ui;
  QSharedPointer<Profile>  profileToBeChoosen;

  void    refreshProfilesOnList();
  void    setUpGui();
  void    readAndSetLastUsedProFromINI();
  QString getSelectedProfName();
};
