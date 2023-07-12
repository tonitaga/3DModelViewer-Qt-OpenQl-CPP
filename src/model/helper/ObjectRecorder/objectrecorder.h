#ifndef CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_OBJECTRECORDER_OBJECTRECORDER_H
#define CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_OBJECTRECORDER_OBJECTRECORDER_H

#include <QFileDialog>
#include <QTimer>
#include <iostream>

#include "../../../extern/gifimage/qgifimage.h"
#include "../../../view/scene.h"
#include "../VertexData/vertexdata.h"

namespace ng {

// ----------- TypeRecorder enum -----------
// Main information:
// 1) Enumuration has all types of recording types of RecorderContext
enum TypeRecorder { kImage, kGif, kGifRotation };

// ----------- Recorder class -----------
// Main information:
// 1) The class is derived from QWidget
// 2) The class is interface
// 3) The class have own derived classes
//     * RecorderImage class
//     * RecorderGif class
// 4) The class have main pure virtual function RecordObject
class Recorder : protected QWidget {
  Q_OBJECT
 public:
  virtual ~Recorder() = default;
  virtual void RecordObject(Scene *scene, TypeRecorder type) = 0;

 protected:
  TypeRecorder type_;
};

// ----------- RecorderContext class -----------
// Main information:
// 1) The class is a menedger of all derived classes from Recorder
// 2) The class have main function that calling RecordObject function of derived
// class
// 3) The class is like a Strategy pattern
class RecorderContext {
 private:
  Recorder *recoder_;

 public:
  RecorderContext(Recorder *recoder = nullptr) : recoder_(recoder) {}
  ~RecorderContext() { delete recoder_; }

  void SetRecorder(Recorder *recoder);

  void RecordObject(ng::Scene *scene, TypeRecorder type) const;
  void SetMoveForImg() const;
};

// ----------- RecorderImage class -----------
// Main information:
// 1) The class is recording from ng::Scene viewport to image in .jpeg .bmp
// formats 2) The class is derived from Recorder 3) The class is constructing
// from ObjectDataModel
class RecorderImage : public Recorder {
 public:
  void RecordObject(Scene *scene, TypeRecorder type) override;
};

// ----------- RecorderGif class -----------
// 1) The class is recording from ng::Scene viewport to .gif format
// 2) The class is derived from Recorder
// 3) The class is constructing from ObjectDataModel
class RecorderGif : public Recorder {
  Q_OBJECT
 public:
  RecorderGif(ObjectDataModel model);
  ~RecorderGif();

  void RecordObject(Scene *scene, TypeRecorder type) override;

 private:
  ObjectDataModel model_;
  std::size_t counter_ = 0;
  const float kAngle_ = 23 / M_PI;
  float current_angle_ = 0.0f;
  Scene *object_;
  QTimer *timer_ = new QTimer();
  QGifImage *gif_ = new QGifImage;
 public slots:
  void GifFile();
};

}  // end namespace ng

#endif  // !CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_OBJECTRECORDER_OBJECTRECORDER_H
