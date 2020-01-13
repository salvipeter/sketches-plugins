#pragma once

#include "plugin-interface.hh"

#ifdef IN_SKETCHES
#include <QPluginLoader>
#endif

class SurfaceInterface {
public:
	virtual			~SurfaceInterface() {}

	virtual int		getSurfaceType() const = 0;
	virtual QString		getSurfaceName() const = 0;
	virtual QString		getSurfaceShortName() const = 0;

//	virtual void		setRibbonMultiplier(int i, double m) = 0;
//	virtual void		setRibbonHandler(int i, PVector3D h) = 0;
//	virtual void		setCurve(int i, const PCurve* curve) = 0;
	virtual void		setCurves(const std::vector<PCurve*>& curves) = 0;
	virtual std::vector<double> getData() const = 0;
	virtual void		setData(const std::vector<double> &data) = 0;
	virtual void		setupLoop() = 0;
	virtual void		update() = 0;

	virtual PPoint3D	eval(double u, double v) const = 0;
	virtual PMesh		eval(int resolution) const = 0;

//	virtual void		getCurves(QList<PCurve*>& curves) = 0;
};

#ifdef IN_SKETCHES
Q_DECLARE_INTERFACE(SurfaceInterface, "hu.shapex.SurfaceInterface")
#endif
