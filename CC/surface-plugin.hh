#include <QObject>
#include <QtPlugin>

#include "surface-interface.hh"

class SurfacePlugin : public QObject, SurfaceInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "hu.shapex.SurfaceInterface")
  Q_INTERFACES(SurfaceInterface)

  public:
  SurfacePlugin();
  virtual ~SurfacePlugin();

  virtual int getSurfaceType() const override;
  virtual QString getSurfaceName() const override;
  virtual QString getSurfaceShortName() const override;

  virtual void setCurves(const std::vector<PCurve*>& curves) override;
  virtual std::vector<double> getData() const override;
  virtual void setData(const std::vector<double> &data) override;
  virtual void setupLoop() override;
  virtual void update() override;

  virtual PPoint3D eval(double u, double v) const override;
  virtual PMesh eval(int resolution) const override;

private:
  void* impl_;
};

