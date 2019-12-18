#include <fstream>
#include "surface-plugin.hh"

void NSidedBSplineSurface::setCurves(const std::vector<PCurve*>& curves) {
  curves_.clear();
  for (const auto &c : curves) {
    Interpolate::SplineForm form(c->degree_, c->cpts_.size());
    form.knots = c->knots_;
    std::vector<Interpolate::NBVector3d> cpts;
    for (const auto &cp : c->cpts_)
      cpts.emplace_back(cp[0], cp[1], cp[2]);
    curves_.push_back({ form, cpts });
  }
}

void NSidedBSplineSurface::setData(const std::vector<double> &) {
  // not used
}

static std::vector<Interpolate::NBVector2d>
parameters(const std::vector<Interpolate::NBVector2d> &polygon, size_t resolution) {
  size_t n = polygon.size();
  size_t size = 1 + n * resolution * (resolution + 1) / 2;

  std::vector<Interpolate::NBVector2d> result;
  result.reserve(size);

  Interpolate::NBVector2d center(0, 0);
  for (size_t j = resolution; j >= 1; --j) {
    double u = (double)j / (double)resolution;
    for (size_t k = 0; k < n; ++k) {
      size_t kprev = (k + n - 1) % n;
      for (size_t i = 0; i < j; ++i) {
        double v = (double)i / (double)j;
        auto ep = polygon[kprev] * (1.0 - v) + polygon[k] * v;
        auto p = center * (1.0 - u) + ep * u;
        result.push_back(p);
      }
    }
  }
  result.push_back(center);

  return result;
}

static PTriangleVector triangulate(int n, int resolution)
{
  PTriangleVector result;
  int next_loop_start = 0;
  for (int layer = resolution; layer >= 1; --layer) {
    int index = next_loop_start;
    next_loop_start += layer * n;
    for (int tri = 0; tri < n; ++tri) {
      int next_line_start = next_loop_start + tri * (layer - 1);
      for (int i = 0; i < layer; ++i, ++index) {
        if (tri == n - 1 && i == layer - 1)
          result.push_back({index, index + 1 - n * layer, next_loop_start});
        else
          result.push_back({index, index + 1, next_line_start + i});
        if (i == layer - 1)
          continue;
        if (tri == n - 1 && i == layer - 2)
          result.push_back({next_line_start + i, index + 1, next_loop_start});
        else
          result.push_back({next_line_start + i, index + 1, next_line_start + i + 1});
      }
    }
  }
  return result;
}

PMesh NSidedBSplineSurface::eval(int resolution) const {
  auto s = MakeSurface(curves_);
  auto params = parameters(s.points, resolution);
  PPoint3DVector vertices;
  vertices.reserve(params.size());
  std::transform(params.begin(), params.end(), std::back_inserter(vertices),
                 [&](const Interpolate::NBVector2d &p) -> PPoint3D {
                   auto v = s.surface.Eval(p.x, p.y);
                   return { v.x, v.y, v.z };
                 });
  auto triangles = triangulate(curves_.size(), resolution);

  // Debug outputs
  std::ofstream crv("/tmp/test.crv");
  for (const auto &c : curves_) {
    crv << "Curve starts"<< std::endl;
    crv << "Degree: " << c.form.degree << std::endl;
    crv << "#CPTS: " << c.form.n << std::endl;
    for (auto const &k : c.form.knots)
      crv << k << ' ';
    crv << std::endl;
    for (auto const &p : c.controlPoints)
      crv << p.x << ' ' << p.y << ' ' << p.z << std::endl;
  }
  crv.close();
  std::ofstream dom("/tmp/test.dom");
  for (const auto &p : s.points)
    dom << p.x << ' ' << p.y << std::endl;
  dom.close();
  std::ofstream bsp("/tmp/test.bsp");
  for (const auto &pv : s.surface.controlPoints)
    for (const auto &p : pv)
      bsp << p.x << ' ' << p.y << ' ' << p.z << std::endl;
  bsp.close();
  std::ofstream par("/tmp/test.par");
  for (const auto &p : params)
    par << p.x << ' ' << p.y << std::endl;
  par.close();
  std::ofstream f("/tmp/test.obj");
  for (const auto &p : vertices)
    f << "v " << p[0] << ' ' << p[1] << ' ' << p[2] << std::endl;
  for (const auto &t : triangles)
    f << "f " << t.i_[0] + 1 << ' ' << t.i_[1] + 1 << ' ' << t.i_[2] + 1 << std::endl;
  f.close();
  return { vertices, triangles };
}
