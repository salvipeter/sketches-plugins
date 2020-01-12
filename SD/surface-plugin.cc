#include "surface-plugin.hh"
#include <iostream>

#include "surface-superd.hh"

using namespace Transfinite;

#define SURFACE_NAME SurfaceSuperD

SurfacePlugin::SurfacePlugin()
{
  impl_ = new SURFACE_NAME();
}

SurfacePlugin::~SurfacePlugin()
{
  delete reinterpret_cast<SURFACE_NAME*>(impl_);
}

int SurfacePlugin::getSurfaceType() const
{
  return 1;
}

QString SurfacePlugin::getSurfaceName() const
{
  return "SuperD Patch";
}

QString SurfacePlugin::getSurfaceShortName() const
{
  return "SDP";
}

static Point3D convert(const PPoint3D &p) {
  return { p[0], p[1], p[2] };
}

static Point3D intersectLines(const Point3D &ap, const Vector3D &ad,
                              const Point3D &bp, const Vector3D &bd) {
  // always returns a point on the (ap, ad) line
  double a = ad * ad, b = ad * bd, c = bd * bd;
  double d = ad * (ap - bp), e = bd * (ap - bp);
  if (a * c - b * b < 1.0e-7)
    return ap;
  double s = (b * e - c * d) / (a * c - b * b);
  return ap + ad * s;
}

void SurfacePlugin::setCurves(const std::vector<PCurve*>& pcurves)
{
  auto *surf = reinterpret_cast<SURFACE_NAME*>(impl_);
  size_t n = pcurves.size();
  surf->initNetwork(n);
  Point3D sum_fcp(0, 0, 0), sum_ecp(0, 0, 0);
  for (size_t i = 0; i < n; ++i) {
    size_t ip = (i + 1) % n;
    size_t k = pcurves[i]->cpts_.size();
    auto p0 = convert(pcurves[i]->cpts_[0]);
    auto p1 = convert(pcurves[i]->cpts_[1]);
    auto q1 = convert(pcurves[i]->cpts_[k-2]);
    auto q0 = convert(pcurves[i]->cpts_[k-1]);
    surf->setFaceControlPoint(i, p0);
    sum_fcp += p0;
    auto ecp = intersectLines(p0, p1 - p0, q0, q1 - q0);
    surf->setEdgeControlPoint(ip, ecp);
    sum_ecp += ecp;
  }  
  surf->setVertexControlPoint((sum_ecp * 2 - sum_fcp) / n);
}

std::vector<double> SurfacePlugin::getData() const {
  return { reinterpret_cast<SURFACE_NAME*>(impl_)->fullness() };
}

void SurfacePlugin::setData(const std::vector<double> &data)
{
  if (data.size() == 1 && data[0] >= 0 && data[0] <= 1)
    reinterpret_cast<SURFACE_NAME*>(impl_)->setFullness(data[0]);
}

void SurfacePlugin::setupLoop()
{
  auto surf = reinterpret_cast<SURFACE_NAME*>(impl_);
  surf->setupLoop();
  surf->updateRibbons();
}

PPoint3D SurfacePlugin::eval(double u, double v) const
{
  Surface* surf = reinterpret_cast<Surface*>(impl_);
  Geometry::Point2D p2d(u, v);
  Geometry::Point3D p3d = surf->eval(p2d);
  return PPoint3D(p3d[0], p3d[1], p3d[2]);
}

PMesh SurfacePlugin::eval(int resolution) const
{
  PMesh pmesh;
  Surface* surf = reinterpret_cast<Surface*>(impl_);
  Geometry::TriMesh mesh = surf->eval(resolution);

  Geometry::PointVector pts = mesh.points();
  PPoint3DVector ppts;
  for (unsigned j = 0, je = pts.size(); j < je; j++) {
    Geometry::Point3D p = pts[j];
    ppts.push_back(PPoint3D(p[0], p[1], p[2]));
  }

  std::list<Geometry::TriMesh::Triangle> triangs = mesh.triangles();
  PTriangleVector ptriangs;
  for (std::list<Geometry::TriMesh::Triangle>::iterator it(triangs.begin()), ite(triangs.end());
       it != ite; it++) {
    Geometry::TriMesh::Triangle triang = *it;
    ptriangs.push_back(PTriangle(triang[0], triang[1], triang[2]));
  }

  pmesh.setPoints(ppts);
  pmesh.setTriangles(ptriangs);
  return pmesh;
}

void SurfacePlugin::update()
{
  Surface* surf = reinterpret_cast<Surface*>(impl_);
  surf->update();
}
