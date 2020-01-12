#include "surface-plugin.hh"
#include <iostream>

#include "surface-harmonic.hh"

using namespace Transfinite;

#define SURFACE_NAME SurfaceHarmonic

SurfacePlugin::SurfacePlugin()
{
	impl_ = new SURFACE_NAME();
}

SurfacePlugin::~SurfacePlugin()
{
	delete (SURFACE_NAME*)impl_;
}

int SurfacePlugin::getSurfaceType() const
{
	return 1;
}

QString SurfacePlugin::getSurfaceName() const
{
	return "Harmonic Patch";	//replace with the real name !!!
}

QString SurfacePlugin::getSurfaceShortName() const
{
	return "HP";						//replace with the real short name !!!
}

void SurfacePlugin::setCurves(const std::vector<PCurve*>& pcurves)
{
	Surface* surf = (Surface*)impl_;
	if (surf)
	{
		Geometry::CurveVector curves;
		for (unsigned i = 0, ie = pcurves.size(); i < ie; i++)
		{
			PCurve* pcrv = pcurves[i];

			PPoint3DVector& ppts = (PPoint3DVector&)pcrv->cpts_;
			Geometry::PointVector pts;
			for (unsigned j = 0, je = ppts.size(); j < je; j++)
			{
				PPoint3D& pp = ppts[j];
				pts.push_back(Geometry::Point3D(pp[0], pp[1], pp[2]));
			}

			const PRealVector& pknots = pcrv->knots_;
			Geometry::DoubleVector knots;
			for (unsigned j = 0, je = pknots.size(); j < je; j++)
			{
				knots.push_back(pknots[j]);
			}
			curves.push_back(std::make_shared<Geometry::BSCurve>(pcrv->degree_, knots, pts));
		}
		surf->setCurves(curves);
	}
}

std::vector<double> SurfacePlugin::getData() const
{
  return { };
}

void SurfacePlugin::setData(const std::vector<double> &)
{
}

void SurfacePlugin::setupLoop()
{
	Surface* surf = (Surface*)impl_;
	if (surf)
	{
		surf->setupLoop();
	}
}

PPoint3D SurfacePlugin::eval(double u, double v) const
{
	Surface* surf = (Surface*)impl_;
	if (surf)
	{
		Geometry::Point2D p2d(u, v);
		Geometry::Point3D p3d = surf->eval(p2d);
		return PPoint3D(p3d[0], p3d[1], p3d[2]);
	}
	return PPoint3D(0.0, 0.0, 0.0);
}

PMesh SurfacePlugin::eval(int resolution) const
{
	PMesh pmesh;
	Surface* surf = (Surface*)impl_;
	if (surf)
	{
		Geometry::TriMesh mesh = surf->eval(resolution);

		Geometry::PointVector pts = mesh.points();
		PPoint3DVector ppts;
		for (unsigned j = 0, je = pts.size(); j < je; j++)
		{
			Geometry::Point3D p = pts[j];
			ppts.push_back(PPoint3D(p[0], p[1], p[2]));
		}

		std::list<Geometry::TriMesh::Triangle> triangs = mesh.triangles();
		PTriangleVector ptriangs;
		for (std::list<Geometry::TriMesh::Triangle>::iterator it(triangs.begin()), ite(triangs.end()); it != ite; it++)
		{
			Geometry::TriMesh::Triangle triang = *it;
			ptriangs.push_back(PTriangle(triang[0], triang[1], triang[2]));
		}

		pmesh.setPoints(ppts);
		pmesh.setTriangles(ptriangs);
	}
	return pmesh;
}

void SurfacePlugin::update()
{
	Surface* surf = (Surface*)impl_;
	if (surf)
	{
		surf->update();
	}
}

// Added later
//void SurfacePlugin::setRibbonMultiplier(int i, double m);
//void SurfacePlugin::setRibbonHandler(int i, PVector3D h);
//void SurfacePlugin::setCurve(int i, const PCurve* curve);
//void getCurves(QList<PCurve*>& curves);
