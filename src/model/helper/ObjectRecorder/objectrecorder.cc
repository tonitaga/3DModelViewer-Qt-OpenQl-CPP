#include "./objectrecorder.h"

#include "../TransformModel/transformmodel.h"

namespace ng {

void RecorderContext::SetRecorder(Recorder* recoder) {
  delete recoder_;
  recoder_ = recoder;
}

void RecorderContext::RecordObject(ng::Scene* scene, TypeRecorder type) const {
  recoder_->RecordObject(scene, type);
}

void ng::RecorderImage::RecordObject(Scene* scene, TypeRecorder type) {
  if (!scene->size().isEmpty()) {
    type_ = type;
    QString path_to_save =
        QFileDialog::getSaveFileName(scene, "Загрузить файл", "/Users/",
                                     "All files (*.*);; JPEG Image (*.jpeg);;"
                                     "BMP Image (*.bmp)");
    scene->grab().save(path_to_save);
  }
}

RecorderGif::RecorderGif(ObjectDataModel model)
    : model_(model),
      counter_(0u),
      kAngle_(23 / M_PI),
      current_angle_(0.f),
      object_(nullptr) {}

RecorderGif::~RecorderGif() {
  delete timer_;
  delete gif_;
}

void RecorderGif::RecordObject(Scene* scene, TypeRecorder type) {
  if (!scene->size().isEmpty()) {
    type_ = type;
    timer_->start(100);
    object_ = scene;
    connect(timer_, SIGNAL(timeout()), this, SLOT(GifFile()));
  }
}

void ng::RecorderGif::GifFile() {
  ++counter_;
  if (type_ == kGifRotation) {
    TransformModel::GetTransform().Rotate(model_, current_angle_, true, false,
                                          false);
    object_->LoadObjectModel(model_.GetVertexes(), model_.GetIndexes());
    object_->update();
    current_angle_ += kAngle_;
  }
  QImage image = object_->grabFramebuffer();
  gif_->setDefaultDelay(10);
  gif_->addFrame(image);
  if (counter_ == 50) {
    timer_->stop();
    QString path_to_save =
        QFileDialog::getSaveFileName(object_, "Save file", "/Users/",
                                     "All Files (*.*);; Gif Image (*.gif);;");
    gif_->save(path_to_save);
    counter_ = 0u;
    image.bits();
    if (type_ == kGifRotation) {
      current_angle_ = 0.f;
      TransformModel::GetTransform().Rotate(model_, 0, true, false, false);
      object_->LoadObjectModel(model_.GetVertexes(), model_.GetIndexes());
      object_->update();
    }
  }
}

}  // namespace ng
