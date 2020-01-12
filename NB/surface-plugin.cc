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

std::vector<double> NSidedBSplineSurface::getData() const {
  return { };
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
  return { vertices, triangles };
}
