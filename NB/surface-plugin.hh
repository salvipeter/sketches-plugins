#pragma once

#define IN_SKETCHES
#ifdef IN_SKETCHES
#include <QtPlugin>
#endif

#include <surface-interface.hh>
#include <InterpolatingBspline.h>

#ifdef IN_SKETCHES
class NSidedBSplineSurface: public QObject, SurfaceInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "hu.shapex.SurfaceInterface")
  Q_INTERFACES(SurfaceInterface)
#else
class NSidedBSplineSurface: public QObject, SurfaceInterface {
#endif

public:
  virtual int getSurfaceType() const override { return 1; }                     // not important
  virtual QString getSurfaceName() const override { return "Interpolating BSpline"; }
  virtual QString getSurfaceShortName() const override { return "NB"; }
  virtual void setCurves(const std::vector<PCurve*>& curves) override;
  virtual std::vector<double> getData() const override;
  virtual void setData(const std::vector<double> &data) override;
  virtual void setupLoop() override { }                                         // not important
  virtual void update() override { }                                            // not important
  virtual PPoint3D eval(double, double) const override { return {0, 0, 0}; }    // nobody calls this
  virtual PMesh eval(int resolution) const override;

private:
  std::vector<Interpolate::SplineCurve3d> curves_;
};
