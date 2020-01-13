#pragma once

//#define IN_SKETCHES
#ifdef IN_SKETCHES
#include <QtPlugin>
#endif

#include "surface-interface.hh"

#ifdef IN_SKETCHES
class ExampleSurface: public QObject, SurfaceInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "hu.shapex.SurfaceInterface")
  Q_INTERFACES(SurfaceInterface)
#else
class ExampleSurface: public QObject, SurfaceInterface {
#endif

public:
  virtual int getSurfaceType() const override { return 1; }                     // not important
  virtual QString getSurfaceName() const override { return "Example Surface"; } // <- change this
  virtual QString getSurfaceShortName() const override { return "ES" }          // <- change this
  virtual void setCurves(const std::vector<PCurve*>& curves) override;          // TODO
  virtual std::vector<double> getData() const override;                         // TODO
  virtual void setData(const std::vector<double> &data) override;               // TODO
  virtual void setupLoop() override { }                                         // not important
  virtual void update() override { }                                            // not important
  virtual PPoint3D eval(double u, double v) const override { }                  // nobody calls this
  virtual PMesh eval(int resolution) const override;                            // TODO

private:
  // Put your data here
};
