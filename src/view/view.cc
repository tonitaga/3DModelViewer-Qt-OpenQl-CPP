#include "view.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include "ui_view.h"

View::View(QWidget *parent) : QMainWindow(parent), ui(new Ui::View) {
  ui->setupUi(this);
  ui->flat_shading->click();
  ui->toolBox->setCurrentIndex(0);

  controller_.SetScene(ui->scene);

  connect(ui->flat_shading, SIGNAL(clicked()), this, SLOT(ShadingRadio()));
  connect(ui->guro_shading, SIGNAL(clicked()), this, SLOT(ShadingRadio()));
  connect(ui->phong_shading, SIGNAL(clicked()), this, SLOT(ShadingRadio()));

  LockUi();
  on_pushButtonZeroSettings_clicked();
  InitializeHotKeys();

  geometry_ = QRect(0, 0, 1160, 764);
  settings_ = new QSettings("Tonijerl", "3DViewer", this);

  misc_ =
      QString::fromStdString(std::filesystem::absolute("../../misc/").string());
  ReadSettings();
}

View::~View() { delete ui; }

void View::on_pushButton_load_model_clicked() {
  QString path =
      QFileDialog::getOpenFileName(0, "Select .obj file", misc_, "*.obj");
  if (!path.isEmpty()) {
    UnlockUi();
    ResetSpinBoxesAndSliders();
    if (!texture_loaded_) ui->pushButton_reset_texture->setDisabled(true);
    controller_.Reset();
    controller_.LoadObjModelToScene(path.toStdString());
    SetModelDataToLabels(path);
    model_is_inited_ = true;
  }
}

void View::on_pushButton_load_texture_clicked() {
  QString path = model_is_inited_ ? QFileDialog::getOpenFileName(
                                        0, "Select .texture file", misc_,
                                        tr("Images(*.png *.jpg *.jpeg)"))
                                  : "";
  if (!path.isEmpty()) {
    texture_loaded_ = true;
    ui->pushButton_reset_texture->setDisabled(false);
    ui->pushButton_reset_texture->setText("Remove Texture");
    ui->flat_shading->click();
    controller_.AddTexture(path.toStdString());
  } else {
    texture_loaded_ = false;
    if (!model_is_inited_)
      QMessageBox::information(
          this, "Texture load error",
          "Sorry! First of all you must import (.obj) file!");
  }
}

void View::SaveSettings() {
  settings_->setValue("geometry", geometry());
  settings_->setValue("line_width", ui->doubleSpinBox_lineWidth->value());
  settings_->setValue("point_size", ui->doubleSpinBox_pointSize->value());
  settings_->setValue("line_type", ui->comboBox_typesLines->currentIndex());
  settings_->setValue("point_type", ui->comboBox_typesPoints->currentIndex());
  settings_->setValue("move_step", ui->lineEdit_stepMove->text().toDouble());
  settings_->setValue("rotate_step",
                      ui->lineEdit_stepRotate->text().toDouble());
  settings_->setValue("scale_step", ui->lineEdit_stepScale->text().toDouble());
  settings_->setValue("line_color_x", controller_.GetColorLine().x);
  settings_->setValue("line_color_y", controller_.GetColorLine().y);
  settings_->setValue("line_color_z", controller_.GetColorLine().z);
  settings_->setValue("point_color_x", controller_.GetColorPoint().x);
  settings_->setValue("point_color_y", controller_.GetColorPoint().y);
  settings_->setValue("point_color_z", controller_.GetColorPoint().z);
  settings_->setValue("background_color_x", controller_.GetColorBackground().x);
  settings_->setValue("background_color_y", controller_.GetColorBackground().y);
  settings_->setValue("background_color_z", controller_.GetColorBackground().z);
}

void View::SaveDefaultSettings() {
  settings_->setValue("geometry", QRect(0, 0, 1160, 764));
  settings_->setValue("line_width", 1.0);
  settings_->setValue("point_size", 2.0);
  settings_->setValue("line_type", 0);
  settings_->setValue("point_type", 0);
  settings_->setValue("move_step", 0.1);
  settings_->setValue("rotate_step", 1);
  settings_->setValue("scale_step", 0.05);
  settings_->setValue("line_color_x", 1.f);
  settings_->setValue("line_color_y", 0.f);
  settings_->setValue("line_color_z", 0.f);
  settings_->setValue("point_color_x", 0.f);
  settings_->setValue("point_color_y", 1.f);
  settings_->setValue("point_color_z", 0.f);
  settings_->setValue("background_color_x", .5f);
  settings_->setValue("background_color_y", .5f);
  settings_->setValue("background_color_z", .5f);
}

void View::ReadSettings() {
  setGeometry(settings_->value("geometry", QRect(0, 0, 1160, 764)).toRect());

  ui->doubleSpinBox_lineWidth->setValue(
      settings_->value("line_width", 1.0).toDouble());
  controller_.SetLineWidth(ui->doubleSpinBox_lineWidth->value());
  ui->doubleSpinBox_pointSize->setValue(
      settings_->value("point_size", 2.0).toDouble());
  controller_.SetPointSize(ui->doubleSpinBox_pointSize->value());
  ui->comboBox_typesLines->setCurrentIndex(
      settings_->value("line_type", 0).toInt());
  controller_.SetLineType(ui->comboBox_typesLines->currentIndex());
  ui->comboBox_typesPoints->setCurrentIndex(
      settings_->value("point_type", 0).toInt());
  controller_.SetPointType(ui->comboBox_typesPoints->currentIndex());

  ui->lineEdit_stepMove->setText(settings_->value("move_step", 0.1).toString());
  ui->lineEdit_stepRotate->setText(
      settings_->value("rotate_step", 1).toString());
  ui->lineEdit_stepScale->setText(
      settings_->value("scale_step", 0.05).toString());

  {
    ng::Vec3 color(settings_->value("line_color_x", 1.f).toFloat(),
                    settings_->value("line_color_y", 0.f).toFloat(),
                    settings_->value("line_color_z", 0.f).toFloat());
    controller_.SetLineColor(color);
    SetColorToLabel(ui->label_colorLines, color);
  }
  {
    ng::Vec3 color(settings_->value("point_color_x", 0.f).toFloat(),
                    settings_->value("point_color_y", 1.f).toFloat(),
                    settings_->value("point_color_z", 0.f).toFloat());
    controller_.SetPointColor(color);
    SetColorToLabel(ui->label_colorPoints, color);
  }
  {
    ng::Vec3 color(settings_->value("background_color_x", .5f).toFloat(),
                    settings_->value("background_color_y", .5f).toFloat(),
                    settings_->value("background_color_z", .5f).toFloat());
    controller_.SetBackgroundColor(color);
    SetColorToLabel(ui->label_colorBackgournd, color);
  }
}

void View::ShadingRadio() {
  if (ui->flat_shading->isChecked()) {
    controller_.ChangeShadow(ng::Shading::kFlatShading);
    on_horizontalSlider_lightPosZ_valueChanged(50);
    ui->horizontalSlider_lightPosZ->setValue(50);
  } else if (ui->guro_shading->isChecked()) {
    controller_.ChangeShadow(ng::Shading::kGuroShading);
    on_horizontalSlider_lightPosZ_valueChanged(-50);
    ui->horizontalSlider_lightPosZ->setValue(-50);
  } else if (ui->phong_shading->isChecked()) {
    controller_.ChangeShadow(ng::Shading::kPhongShading);
    on_horizontalSlider_lightPosZ_valueChanged(-50);
    ui->horizontalSlider_lightPosZ->setValue(-50);
  }
}

void View::on_horizontalSlider_lightPower_valueChanged(int value) {
  controller_.ChangeSceneLightPower(value);
}

void View::on_horizontalSlider_lightPosX_valueChanged(int value) {
  controller_.ChangeSceneLightPos(value, true, false, false);
}

void View::on_horizontalSlider_lightPosY_valueChanged(int value) {
  controller_.ChangeSceneLightPos(value, false, true, false);
}

void View::on_horizontalSlider_lightPosZ_valueChanged(int value) {
  controller_.ChangeSceneLightPos(value, false, false, true);
}

void View::on_pushButton_light_reset_clicked() {
  ui->horizontalSlider_lightPosX->setValue(0);
  ui->horizontalSlider_lightPosY->setValue(0);
  ui->horizontalSlider_lightPosZ->setValue(0);
  ui->horizontalSlider_lightPower->setValue(100);
}

void View::on_pushButton_perspective_clicked() {
  controller_.ChangePerspective(true);
}

void View::on_pushButton_orthographic_clicked() {
  controller_.ChangePerspective(false);
}

void View::on_pushButton_create_image_clicked() { controller_.RecordImage(); }

void View::on_pushButton_create_gif_clicked() { controller_.RecordGif(); }

void View::on_pushButton_create_gif_rotate_clicked() {
  controller_.RecordGifRotate();
}

void View::on_doubleSpinBox_xMove_valueChanged(double arg1) {
  controller_.ChangeMove(arg1, true);
}

void View::on_doubleSpinBox_yMove_valueChanged(double arg1) {
  controller_.ChangeMove(arg1, false, true);
}

void View::on_doubleSpinBox_zMove_valueChanged(double arg1) {
  controller_.ChangeMove(arg1, false, false, true);
}

void View::on_pushButton_reset_texture_clicked() {
  if (texture_loaded_) {
    controller_.ResetTexture();
    if (ui->pushButton_reset_texture->text() == "Texture Overlay") {
      is_texture_view_ = true;
      ui->pushButton_reset_texture->setText("Remove Texture");
    } else {
      is_texture_view_ = false;
      ui->pushButton_reset_texture->setText("Texture Overlay");
    }
  }
}

void View::on_horizontalSlider_rotate_x_valueChanged(int value) {
  controller_.ChangeRotate(value, true);
}

void View::on_horizontalSlider_rotate_y_valueChanged(int value) {
  controller_.ChangeRotate(value, false, true);
}

void View::on_horizontalSlider_rotate_z_valueChanged(int value) {
  controller_.ChangeRotate(value, false, false, true);
}

void View::on_pushButton_light_color_clicked() {
  controller_.ChangeShadowColor(QColorDialog::getColor(Qt::green, this));
}

void View::on_doubleSpinBox_Scale_valueChanged(double arg1) {
  controller_.ChangeScale(arg1);
}

void View::ResetSpinBoxesAndSliders() {
  on_pushButtonZeroScale_clicked();
  on_pushButton_zeroRotate_clicked();
  on_pushButtonZeroMovement_clicked();
}

void View::SetModelDataToLabels(QString path) {
  ui->label_filepath->setText(path);
  ui->label_facets_count->setText(
      QString::number(controller_.GetFacetsCount()));
  ui->label_vertexes_count->setText(
      QString::number(controller_.GetVertexesCount()));
  ui->label_edges_count->setText(QString::number(controller_.GetEdgesCount()));
}

void View::SetColorToLabel(QLabel *label, ng::Vec3 color) {
  QColor qcolor(255 * color.x, 255 * color.y, 255 * color.z);
  QPalette palette = label->palette();
  palette.setColor(label->backgroundRole(), qcolor);
  label->setPalette(palette);
}

void View::InitializeHotKeys() {
  shift_plus_i_ = new QShortcut(this);
  shift_plus_i_->setKey(Qt::SHIFT | Qt::Key_I);
  shift_plus_t_ = new QShortcut(this);
  shift_plus_t_->setKey(Qt::SHIFT | Qt::Key_T);
  shift_plus_r_ = new QShortcut(this);
  shift_plus_r_->setKey(Qt::SHIFT | Qt::Key_R);

  connect(shift_plus_i_, SIGNAL(activated()), this,
          SLOT(on_pushButton_load_model_clicked()));
  connect(shift_plus_t_, SIGNAL(activated()), this,
          SLOT(on_pushButton_load_texture_clicked()));
  connect(shift_plus_r_, SIGNAL(activated()), this,
          SLOT(on_pushButton_reset_texture_clicked()));
}

void View::closeEvent(QCloseEvent *event) {
  auto result = QMessageBox::question(
      this, "Program closing", "Do you want to save the state of the program?");
  if (result == QMessageBox::Yes) {
    SaveSettings();
  } else {
    SaveDefaultSettings();
  }
  event->accept();
}

void View::LockUi() {
  ui->move->setDisabled(true);
  ui->scale->setDisabled(true);
  ui->rotate->setDisabled(true);
  ui->settings->setDisabled(true);
  ui->lightning->setDisabled(true);
  ui->render->setDisabled(true);
  ui->projection->setDisabled(true);
  ui->pushButton_load_texture->setDisabled(true);
  ui->pushButton_reset_texture->setDisabled(true);
}

void View::UnlockUi() {
  ui->move->setDisabled(false);
  ui->scale->setDisabled(false);
  ui->rotate->setDisabled(false);
  ui->settings->setDisabled(false);
  ui->lightning->setDisabled(false);
  ui->render->setDisabled(false);
  ui->projection->setDisabled(false);
  ui->pushButton_load_texture->setDisabled(false);
  ui->pushButton_reset_texture->setDisabled(false);
}

void View::on_lineEdit_stepScale_textChanged(const QString &arg1) {
  double step = arg1.toFloat();
  ui->doubleSpinBox_Scale->setSingleStep(step);
}

void View::on_lineEdit_stepMove_textChanged(const QString &arg1) {
  double step = arg1.toDouble();
  ui->doubleSpinBox_xMove->setSingleStep(step);
  ui->doubleSpinBox_yMove->setSingleStep(step);
  ui->doubleSpinBox_zMove->setSingleStep(step);
}

void View::on_pushButtonZeroMovement_clicked() {
  ui->doubleSpinBox_xMove->setValue(0.0);
  ui->doubleSpinBox_yMove->setValue(0.0);
  ui->doubleSpinBox_zMove->setValue(0.0);
  on_doubleSpinBox_xMove_valueChanged(0.0);
  on_doubleSpinBox_yMove_valueChanged(0.0);
  on_doubleSpinBox_zMove_valueChanged(0.0);
}

void View::on_lineEdit_stepRotate_textChanged(const QString &arg1) {
  double step = arg1.toDouble();
  ui->horizontalSlider_rotate_x->setSingleStep(step);
  ui->horizontalSlider_rotate_y->setSingleStep(step);
  ui->horizontalSlider_rotate_z->setSingleStep(step);
}

void View::on_pushButton_zeroRotate_clicked() {
  ui->horizontalSlider_rotate_x->setValue(0.0);
  ui->horizontalSlider_rotate_y->setValue(0.0);
  ui->horizontalSlider_rotate_z->setValue(0.0);
  on_horizontalSlider_rotate_x_valueChanged(0.0);
  on_horizontalSlider_rotate_y_valueChanged(0.0);
  on_horizontalSlider_rotate_z_valueChanged(0.0);
}

void View::on_pushButtonZeroScale_clicked() {
  ui->doubleSpinBox_Scale->setValue(1.0);
  on_doubleSpinBox_Scale_valueChanged(1.0);
}

void View::on_doubleSpinBox_lineWidth_valueChanged(double arg1) {
  controller_.SetLineWidth(arg1);
}

void View::on_doubleSpinBox_pointSize_valueChanged(double arg1) {
  controller_.SetPointSize(arg1);
}

void View::on_pushButton_selectColorLines_clicked() {
  auto color = QColorDialog::getColor(Qt::green, this);
  controller_.SetLineColor(
      ng::Vec3(color.redF(), color.greenF(), color.blueF()));
  SetColorToLabel(ui->label_colorLines,
                  ng::Vec3(color.redF(), color.greenF(), color.blueF()));
}

void View::on_pushButton_selectColorPoints_clicked() {
  auto color = QColorDialog::getColor(Qt::green, this);
  controller_.SetPointColor(
      ng::Vec3(color.redF(), color.greenF(), color.blueF()));
  SetColorToLabel(ui->label_colorPoints,
                  ng::Vec3(color.redF(), color.greenF(), color.blueF()));
}

void View::on_comboBox_typesPoints_currentIndexChanged(int index) {
  controller_.SetPointType(index);
}

void View::on_comboBox_typesLines_currentIndexChanged(int index) {
  controller_.SetLineType(index);
}

void View::on_pushButton_selectColorBackground_clicked() {
  auto color = QColorDialog::getColor(Qt::green, this);
  controller_.SetBackgroundColor(
      ng::Vec3(color.redF(), color.greenF(), color.blueF()));
  SetColorToLabel(ui->label_colorBackgournd,
                  ng::Vec3(color.redF(), color.greenF(), color.blueF()));
}

void View::on_pushButtonZeroSettings_clicked() {
  controller_.SetLineType(0);
  controller_.SetLineWidth(1);
  controller_.SetLineColor(ng::Vec3(1.f, 0.f, 0.f));
  controller_.SetPointType(0);
  controller_.SetPointSize(2);
  controller_.SetPointColor(ng::Vec3(0.f, 1.f, 0.f));
  controller_.SetBackgroundColor(ng::Vec3(0.5f, 0.5f, 0.5f));

  ui->doubleSpinBox_lineWidth->setValue(1);
  ui->doubleSpinBox_pointSize->setValue(2);
  ui->comboBox_typesLines->setCurrentIndex(0);
  ui->comboBox_typesPoints->setCurrentIndex(0);

  SetColorToLabel(ui->label_colorLines, ng::Vec3(1.f, 0.f, 0.f));
  SetColorToLabel(ui->label_colorPoints, ng::Vec3(0.f, 1.f, 0.f));
  SetColorToLabel(ui->label_colorBackgournd, ng::Vec3(.5f, .5f, .5f));
}

void View::on_comboBox_primitive_type_currentIndexChanged(int index) {
  controller_.SetPrimitiveType(index);
}

void View::wheelEvent(QWheelEvent *event) {
  float step = ui->lineEdit_stepScale->text().toFloat(), new_scale = 0.f;
  if (model_is_inited_) {
    if (event->angleDelta().y() > 0) {
      new_scale = ui->doubleSpinBox_Scale->value() + step;
    } else if (event->angleDelta().y() < 0) {
      new_scale = ui->doubleSpinBox_Scale->value() - step;
    }
    on_doubleSpinBox_Scale_valueChanged(new_scale);
    ui->doubleSpinBox_Scale->setValue(new_scale);
  }
}
