#pragma once
#include "NewProfileConfiguartor.h"
#include "Profession.hpp"
#include "Profile.h"
namespace Ui {
class SelectProfileWindow;
};
namespace CJ {
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
  QSharedPointer<Profile>  profile_to_be_choosen_;

  void    refreshProfilesOnList();
  void    setUpGui();
  QString getSelectedProfName();
};
}  // namespace CJ
